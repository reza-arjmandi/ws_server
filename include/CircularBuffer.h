
#include <memory>

#include <boost/multi_array.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

#include "IBuffer.h"
#include "IBufferView.h"
#include "IBufferConfig.h"
#include "Types.h"

using namespace std;

class CircularBuffer : public IBuffer 
{

public:

	class View : public IBufferView 
	{

	public:

		View(CircularBuffer& buffer, size_t idx)
			:_buffer{ buffer },
			_idx{ idx },
			_socket{ buffer.get_ioc_list()[idx] }
		{}

		~View()
		{
			_buffer.pop_back();
		}

    	void set_authentication_result(bool result) final
		{
			_auth_result = result;
		}

    	bool is_authenticated() const final
		{
			return _auth_result;
		}

    	tcp_socket get_socket() final
		{
			return std::move(_socket);
		}

		tcp_socket& get_socket_ref() final
		{
			return _socket;
		}

	private:

		CircularBuffer& _buffer;
		size_t _idx;
		tcp_socket _socket;
		bool _auth_result{ false };

	};

	CircularBuffer(shared_ptr<IBufferConfig> config) 
		: _config{ config },
		_buffer_size{ config->get_buffer_size() },
		_ioc_list{ vector<io_context>(_buffer_size) }
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
		return _ioc_list;
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

	size_t _unread{ 0 };
	size_t _buff_idx{ 0 };
	shared_ptr<IBufferConfig> _config;
	size_t _buffer_size;
	boost::mutex _mutex;
	boost::condition _not_empty;
	boost::condition _not_full;
	vector<io_context> _ioc_list;

};

