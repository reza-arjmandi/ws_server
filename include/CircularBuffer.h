
#include <memory>

#include <boost/multi_array.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

#include "IBuffer.h"
#include "IBufferView.h"
#include "IBufferConfig.h"

using namespace std;

class CircularBuffer : public IBuffer 
{

public:

	class View : public IBufferView 
	{

	public:

		View(CircularBuffer& buffer, size_t idx)
			:_buffer{buffer},
			_idx{ idx },
			_socket{buffer.get_ioc_list()[idx]}
		{}

		~View()
		{
			_buffer.pop_back();
		}

    	bool is_authenticated() const final
		{
			return true;
		}

    	tcp_socket get_socket() final
		{
			// return std::move(_socket);
		}

	    io_context& get_ioc() final
		{
		}

    	void set_socket(tcp_socket&& socket) final
		{
			// _socket = std::move(socket);
		}

	private:

		CircularBuffer& _buffer;
		size_t _idx;
		tcp_socket _socket;
			
	};

	CircularBuffer(shared_ptr<IBufferConfig> config) :
		_config{ config },
		_buffer_size{ config->get_buffer_size() }
	{
	}

	shared_ptr<IBufferView> next_view()
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

	vector<io_context>& get_ioc_list() final 
	{

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

	size_t _unread{0};
	size_t _buff_idx{0};
	shared_ptr<IBufferConfig> _config;
	size_t _buffer_size;
	boost::mutex _mutex;
	boost::condition _not_empty;
	boost::condition _not_full;

};

