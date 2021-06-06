#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

namespace beast = boost::beast;  
namespace http = beast::http; 
namespace websocket = beast::websocket;
namespace net = boost::asio; 

using tcp = net::ip::tcp;
using tcp_endpoint = tcp::endpoint;
using io_context = net::io_context;
using tcp_acceptor = tcp::acceptor;
using reuse_address = net::socket_base::reuse_address;
using tcp_socket = tcp::socket; 






