#pragma once

#include <memory>
#include <filesystem>
#include <functional>

#include "async/IPipeline.h"
#include "async/IStageFactory.h"
#include "async/IBufferFactory.h"

namespace Async {

	using namespace std;
	using namespace std::filesystem;
	using namespace Async;

	template<
		class S/*StageType*/, 
		class B/*BufferType*/>
	class IPipelineFactory {

	public:

		using PipelineType = IPipeline<S>;
		using StageFactoryType = IStageFactory<S>;
		using BufferFactoryType = IBufferFactory<B>;
		using StageType = S;
		using BufferType = B;
		using PostDataHandlerType = function<void(void*, size_t)>;

		virtual ~IPipelineFactory() = default;

		virtual const StageFactoryType&
			get_stage_factory() const = 0;

		virtual const BufferFactoryType&
			get_buffer_factory() const = 0;

		virtual size_t get_stage_count() const = 0;

	};

}