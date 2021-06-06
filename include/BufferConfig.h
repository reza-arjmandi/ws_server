#pragma once

#include "IBufferConfig.h"

using namespace std;

class BufferConfig : public IBufferConfig
{
    
public:

    size_t get_buffer_size() const final
    {
        return 20;
    }

    void set_buffer_size(size_t size) final
    {
        _size = size;
    }

private:

    size_t _size{ 5 };

};