
#include <memory>

#include <boost/multi_array.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

#include "async/IWSServerConfig.h"

using namespace std;

namespace WSServer
{

	class CircularBuffer {

	public:

		class View {

		public:

			View(CircularBuffer& buffer, size_t idx)
				:_buffer{ buffer },
				_idx{ idx }
			{}

			~View()
			{
				_buffer.pop_back();
			}

		private:


			CircularBuffer& _buffer;
			size_t _idx;
			
		};

		CircularBuffer(shared_ptr<IWSServerConfig> config) :
			_config{ config },
			_buffer_size{ config->get_buffer_size() }
		{
		}

		shared_ptr<View> next_view()
		{
			boost::mutex::scoped_lock lock(_mutex);
			_not_full.wait(lock, bind(&CircularBuffer::is_not_full, this));
			if (_buff_idx >= _buffer_size) {
				_buff_idx = 0;
			}
			++_unread;
			lock.unlock();
			_not_empty.notify_one();
			return make_shared<View>(*this, _buff_idx++);
		}

		void pop_back()
		{
			boost::mutex::scoped_lock lock(_mutex);
			_not_empty.wait(lock, bind(&CircularBuffer::is_not_empty, this));
			--_unread;
			lock.unlock();
			_not_full.notify_one();
		}

	private:

		bool is_not_empty() const
		{
			return _unread > 0;
		}

		bool is_not_full() const
		{
			return _unread < _buffer_size;
		}

		shared_ptr<IWSServerConfig> _config;
		size_t _buffer_size;
		boost::mutex _mutex;
		boost::condition _not_empty;
		boost::condition _not_full;

	};

}
