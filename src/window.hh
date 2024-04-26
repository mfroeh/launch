#pragma once

#include "gtkmm/box.h"
#include "gtkmm/listbox.h"
#include "gtkmm/scrolledwindow.h"
#include <gtkmm/entry.h>
#include <memory>
#include <gtkmm/applicationwindow.h>
#include "mode.hh"

template <class T>
class SearchWindow : public Gtk::ApplicationWindow {
public:
  SearchWindow(std::unique_ptr<Mode<T>> mode);
  virtual ~SearchWindow();

protected:
    Gtk::Box box;
    Gtk::Entry entry;

    Gtk::ScrolledWindow optionsWindow;
    Gtk::ListBox options;

    std::unique_ptr<Mode<T>> mode;

    void onTextChanged();

    bool onKeyPressed(guint keyval, guint keycode, Gdk::ModifierType state);

    void rowActivated(const Gtk::ListBoxRow* row);
    void rowSelected(const Gtk::ListBoxRow* row);

    void onEntryActivated();
};

#include "window.cc"
