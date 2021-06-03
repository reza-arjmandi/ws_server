#pragma once

#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

namespace Async 
{

    class Active {

    public:

        Active(size_t num_threads = 1)
            : _thd_pool{ num_threads } {
        }

        ~Active() {
            _thd_pool.join();
        }

        template<class MessageType>
        void send(MessageType&& m) {
            post(_thd_pool, forward<MessageType>(m));
        }

        any_io_executor get_executor() {
            return _thd_pool.get_executor();
        }

        void stop() {
            _thd_pool.stop();
            _thd_pool.join();
        }

    private:

        thread_pool _thd_pool;

    };

}
