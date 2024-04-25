#pragma once

#include <filesystem>

#include "mode.hh"

struct ExecData {
    std::filesystem::path path;
};

class ExecMode : public Mode<ExecData> {
    std::vector<Item<ExecData>> getItems() override;
    bool onActivation(const ExecData& item) override;
};