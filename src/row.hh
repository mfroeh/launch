#pragma once

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/listboxrow.h>
#include <filesystem>

#include "mode.hh"

template <class T>
class MyRow : public Gtk::ListBoxRow {
public:
  MyRow(const Item<T>& item);
  Item<T> item;

protected:
  Gtk::Box hbox;
  Gtk::Label label;
};

#include "row.cc"