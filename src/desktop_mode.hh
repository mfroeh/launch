#pragma once

#include "mode.hh"

#include <giomm/desktopappinfo.h>
#include <vector>

struct DesktopData {
  std::shared_ptr<Gio::DesktopAppInfo> info;
};

class DesktopMode : public Mode<DesktopData> {
  std::vector<Item<DesktopData>> getItems() override;
  void onActivation(const DesktopData &item) override;
};