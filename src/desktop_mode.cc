#include "desktop_mode.hh"
#include "giomm/asyncresult.h"
#include "giomm/desktopappinfo.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <unistd.h>
#include <unordered_set>

using namespace std;
using filesystem::directory_entry;
using filesystem::path;

std::vector<Item<DesktopData>> DesktopMode::getItems() {
  string DATA_DIRS = getenv("XDG_DATA_DIRS");
  stringstream ss(DATA_DIRS);
  string dirName;

  unordered_set<path> dirs;
  while (getline(ss, dirName, ':')) {
    auto path = filesystem::path(dirName);
    if (filesystem::exists(path) && filesystem::is_directory(path))
      dirs.insert(path);
  }

  auto getPath = [](const directory_entry &entry) { return entry.path(); };

  auto isDesktopFile = [](const path &path) {
    return path.has_extension() &&
           path.extension().c_str() == string(".desktop");
  };

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
  for (const path &dir : dirs) {
    auto dirIter = filesystem::recursive_directory_iterator(dir);
    ranges::copy(dirIter | ranges::views::transform(getPath) |
                     ranges::views::filter(isDesktopFile) |
                     ranges::views::transform(getInfo) |
                     ranges::views::filter(infoValid) |
                     ranges::views::transform(makeItem),
                 back_inserter(items));
  }

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
