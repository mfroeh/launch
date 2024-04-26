#include "exec_mode.hh"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <ranges>
#include <unistd.h>

using namespace std;
using filesystem::directory_entry;
using filesystem::path;

std::vector<Item<ExecData>> ExecMode::getItems() {
  vector<path> dirs;

  string PATH = getenv("PATH");
  stringstream ss(PATH);
  string dirName;
  while (getline(ss, dirName, ':')) {
    auto path = filesystem::path(dirName);
    if (filesystem::exists(path) && filesystem::is_directory(path) &&
        ranges::find(dirs, path) == dirs.end()) {
      dirs.push_back(path);
    }
  }

  vector<path> executables;
  for (const path &dir : dirs) {
    auto dirIter = filesystem::directory_iterator(dir);
    for (const directory_entry &entry : dirIter) {
      // if executable
      const path &path = entry.path();
      if (access(path.c_str(), X_OK) == 0) {
        executables.push_back(entry.path());
      }
    }
  }

  vector<Item<ExecData>> items;
  ranges::transform(executables, back_inserter(items), [](const path &path) {
    return Item<ExecData>{.displayName = path.filename().c_str(),
                          .data = ExecData{.path = path}};
  });
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
