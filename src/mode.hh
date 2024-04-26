#pragma once

#include "giomm/icon.h"
#include <optional>
#include <string>
#include <vector>

template <class T> struct Item {
  std::string displayName;
  std::shared_ptr<Gio::Icon> icon;
  T data;
};

template <class T> class Mode {
public:
  virtual ~Mode() = default;
  virtual std::vector<Item<T>> getItems() = 0;
  virtual void onActivation(const T &data) = 0;
};