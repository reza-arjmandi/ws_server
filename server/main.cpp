#include <iostream>

#include "TCPServer.h"
#include "TCPServerBuilder.h"
#include "WSEchoSessionEx.h"
#include "WSVisSessionEx.h"
#include "NoAuth.h"

int main()
{
    auto server{ 
        TCPServerBuilder()
            .address("127.0.0.1")
            .port(1111)
            .session_executor(make_shared<WSVisSessionEx>())
            .authenticator(make_shared<NoAuth>())
            .build()
    };

    server->run();

    cin.get();

    return 0;
}