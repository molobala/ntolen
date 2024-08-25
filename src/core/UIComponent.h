#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#pragma once

namespace ntolen
{

class UIComponent
{
public:
    UIComponent();
    virtual ~UIComponent();
    virtual void render(float fps) = 0;
};
    
} // namespace ntolen

#endif