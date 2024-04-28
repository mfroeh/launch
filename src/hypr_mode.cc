#include "hypr_mode.hh"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

std::vector<Item<HyprData>> HyprMode::getItems() {
  array<char, 256> buffer;
  string result;

  auto pipe = unique_ptr<FILE, decltype(&pclose)>(
      popen("hyprctl -j clients", "r"), pclose);
  if (!pipe) {
    cerr << "failed to run 'hyprctl -j clients'" << endl;
    exit(1);
  }

  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    result += buffer.data();
  }

  nlohmann::json json = nlohmann::json::parse(result);

  vector<Item<HyprData>> items;
  for (auto &jsonItem : json) {
    HyprData info;
    info.title = jsonItem["title"];
    info.address = jsonItem["address"];
    info.workspace.id = jsonItem["workspace"]["id"];

    auto item = Item<HyprData>{.displayName = info.title, .data = info};
    items.push_back(item);
  }

  return items;
}

void HyprMode::onActivation(const HyprData &item) {
  stringstream ss;
  ss << "hyprctl workspace " << item.workspace.id << " && "
     << "hyprctl dispatch focuswindow address:" << item.address << endl;
  if (system(ss.str().c_str()) == 0) {
    exit(0);
  }
}