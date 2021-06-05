#pragma once

#include "IWork.h"
#include "IStageFactory.h"

class IPipelineDependencies
{
    
public:

    virtual ~IPipelineDependencies() = default;

    virtual shared_ptr<IWork> get_session_factory() const = 0;
    virtual shared_ptr<IWork> get_authenticator() const = 0;
    virtual shared_ptr<IWork> get_acceptor() const = 0;
    virtual IStageFactory& get_stage_factory() = 0;

};