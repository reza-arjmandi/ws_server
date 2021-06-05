#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ITCPServerDependencies.h"

class MockConfig : public ITCPServerDependencies
{

public:

    MOCK_METHOD(void, set_endpoint, (tcp_endpoint));
    MOCK_METHOD(tcp_endpoint, get_endpoint, (), (const));
    MOCK_METHOD(IBufferFactory&, get_buffer_factory, ());
    MOCK_METHOD(IPipelineFactory&, get_pipeline_factory, ());

};