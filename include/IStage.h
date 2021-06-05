#pragma once

#include <memory>

#include "IBufferView.h"

using namespace std;

class IStage
{

public:

    virtual ~IStage() = default;

	virtual void process(shared_ptr<IBufferView> buffer) = 0; 
    virtual void stop() = 0;

};