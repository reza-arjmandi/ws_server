#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "IBufferView.h"

class MockBufferView : public IBufferView
{

public:

    MOCK_METHOD(bool, is_authenticated, (), (const));
    MOCK_METHOD(tcp_socket&, get_socket, ());
    MOCK_METHOD(Session&, get_session, ());

};