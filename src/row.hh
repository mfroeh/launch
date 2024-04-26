#pragma once

#include <filesystem>
#include <gtkmm/box.h>
#include <gtkmm/iconview.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/listboxrow.h>

#include "mode.hh"

template <class T> class MyRow : public Gtk::ListBoxRow {
public:
  MyRow(const Item<T> &item);
  Item<T> item;

protected:
  Gtk::Box hbox;
  Gtk::Label label;
  Gtk::Image image;
};

#include "row.cc"