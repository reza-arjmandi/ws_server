#pragma once

#include <iostream>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include "IWork.h"

using namespace std;

class Acceptor : public IWork
{

    using tcp_endpoint = boost::asio::ip::tcp::endpoint;
    using io_context = boost::asio::io_context;
    using tcp_acceptor = boost::asio::ip::tcp::acceptor;
    using error_code = boost::beast::error_code;
    using reuse_address = boost::asio::socket_base::reuse_address;
    int max_listen_connections = boost::asio::socket_base::max_listen_connections;

public:

    Acceptor(tcp_endpoint endpoint)
    {
        error_code ec;
        _acceptor.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }
        
        _acceptor.set_option(reuse_address(true), ec);
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
            max_listen_connections, ec);
        
        if(ec)
        {
            fail(ec, "listen");
            return;
        }
    }
    
    void exec(shared_ptr<IBufferView> view) final
    {
        _acceptor.accept(view->get_socket());
        if(!_next)
        {
            return;
        }
        _next(view);
    }   

    void next(function<void(shared_ptr<IBufferView>)> next_) final
    {
        _next = next_;
    }

private:

    void fail(error_code ec, char const* what)
    {
        cerr << what << ": " << ec.message() << "\n";
    }

    io_context _ioc;
    tcp_acceptor _acceptor{ _ioc };
    function<void(shared_ptr<IBufferView>)> _next{ nullptr };

};

