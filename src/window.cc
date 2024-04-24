#include "window.hh"
#include "gdkmm/display.h"
#include "gtkmm/enums.h"
#include "gtkmm/label.h"
#include "gtkmm/listboxrow.h"
#include "gtkmm/object.h"
#include "gtkmm/scrolledwindow.h"
#include "gtkmm/eventcontrollerkey.h"
#include <gtkmm.h>
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

SearchWindow::SearchWindow() : executables(getExecutableFilesOnPath())
{
  // avoid window manager resizing
  set_resizable(false);
  set_default_size(360, 720);

  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(sigc::mem_fun(*this, &SearchWindow::onKeyPressed), false);
  add_controller(controller);

  entry.signal_changed().connect(sigc::mem_fun(*this, &SearchWindow::onTextChanged));
  entry.signal_activate().connect(sigc::mem_fun(*this, &SearchWindow::onEntryActivate));

  options.signal_row_selected().connect(sigc::mem_fun(*this, &SearchWindow::rowSelected));

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

  for (const path &path : executables)
  {
    auto row = Gtk::make_managed<MyRow>(path);
    options.append(*row);
  }

  options.set_selection_mode(Gtk::SelectionMode::BROWSE);
  options.select_row(*options.get_row_at_index(0));
  options.set_sort_func([this](Gtk::ListBoxRow *a, Gtk::ListBoxRow *b)
                        {
    string curText = entry.get_text();
    auto lhs = dynamic_cast<MyRow *>(a);
    auto rhs = dynamic_cast<MyRow *>(b);

    return rapidfuzz::fuzz::partial_ratio(lhs->path.c_str(), curText) <
           rapidfuzz::fuzz::partial_ratio(rhs->path.c_str(), curText); });
}

SearchWindow::~SearchWindow() {}

vector<path> getExecutableFilesOnPath()
{
  using filesystem::directory_entry;
  vector<path> dirs;

  string PATH = getenv("PATH");
  stringstream ss(PATH);
  string dirName;
  while (getline(ss, dirName, ':'))
  {
    auto path = filesystem::path(dirName);
    if (filesystem::exists(path) && filesystem::is_directory(path) &&
        ranges::find(dirs, path) == dirs.end())
    {
      dirs.push_back(path);
    }
  }

  vector<path> executables;
  for (const path &dir : dirs)
  {
    auto dirIter = filesystem::directory_iterator(dir);
    for (const directory_entry &entry : dirIter)
    {

      // if executable
      const path &path = entry.path();
      if (access(path.c_str(), X_OK) == 0)
      {
        executables.push_back(entry.path());
      }
    }
  }
  return executables;
}

void SearchWindow::onTextChanged()
{
  options.invalidate_sort();
  options.select_row(*options.get_row_at_index(0));
}

bool SearchWindow::onKeyPressed(guint keyval, guint keycode, Gdk::ModifierType state)
{
  bool isCtrlOnly = (state & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK | Gdk::ModifierType::ALT_MASK | Gdk::ModifierType::SUPER_MASK)) == Gdk::ModifierType::CONTROL_MASK;
  if (!isCtrlOnly)
    return false;

  if (keyval == GDK_KEY_j || keyval == GDK_KEY_n)
  {
    Gtk::ListBoxRow *row = options.get_selected_row();
    int rowIdx = row ? row->get_index() : -1;
    Gtk::ListBoxRow *nextRow = options.get_row_at_index(rowIdx + 1);
    options.select_row(nextRow ? *nextRow : *row);
  }
  else if (keyval == GDK_KEY_k || keyval == GDK_KEY_p)
  {
    Gtk::ListBoxRow *row = options.get_selected_row();
    int rowIdx = row ? row->get_index() : 1;
    Gtk::ListBoxRow *nextRow = options.get_row_at_index(rowIdx - 1);
    options.select_row(nextRow ? *nextRow : *row);
  }

  return true;
}

void SearchWindow::rowSelected(const Gtk::ListBoxRow *row)
{
  // TODO: Move scrollableWindow
}

void SearchWindow::onEntryActivate()
{
  
  if (auto row = dynamic_cast<MyRow*>(options.get_selected_row()); row) {
    pid_t child = fork();
    if (child == -1) cerr << strerror(errno) << endl;

    if (child == 0) {
      // inside child process
      int ret = execv(row->path.c_str(), nullptr);
      if (ret == -1) cerr << strerror(errno) << endl;
    } else if (child != -1) {
      get_application()->quit();
    }
  }
}
