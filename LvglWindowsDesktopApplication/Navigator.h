#pragma once
#include <string>
class Navigator {
public:
    virtual bool navigate(const std::string& path) = 0;
    virtual void pop() = 0;
    virtual ~Navigator() = default;
};
