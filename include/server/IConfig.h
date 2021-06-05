#pragma once

#include <cstddef>

namespace Server
{

    using namespace std;

    class IWSServerConfig
    {
        public:

        virtual size_t get_buffer_size() const = 0;
        
    };

}

