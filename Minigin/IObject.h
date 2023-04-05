#pragma once

class IObject
{
public:
    virtual void Start() {};
    virtual void Update() {};
    virtual void Render() const {};
};