#pragma once

#include "gtkmm/box.h"
#include "gtkmm/listbox.h"
#include "gtkmm/scrolledwindow.h"
#include "gtkmm/widget.h"
#include <gtkmm/entry.h>
#include <filesystem>
#include <vector>
#include <gtkmm/applicationwindow.h>

class SearchWindow : public Gtk::ApplicationWindow {
public:
  SearchWindow();
  virtual ~SearchWindow();

protected:
    Gtk::Box box;
    Gtk::Entry entry;

    Gtk::ScrolledWindow optionsWindow;
    Gtk::ListBox options;

    std::vector<std::filesystem::path> executables;

    void onTextChanged();

    bool onKeyPressed(guint keyval, guint keycode, Gdk::ModifierType state);

    void rowSelected(const Gtk::ListBoxRow* row);

    void onEntryActivate();
};
