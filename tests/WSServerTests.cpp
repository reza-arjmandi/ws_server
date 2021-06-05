#include <string>
#include <memory>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "TCPServer.h"

#include "tests/MockConfig.h"
#include "tests/MockBufferFactory.h"
#include "tests/MockPipelineFactory.h"
#include "tests/MockPipeline.h"
#include "tests/MockBuffer.h"
#include "tests/MockBufferView.h"
#include "tests/MockStage.h"

using namespace testing;

class WSServerTests : public Test
{

public:

    void SetUp() final
    {
        EXPECT_CALL(*_config, get_buffer_factory()).WillOnce(
            ReturnRef(_buffer_factory));
        EXPECT_CALL(_buffer_factory, create()).WillOnce(
            Return(_buffer));

        EXPECT_CALL(*_config, get_pipeline_factory()).WillOnce(
            ReturnRef(_pipeline_factory));
        EXPECT_CALL(_pipeline_factory, create(_)).WillOnce(
            DoAll(SaveArg<0>(&_push_buffer), Return(_pipeline)));
    
        _server = make_unique<TCPServer>(_config);
    }

    void TearDown() final
    {
        if(_is_stopped)
        {
            return;
        }

        EXPECT_CALL(*_pipeline, stop()).Times(1);
    }

    void RUN_AND_ASSERT_SERVER_IS_RUN()
    {
        EXPECT_CALL(*_buffer, next_view()).WillOnce(
            Return(_buffer_view));
        EXPECT_CALL(*_pipeline, front()).WillOnce(
            Return(_front_stage));
        EXPECT_CALL(*_front_stage, process(Eq(_buffer_view))).Times(1);
        _server->run();
    }

    void STOP_AND_ASSERT_SERVER_IS_STOPPED()
    {
        EXPECT_CALL(*_pipeline, stop()).Times(1);
        _server->stop();
    }

    MockBufferFactory _buffer_factory;
    MockPipelineFactory _pipeline_factory;
    function<void()> _push_buffer;
    bool _is_stopped{ false };

    shared_ptr<MockStage> _front_stage{ 
        make_shared<MockStage>() };
    shared_ptr<MockConfig> _config{ 
        make_shared<MockConfig>() };
    shared_ptr<MockPipeline> _pipeline{ 
        make_shared<MockPipeline>() };
    shared_ptr<MockBuffer> _buffer{ 
        make_shared<MockBuffer>() };
    shared_ptr<MockBufferView> _buffer_view{ 
        make_shared<MockBufferView>() };
    unique_ptr<TCPServer> _server;

}; 

TEST_F(WSServerTests, ctor)
{
    _is_stopped = true;
}

TEST_F(WSServerTests, test_run)
{
    EXPECT_CALL(*_buffer, next_view()).WillOnce(
        Return(_buffer_view));
    EXPECT_CALL(*_pipeline, front()).WillOnce(
        Return(_front_stage));
    EXPECT_CALL(*_front_stage, process(Eq(_buffer_view))).Times(1);

    _server->run();
}

TEST_F(WSServerTests, test_run_when_is_called_more_than_once)
{
    RUN_AND_ASSERT_SERVER_IS_RUN();

    _server->run();
    _server->run();
}

TEST_F(WSServerTests, test_push_buffer)
{
    RUN_AND_ASSERT_SERVER_IS_RUN();

    EXPECT_CALL(*_buffer, next_view()).WillOnce(
        Return(_buffer_view));
    EXPECT_CALL(*_pipeline, front()).WillOnce(
        Return(_front_stage));
    EXPECT_CALL(*_front_stage, process(Eq(_buffer_view))).Times(1);

    _push_buffer();
}

TEST_F(WSServerTests, test_stop_when_is_not_run)
{
    _is_stopped = true;
    _server->stop();
}

TEST_F(WSServerTests, test_stop_when_is_run)
{
    RUN_AND_ASSERT_SERVER_IS_RUN();

    EXPECT_CALL(*_pipeline, stop()).Times(1);
    _server->stop();
    _is_stopped = true;
}

TEST_F(WSServerTests, test_stop_when_is_called_more_than_once)
{
    RUN_AND_ASSERT_SERVER_IS_RUN();

    EXPECT_CALL(*_pipeline, stop()).Times(1);
    _server->stop();
    _is_stopped = true;
    _server->stop();
}

TEST_F(WSServerTests, test_stop_when_push_buffer_is_called)
{
    RUN_AND_ASSERT_SERVER_IS_RUN();
    STOP_AND_ASSERT_SERVER_IS_STOPPED();
    _is_stopped = true;

    _push_buffer();
}
