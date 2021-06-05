#pragma once

#include <functional>

#include "async/Stage.h"
#include "ws_server/CircularBuffer.h"

namespace Server {

	using namespace std;
	using namespace Async;

	using BufferType = shared_ptr<CircularBuffer>;
	using BufferViewType = shared_ptr<CircularBuffer::View>;
	using WorkType = function<void(BufferViewType)>;
	using StageType = Stage<WorkType>;

}