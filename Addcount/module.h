#pragma once

struct module
{
    virtual void init() = 0;
    virtual void end() = 0;
};