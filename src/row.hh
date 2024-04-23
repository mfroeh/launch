#pragma once

#include "gtkmm/box.h"
#include "gtkmm/label.h"
#include <gtkmm/listboxrow.h>
#include <filesystem>

class MyRow : public Gtk::ListBoxRow {
public:
  MyRow(const std::filesystem::path& path);
  std::filesystem::path path;

protected:
  Gtk::Box hbox;
  Gtk::Label label;
};
