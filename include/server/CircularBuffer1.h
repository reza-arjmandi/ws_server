#pragma once

#include <memory>

#include <boost/multi_array.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

#include "offline_mode/IVirtualCardConfig.h"
#include "offline_mode/virtual_ddc_pipeline/BufferTypes.h"

namespace VirtualDDCPipeline {

	using namespace std;
	using namespace VirtualUtils;

	class CircularBuffer {

	public:

		class View {

		public:

			using raw_arr_type = short;


			View(CircularBuffer& buffer, size_t idx)
				:_buffer{ buffer },
				_idx{ idx }
			{}

			~View()
			{
				_buffer.pop_back();
			}

			void* data()
			{
				return result().first;
			}

			size_t size()
			{
				return result().second;
			}

			short_view_type raw_arr(size_t antenna_idx) const
			{
				auto raw_arr_view{
					_buffer.raw_arr()[
						_indices[_idx][antenna_idx][range()]] };
				return raw_arr_view;
			}

		private:

			pair<void*, int> result()
			{
				auto selected_antennas{ _buffer._config->get_selected_antennas() };
				if (selected_antennas == Antennas::All)
				{
					auto raw_arr{ _buffer.raw_arr()[
						_indices[_idx][range()][range()]] };
					return make_pair(
						raw_arr.origin(), 
						raw_arr.num_elements() 
						* sizeof(short));
				}
				if (selected_antennas == Antennas::_1)
				{
					auto raw_arr{ _buffer.raw_arr()[
						_indices[_idx][0][range()]] };
					return make_pair(
						raw_arr.origin(),
						raw_arr.num_elements()
						* sizeof(raw_arr_type));
				}
				if (selected_antennas == Antennas::_2)
				{
					auto raw_arr{ _buffer.raw_arr()[
						_indices[_idx][1][range()]] };
					return make_pair(
						raw_arr.origin(),
						raw_arr.num_elements()
						* sizeof(raw_arr_type));
				}
				return make_pair(nullptr, 0);
			}

			CircularBuffer& _buffer;
			size_t _idx;
			arr_3d_type<raw_arr_type>::index_gen _indices;

		};

		CircularBuffer(shared_ptr<IVirtualCardConfig> config) :
			_config{ config },
			_buffer_size{ config->get_buffer_size() },
			_ddc_chunk_size{ config->get_ddc_chunk_size() },
			_antenna_size{ config->get_antenna_size() },
			_arr{ boost::extents[_buffer_size][_antenna_size][_ddc_chunk_size]}
		{
		}

		size_t size()
		{
			return _buffer_size;
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

		arr_3d_ref_type<short> raw_arr() const
		{
			return _arr;
		}

		size_t ddc_chunk_size() const
		{
			return _ddc_chunk_size;
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

		shared_ptr<IVirtualCardConfig> _config;
		size_t _buffer_size;
		size_t _ddc_chunk_size;
		size_t _antenna_size;
		size_t _buff_idx{ 0 };
		size_t _unread{0};
		boost::mutex _mutex;
		boost::condition _not_empty;
		boost::condition _not_full;

		arr_3d_type<short> _arr;

	};

}