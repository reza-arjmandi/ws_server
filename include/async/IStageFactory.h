#pragma once

#include <memory>

namespace Async
{

	using namespace std;

	template<class StageType>
	class IStageFactory {

	public:

		virtual ~IStageFactory() = default;

		virtual shared_ptr<StageType> create(
			typename StageType::WorkType work) const = 0;

	};

}
