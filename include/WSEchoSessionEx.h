#pragma once

#include "IWork.h"
#include "WSEchoSession.h"

class WSEchoSessionEx : public IWork
{

public:

    void exec(shared_ptr<IBufferView> view) final
    {
        make_shared<WSEchoSession>(view->get_socket())->run();
    }

    void next(function<void(shared_ptr<IBufferView>)>) final
    {
    }

private:

};