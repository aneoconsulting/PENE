#pragma once

struct module
{
    virtual void Init() = 0;
    virtual void End() = 0;
};