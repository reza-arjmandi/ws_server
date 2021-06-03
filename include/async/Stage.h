#pragma once

#include <memory>

#include "async/Active.h"

using namespace std;

namespace Async
{

	template<
		class W /*WorkType*/,
		class A /*ActiveType*/ = Active>
	class Stage {

	public:

		using WorkType = W;
		using ActiveType = A;

		Stage(WorkType work, size_t num_threads = 1)
			: _work{ work },
			_active{ num_threads }{
		}

		template<class BufferType>
		void process(BufferType&& buffer) {
			_active.send([
				this, buff{ forward<BufferType>(buffer) }]() mutable {
					_work(forward<BufferType>(buff));
				});
		}

		void stop() {
			_active.stop();
		}

	private:

		WorkType _work;
		ActiveType _active;

	};

}
