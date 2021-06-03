#pragma once

#include <vector>
#include <algorithm>
#include <array>

#include "detection/DetectionPartition.h"
#include "detection/IDetectionProcess.h"

namespace Async
{

	using namespace std;

	template<class ExecutionContext>
	class WidebandDetector : public IDetectionProcess
	{

	public:

		WidebandDetector(ExecutionContext& ex) :
			m_ExecutionContext{ ex }
		{
		}

		template<class MutableBufferType>
		void detect(MutableBufferType& buff)
		{
			auto spectrum{ get<Antenna::Idx::Spectrum>(buff) };
			int spectrumLen{ static_cast<int>(
				get<Antenna::Idx::SpectrumLen>(buff))
			};
			auto trimedQuantizedLevels{
				TrimAndQuantizedLevels(spectrum, spectrumLen)
			};
			auto noiseFloorEstimation{
				FindNoiseFloor(trimedQuantizedLevels, spectrumLen)
			};
			auto parts{
				FindPartitions(
					spectrum,
					spectrumLen,
					trimedQuantizedLevels,
					noiseFloorEstimation)
			};
			auto partionPSD{ TakePartitionPSD(spectrumLen, parts) };
			SiftPartitions(buff, parts, noiseFloorEstimation, partionPSD);
		}

		template<class MutableBufferType, class HandlerType>
		void async_detect(MutableBufferType& buff, HandlerType&& handler)
		{
			m_ExecutionContext.send(
				[&, handler{ forward<HandlerType>(handler) }]() mutable
			{
				detect(buff);
				handler();
			});
		}

		void AsyncProcess(
			Antenna::Type& antenna,
			int nMinSNR,
			int nMinBW,
			const std::function<void()>& handler)
		{
			async_detect(antenna, handler);
		}

		void Config(unsigned long long nSigLen)
		{}

		void SetMinBW(double dMinBW)
		{}

		void SetStartFrequency(double dStartFrequency)
		{}

		void SetStepFrequency(double dStepFrequency)
		{}

		void ResetFlag() 
		{}

	private:

		vector<float> TakePartitionPSD(
			int spectrum_len,
			const vector<DetectionPartition>& parts)
		{
			vector<float> partionPSD;
			partionPSD.resize(spectrum_len);
			for (auto& part : parts)
			{
				for (int nK = part.nStartIndx; nK <= part.nStopIndx; nK++)
				{
					partionPSD[nK] = part.fMeanLevel;
				}
			}
			return partionPSD;
		}

		template<class MutableBufferType>
		void SiftPartitions(
			MutableBufferType& buff,
			const vector<DetectionPartition>& parts,
			int noiseFloorEstimation,
			const vector<float>& partionPSD)
		{
			auto spectrumLen{ get<Antenna::Idx::SpectrumLen>(buff) };
			for (int nI = 0; nI < parts.size(); nI++)
			{
				if (!(HaveIndicesValidRanges(parts[nI], spectrumLen)
					&& IsNotNarrow(parts[nI])
					&& HasEnoughLevel(parts[nI], noiseFloorEstimation)
					&& AreNeighboursLower(parts[nI], partionPSD)))
				{
					continue;
				}

				get<Antenna::Idx::Partitions>(buff).push_back(parts[nI]);
			}
		}

		bool AreNeighboursLower(
			const DetectionPartition& part, 
			const vector<float>& partionPSD) const
		{
			return part.fMeanLevel > partionPSD[part.nStartIndx - 1]
				&& part.fMeanLevel > partionPSD[part.nStopIndx + 1];
		}

		bool HaveIndicesValidRanges(
			const DetectionPartition& part, 
			size_t spectrumLen) const
		{
			return part.nStartIndx > 0 && part.nStopIndx < (spectrumLen - 1);
		}

		bool IsNotNarrow(const DetectionPartition& part) const
		{
			return part.nStopIndx - part.nStartIndx > 500;
		}

		bool HasEnoughLevel(
			const DetectionPartition& part, 
			int noiseFloorEstimation) const
		{
			return part.fMeanLevel > noiseFloorEstimation + 10;
		}

		int Quantize(float value, int levelDepth, int levelResolution)
		{
			return trunc(
				(value + levelDepth) / levelResolution);
		}

		auto TrimAndQuantizedLevels(float* spectrum, size_t spectrumLen)
		{
			vector<vector<int>> trimedQuantizedLevels{
				vector<vector<int>>(m_TrimedLevelRowSize) };

			for (int i = 0; i < spectrumLen; i++)
			{
				auto quantizedValue{
					Quantize(spectrum[i], m_SpectrumDepth, m_ScaleRatio) };
				auto level{
					clamp(quantizedValue, 0, m_TrimedLevelRowSize - 1) };
				trimedQuantizedLevels[level].push_back(i);
			}
			return trimedQuantizedLevels;
		}

		int FindNoiseFloor(
			const vector<vector<int>>& trimedQuantizedLevels,
			int spectrumLen)
		{
			int maxLevel = 0;
			int secondMaxLevel = 0;
			size_t secondMaxLevelSize = 0;
			size_t maxLevelSize = 0;
			for (int nIndx = 0; nIndx < trimedQuantizedLevels.size(); nIndx++)
			{
				if (maxLevelSize < trimedQuantizedLevels[nIndx].size())
				{
					secondMaxLevelSize = maxLevelSize;
					maxLevelSize = trimedQuantizedLevels[nIndx].size();
					maxLevel = secondMaxLevel;
					secondMaxLevel = -1 * m_SpectrumDepth + (nIndx * m_ScaleRatio);
				}
			}
			int noiseFloor = 0;
			if (abs(static_cast<int>(secondMaxLevelSize - maxLevelSize))
				< (int)(.5 * spectrumLen))
			{
				noiseFloor = min(maxLevel, secondMaxLevel);
			}
			else
			{
				noiseFloor = secondMaxLevel;
			}
			return noiseFloor;
		}

		int GetLevelThreshold(
			float avg,
			int noiseFloorEstimation)
		{
			int nMagnetudeForDetection = 15;
			int levelThreshold = nMagnetudeForDetection;
			if (int((avg - noiseFloorEstimation)) < nMagnetudeForDetection
				&& int((avg - noiseFloorEstimation)) > (-1 * nMagnetudeForDetection))
			{
				levelThreshold = 9;
			}
			else if (int(0.5 * (avg - noiseFloorEstimation)) < nMagnetudeForDetection
				&& int(0.5 * (avg - noiseFloorEstimation)) > (-1 * nMagnetudeForDetection))
			{
				levelThreshold = 12;
			}
			else
			{
				levelThreshold = 15;
			}
			return levelThreshold;
		}

		bool IsNotPreccessedAndInRegion(
			const vector<bool>& spectIdxProcessedList,
			float avg,
			float* spectrum,
			int idx,
			int noiseFloorEstimation)
		{
			if (spectIdxProcessedList[idx] == false)
			{
				auto levelThreshold{ GetLevelThreshold(avg, noiseFloorEstimation) };
				auto fDiff{ abs(spectrum[idx] - avg) };

				if (fDiff < levelThreshold)
				{

					return true;
				}
			}
			return false;
		}

		DetectionPartition FindPartition(
			int spectrumIdx,
			float* spectrum,
			size_t spectrumLen,
			vector<bool>& spectIdxProcessedList,
			int noiseFloorEstimation)
		{

			int nPartStart{ spectrumIdx };
			int nPartStop{ spectrumIdx };
			float avg{ spectrum[spectrumIdx] };
			int avgCount{ 1 };
			int nStart{ max(spectrumIdx - 1, 0) };
			int nStop{ min(spectrumIdx + 1,
				static_cast<int>(spectrumLen) - 1) };
			float fSum{ spectrum[spectrumIdx] };

			int levelThreshold{
				GetLevelThreshold(avg, noiseFloorEstimation)
			};

			float fDiffStart{ 0 };
			float fDiffStop{ 0 };
			bool isInRegion{ true };

			while (isInRegion)
			{
				isInRegion = false;

				if (IsNotPreccessedAndInRegion(
					spectIdxProcessedList, avg, spectrum, nStart, noiseFloorEstimation))
				{
					avgCount++;
					fSum += spectrum[nStart];
					avg = fSum / avgCount;
					nPartStart = nStart;
					spectIdxProcessedList[nStart] = true;
					nStart = max(nStart - 1, 0);
					isInRegion = true;
				}

				if (IsNotPreccessedAndInRegion(
					spectIdxProcessedList, avg, spectrum,
					nStop, noiseFloorEstimation))
				{
					avgCount++;
					fSum += spectrum[nStop];
					avg = fSum / avgCount;
					spectIdxProcessedList[nStop] = true;
					nPartStop = nStop;
					nStop = min(nStop + 1, static_cast<int>(spectrumLen) - 1);
					isInRegion = true;
				}

			}

			DetectionPartition part;
			part.nStartIndx = nPartStart;
			part.nStopIndx = nPartStop;
			part.fMeanLevel = avg;
			part.nCenterIndx = (int)(.5 * (nPartStop - nPartStart));
			return part;
		}

		vector<DetectionPartition> FindPartitions(
			float* spectrum,
			size_t spectrumLen,
			const vector<vector<int>>& trimedQuantizedLevels,
			int noiseFloorEstimation)
		{
			vector<DetectionPartition> parts;
			vector<bool> spectIdxProcessedList(spectrumLen, false);

			for_each(
				trimedQuantizedLevels.rbegin(),
				trimedQuantizedLevels.rend(),
				[&](auto& level)
				{
					for (auto spectrumIdx : level)
					{
						if (spectIdxProcessedList[spectrumIdx] == true)
						{
							continue;
						}
						parts.push_back(
							FindPartition(
								spectrumIdx,
								spectrum,
								spectrumLen,
								spectIdxProcessedList,
								noiseFloorEstimation));
					}
				});

			return parts;
		}

		const int m_ScaleRatio = 5;
		const int m_TrimedLevelRowSize = 29;
		const int m_SpectrumDepth = m_ScaleRatio * (m_TrimedLevelRowSize - 1);
		std::function<void()> m_handler;
		ExecutionContext& m_ExecutionContext;
	};

}