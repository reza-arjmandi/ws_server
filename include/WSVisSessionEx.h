#pragma once

#include "IWork.h"
#include "WSVisSession.h"

class WSVisSessionEx : public IWork
{

public:

    void exec(shared_ptr<IBufferView> view) final
    {
        make_shared<WSVisSession>(view->get_socket())->run();
    }

    void next(function<void(shared_ptr<IBufferView>)>) final
    {
    }

private:

};