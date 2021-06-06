#pragma once

#include "IWork.h"

class NoAuth : public IWork
{

public:

	void exec(shared_ptr<IBufferView> view) final
    {
        view->set_authentication_result(true);
        if(!_next)
        {
            return;
        }
        _next(view);
    }

    void next(function<void(shared_ptr<IBufferView>)> next_) final
    {
        _next = next_;
    }

private:

    function<void(shared_ptr<IBufferView>)> _next{ nullptr };

};