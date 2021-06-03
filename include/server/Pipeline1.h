#pragma once

#include <filesystem>

#include "offline_mode/WAVFile.h"

#include "async/IPipeline.h"
#include "offline_mode/ReadData.h"
#include "offline_mode/WAVFileFactory.h"

#include "offline_mode/virtual_ddc_pipeline/Types.h"


namespace VirtualDDCPipeline {

	using namespace std;
	using namespace std::filesystem;
	using namespace Async;
	using namespace VirtualUtils;

	class Pipeline : public IPipeline<StageType> {

	public:

		Pipeline(
			shared_ptr<IVirtualCardConfig> config,
			StageType& back_stage,
			function<void()> push_buffer_handler)
			:_config{config},
			_read_data{ _config ,_wav_file_factory },
			_read_file{
				[&, push_buffer_handler]
				(auto buffer) mutable {
					_read_data.read(buffer);
					back_stage.process(buffer);
					push_buffer_handler();
			}}
		{}

		StageType& front() final {
			return _read_file;
		}

		void stop() final {
			_read_file.stop();
		}


	private:

		shared_ptr<IVirtualCardConfig> _config;
		shared_ptr<WAVFileFactory> _wav_file_factory{
			make_shared<WAVFileFactory>()};
		ReadData _read_data;
		StageType _read_file;

	};

}