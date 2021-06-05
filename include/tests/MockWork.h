#pragma once

#include <memory>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IWork.h"

using namespace std;

class MockWork : public IWork
{

public:

	MOCK_METHOD(void, exec, (shared_ptr<IBufferView> view));
    MOCK_METHOD(void, next, (function<void(shared_ptr<IBufferView>)>));

};