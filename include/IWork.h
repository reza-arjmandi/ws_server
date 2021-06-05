#pragma once

#include <memory>
#include <functional>

#include "IBufferView.h"

using namespace std;

class IWork
{

public:

	virtual ~IWork() = default;

	virtual void exec(shared_ptr<IBufferView> view) = 0;
    virtual void next(function<void(shared_ptr<IBufferView>)>) = 0;

};
