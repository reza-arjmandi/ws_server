#pragma once

namespace Async 
{

	template<class StageType>
	class IPipeline {

	public:

		virtual ~IPipeline() = default;

		virtual StageType& front() = 0;
		virtual void stop() = 0;

	};

}
