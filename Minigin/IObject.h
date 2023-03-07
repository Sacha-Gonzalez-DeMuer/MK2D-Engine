#pragma once

class IObject
{
public:
    virtual ~IObject() = default;
    virtual void Awake() {};
    virtual void Start() {};
    virtual void Update() {};
    virtual void Render() const {};
};