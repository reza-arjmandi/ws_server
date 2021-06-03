#pragma once

#include <boost/asio.hpp>

#include "Types.h"

namespace WSServer 
{
    
    using tcp = boost::asio::ip::tcp;

    class Acceptor
    {


    public:

        Acceptor(tcp::endpoint endpoint)
        {
            beast::error_code ec;
            _acceptor.open(endpoint.protocol(), ec);
            if(ec)
            {
                fail(ec, "open");
                return;
            }
            
            _acceptor.set_option(net::socket_base::reuse_address(true), ec);
            if(ec)
            {
                fail(ec, "set_option");
                return;
            }

            _acceptor.bind(endpoint, ec);
            if(ec)
            {
                fail(ec, "bind");
                return;
            }

            _acceptor.listen(
                net::socket_base::max_listen_connections, ec);
            if(ec)
            {
                fail(ec, "listen");
                return;
            }
        }

        void accept(BufferViewType buffer)
        {
            _acceptor.accept(buffer.socket());
        }

    private:

        tcp::acceptor _acceptor;

    };

}
