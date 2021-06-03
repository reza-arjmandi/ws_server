#pragma once

namespace Async
{

	template<class BufferType>
	class IBufferFactory {

	public:

		virtual ~IBufferFactory() = default;

		virtual BufferType create() const = 0;

	};

}
