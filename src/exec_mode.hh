#pragma once

#include <filesystem>

#include "mode.hh"

struct ExecData {
  std::filesystem::path path;
};

class ExecMode : public Mode<ExecData> {
public:
  ExecMode();
  std::vector<Item<ExecData>> getItems() override;
  void onActivation(const ExecData &item) override;
};