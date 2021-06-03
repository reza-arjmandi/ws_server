#pragma once

#include <cstddef>

namespace WSServer
{

    using namespace std;

    class IWSServerConfig
    {
        public:

        virtual size_t get_buffer_size() const = 0;
        
    };

}

