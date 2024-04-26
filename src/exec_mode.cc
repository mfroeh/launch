#include "exec_mode.hh"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <ranges>
#include <unistd.h>
#include <unordered_set>

using namespace std;
using filesystem::directory_entry;
using filesystem::path;

ExecMode::ExecMode() {}

std::vector<Item<ExecData>> ExecMode::getItems() {
  string PATH = getenv("PATH");
  stringstream ss(PATH);
  string dirName;

  unordered_set<path> dirs;
  while (getline(ss, dirName, ':')) {
    auto path = filesystem::path(dirName);
    if (filesystem::exists(path) && filesystem::is_directory(path))
      dirs.insert(path);
  }

  auto getPath = [](const directory_entry &entry) { return entry.path(); };

  auto isExecutable = [](const path &path) {
    return access(path.c_str(), X_OK) == 0;
  };

  auto makeItem = [](const path &path) {
    return Item<ExecData>{.displayName = path.filename().c_str(),
                          .data = ExecData{.path = path}};
  };

  vector<Item<ExecData>> items;
  for (const path &dir : dirs) {
    auto dirIter = filesystem::directory_iterator(dir);
    ranges::copy(dirIter | ranges::views::transform(getPath) |
                     ranges::views::filter(isExecutable) |
                     ranges::views::transform(makeItem),
                 back_inserter(items));
  }

  return items;
}

void ExecMode::onActivation(const ExecData &data) {
  pid_t child = fork();
  if (child == -1)
    cerr << strerror(errno) << endl;

  if (child == 0) {
    // inside child process
    int ret = execv(data.path.c_str(), nullptr);
    if (ret == -1)
      cerr << strerror(errno) << endl;
  }

  // parent process and fork successful
  if (child != -1)
    exit(0);
}
