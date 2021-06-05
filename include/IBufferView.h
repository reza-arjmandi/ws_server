#pragma once

#include <boost/asio.hpp>

class IBufferView
{
    
public:

    using tcp_socket = boost::asio::ip::tcp::socket; 
    using io_context = boost::asio::io_context; 

    virtual ~IBufferView() = default;
    
    virtual bool is_authenticated() const = 0;
    virtual tcp_socket get_socket() = 0;
    virtual void set_socket(tcp_socket&& socket) = 0;
    virtual io_context& get_ioc() = 0;

};
