#pragma once

#include "mode.hh"
#include <nlohmann/json.hpp>

struct HyprlandWorkspaceInfo {
  int64_t id;
  std::string name;
};

struct HyprData {
  std::string address;
  bool mapped;
  bool hidden;
  std::vector<int64_t> at;
  std::vector<int64_t> size;
  HyprlandWorkspaceInfo workspace;
  bool floating;
  int64_t monitor;
  std::string welcomeClass;
  std::string title;
  std::string initialClass;
  std::string initialTitle;
  int64_t pid;
  bool xwayland;
  bool pinned;
  bool fullscreen;
  int64_t fullscreenMode;
  bool fakeFullscreen;
  std::vector<nlohmann::json> grouped;
  std::string swallowing;
  int64_t focusHistoryId;
};

class HyprMode : public Mode<HyprData> {
public:
  HyprMode() = default;
  std::vector<Item<HyprData>> getItems() override;
  void onActivation(const HyprData &item) override;
};