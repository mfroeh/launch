#include "window.hh"
#include "gdkmm/display.h"
#include "gtkmm/enums.h"
#include "gtkmm/label.h"
#include "gtkmm/listboxrow.h"
#include "gtkmm/object.h"
#include "gtkmm/scrolledwindow.h"
#include "row.hh"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <rapidfuzz/fuzz.hpp>
#include <sigc++/functors/mem_fun.h>
#include <sstream>
#include <string>
#include <unistd.h>

using namespace std;
using filesystem::path;

vector<path> getExecutableFilesOnPath();

SearchWindow::SearchWindow() : executables(getExecutableFilesOnPath()) {
  entry.signal_changed().connect(
      sigc::mem_fun(*this, &SearchWindow::onTextChanged));
  // avoid window manager resizing
  set_resizable(false);
  set_default_size(360, 720);

  box.set_orientation(Gtk::Orientation::VERTICAL);
  box.set_valign(Gtk::Align::FILL);
  set_child(box);
  box.append(entry);

  auto optionsWindow = Gtk::ScrolledWindow();
  optionsWindow.set_child(options);

  int width, height;
  get_default_size(width, height);
  optionsWindow.set_min_content_height(height - entry.get_height());
  optionsWindow.set_min_content_width(width);
  box.append(optionsWindow);

  for (const path &path : executables) {
    auto row = Gtk::make_managed<MyRow>(path);
    options.append(*row);
  }

  options.set_sort_func([this](Gtk::ListBoxRow *a, Gtk::ListBoxRow *b) {
    string curText = entry.get_text();
    auto lhs = dynamic_cast<MyRow *>(a);
    auto rhs = dynamic_cast<MyRow *>(b);

    return rapidfuzz::fuzz::partial_ratio(lhs->path.c_str(), curText) <
           rapidfuzz::fuzz::partial_ratio(rhs->path.c_str(), curText);
  });
}

SearchWindow::~SearchWindow() {}

vector<path> getExecutableFilesOnPath() {
  using filesystem::directory_entry;
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
  return executables;
}

void SearchWindow::onTextChanged() {
  options.invalidate_sort();
}
