#pragma once

using namespace std;

class IBufferConfig
{
    
public:

    virtual ~IBufferConfig() = default;

    virtual size_t get_buffer_size() const = 0;
    virtual void set_buffer_size(size_t) = 0;

};