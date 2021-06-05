#pragma once

#include "IWork.h"
#include "Session.h"

class SessionExecutor : public IWork
{

public:

    void exec(shared_ptr<IBufferView> view) final
    {
        view->get_session().run();
    }

    void next(function<void(shared_ptr<IBufferView>)>) final
    {
    }

private:

};