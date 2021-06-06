#pragma once 

#include <memory>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "Active.h"
#include "Types.h"
#include "VisDataProvider.h"

class WSVisSession : public std::enable_shared_from_this<WSVisSession>
{

public:
    // Take ownership of the socket
    explicit WSVisSession(tcp::socket&& socket)
    : ws_(std::move(socket))
    {
    }

    void run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this WSVisSession. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(ws_.get_executor(),
            beast::bind_front_handler(
                &WSVisSession::on_run,
                shared_from_this()));
        _active.send([&](){
            auto& ex{ static_cast<io_context&>(ws_.get_executor().context()) };
            ex.run();
        });
    }

    // Start the asynchronous operation
    void on_run()
    {
        // Set suggested timeout settings for the websocket
        ws_.set_option(
            websocket::stream_base::timeout::suggested(
                beast::role_type::server));
        // Set a decorator to change the Server of the handshake
        ws_.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res)
            {
                res.set(http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-server-async");
            }));
        // Accept the websocket handshake
        ws_.async_accept(
            beast::bind_front_handler(
                &WSVisSession::on_accept,
                shared_from_this()));
    }

    void on_accept(beast::error_code ec)
    {
        if(ec)
            return fail(ec, "accept");
        // Read a message
        do_read();
    }

    void do_read()
    {
        // Read a message into our buffer
        ws_.async_read(
            buffer_,
            beast::bind_front_handler(
                &WSVisSession::on_read,
                shared_from_this()));
    }

    void on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        // This indicates that the WSVisSession was closed
        if(ec == websocket::error::closed)
            return;
        if(ec)
            fail(ec, "read");
        // Echo the message
        ws_.text(ws_.got_text());
        auto result = _data_proider.provide(
            beast::buffers_to_string(buffer_.data()));
        
        ws_.async_write(
            net::buffer(result),
            beast::bind_front_handler(
                &WSVisSession::on_write,
                shared_from_this()));
    }

    void on_write(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        if(ec)
            return fail(ec, "write");
        // Clear the buffer
        buffer_.consume(buffer_.size());
        // Do another read
        do_read();
    }

    void
    fail(beast::error_code ec, char const* what)
    {
        std::cerr << what << ": " << ec.message() << "\n";
    }

private:

    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    VisDataProvider _data_proider;
    Active _active;

};
