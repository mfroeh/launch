#include "desktop_mode.hh"
#include "giomm/appinfo.h"
#include "giomm/asyncresult.h"
#include "giomm/desktopappinfo.h"
#include "glibmm/spawn.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <ranges>
#include <string_view>
#include <unistd.h>
#include <unordered_map>

using namespace std;
using filesystem::directory_entry;
using filesystem::path;

std::vector<Item<DesktopData>> DesktopMode::getItems() {
  vector<path> dirs;

  string PATH = getenv("XDG_DATA_DIRS");
  stringstream ss(PATH);
  string dirName;
  while (getline(ss, dirName, ':')) {
    auto path = filesystem::path(dirName);
    if (filesystem::exists(path) && filesystem::is_directory(path) &&
        ranges::find(dirs, path) == dirs.end()) {
      dirs.push_back(path);
    }
  }

  // these are at $XDG_DATA_DIRS/icons
  vector<path> iconDirs;

  auto getPath = [](const directory_entry &entry) { return entry.path(); };

  auto isDesktopFile = [](const path &path) {
    return path.has_extension() &&
           path.extension().c_str() == string(".desktop");
  };

  vector<path> desktopFiles;
  for (const path &dir : dirs) {
    auto dirIter = filesystem::recursive_directory_iterator(dir);
    ranges::copy(dirIter | ranges::views::transform(getPath) |
                     ranges::views::filter(isDesktopFile),
                 back_inserter(desktopFiles));
  }

  auto getInfo = [](const path &path) {
    auto ifs = ifstream(path.c_str());
    shared_ptr<Gio::DesktopAppInfo> info =
        Gio::DesktopAppInfo::create_from_filename(path.c_str());
    return info;
  };

  auto infoValid = [](const shared_ptr<Gio::DesktopAppInfo> &info) {
    return info != nullptr;
  };

  auto makeItem = [](const shared_ptr<Gio::DesktopAppInfo> &info) {
    return Item<DesktopData>{.displayName = info->get_display_name(),
                             .icon = info->get_icon(),
                             .data = DesktopData{.info = info}};
  };

  vector<Item<DesktopData>> items;
  ranges::copy(desktopFiles | ranges::views::transform(getInfo) |
                   ranges::views::filter(infoValid) |
                   ranges::views::transform(makeItem),
               back_inserter(items));
  cout << "DONE" << endl;
  return items;
}

void DesktopMode::onActivation(const DesktopData &data) {
  shared_ptr<Gio::DesktopAppInfo> info = data.info;
  if (info->get_boolean("DBusActivatable")) {
    auto on_launch_finished =
        [info](const Glib::RefPtr<Gio::AsyncResult> &result) {
          try {
            if (info->launch_uris_finish(result)) {
              exit(0);
            } else {
              cerr << info->get_display_name() << ": failed to launch" << endl;
            };
          } catch (const Glib::Error &err) {
            cerr << err.what() << endl;
          }
        };

    info->launch_uris_async({}, {}, on_launch_finished);
  } else {
    if (data.info->launch_uris({})) {
      exit(0);
    } else {
      cerr << info->get_display_name() << ": failed to launch" << endl;
    }
  }
}
