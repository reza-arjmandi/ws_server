#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IBufferFactory.h"

using namespace std;

class MockBufferFactory : public IBufferFactory
{
    
public:

    MOCK_METHOD(shared_ptr<IBuffer>, create, ());

};