#pragma once

#include <vector>
#include <string>

template <class T>
struct Item {
    std::string displayName;
    T data;
};

template <class T>
class Mode
{
public:
    virtual ~Mode() = default;
    virtual std::vector<Item<T>> getItems() = 0;
    virtual bool onActivation(const T& data) = 0;
};