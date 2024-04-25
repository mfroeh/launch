#include "window.hh"
#include <gdkmm/display.h>
#include <gtkmm/enums.h>
#include "gtkmm/listboxrow.h"
#include "gtkmm/object.h"
#include "gtkmm/scrolledwindow.h"
#include "gtkmm/eventcontrollerkey.h"
#include <gtkmm.h>
#include "row.hh"

#include <algorithm>
#include <rapidfuzz/fuzz.hpp>
#include <sigc++/functors/mem_fun.h>
#include <string>
#include <unistd.h>

using namespace std;

template <class T>
SearchWindow<T>::SearchWindow(unique_ptr<Mode<T>> mode) : mode(std::move(mode))
{
  // avoid window manager resizing
  set_resizable(false);
  set_default_size(360, 720);

  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(sigc::mem_fun(*this, &SearchWindow::onKeyPressed), false);
  add_controller(controller);

  entry.signal_changed().connect(sigc::mem_fun(*this, &SearchWindow::onTextChanged));
  entry.signal_activate().connect(sigc::mem_fun(*this, &SearchWindow::onEntryActivated));

  options.signal_row_selected().connect(sigc::mem_fun(*this, &SearchWindow::rowSelected));
  options.signal_row_activated().connect(sigc::mem_fun(*this, &SearchWindow::rowActivated));

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

  for (const auto &item : this->mode->getItems())
  {
    auto row = Gtk::make_managed<MyRow<T>>(item);
    options.append(*row);
  }

  options.set_selection_mode(Gtk::SelectionMode::BROWSE);
  options.select_row(*options.get_row_at_index(0));
  options.set_sort_func([this](Gtk::ListBoxRow *a, Gtk::ListBoxRow *b) {
    string curText = entry.get_text();
    auto const& lhs = dynamic_cast<MyRow<T> *>(a)->item;
    auto const& rhs = dynamic_cast<MyRow<T> *>(b)->item;

    // TODO: make this depend on item directly and just do lhs < rhs;
    return rapidfuzz::fuzz::partial_ratio(lhs.displayName, curText) <
           rapidfuzz::fuzz::partial_ratio(rhs.displayName, curText); });
}

template <class T>
SearchWindow<T>::~SearchWindow() {}

template <class T>
void SearchWindow<T>::onTextChanged()
{
  options.invalidate_sort();
  options.select_row(*options.get_row_at_index(0));
}

template <class T>
bool SearchWindow<T>::onKeyPressed(guint keyval, guint keycode, Gdk::ModifierType state)
{
  bool isCtrlOnly = (state & (Gdk::ModifierType::SHIFT_MASK | Gdk::ModifierType::CONTROL_MASK | Gdk::ModifierType::ALT_MASK | Gdk::ModifierType::SUPER_MASK)) == Gdk::ModifierType::CONTROL_MASK;
  if (isCtrlOnly && (keyval == GDK_KEY_j || keyval == GDK_KEY_n))
  {
    Gtk::ListBoxRow *row = options.get_selected_row();
    int rowIdx = row ? row->get_index() : -1;
    Gtk::ListBoxRow *nextRow = options.get_row_at_index(rowIdx + 1);
    options.select_row(nextRow ? *nextRow : *row);
  }
  else if (isCtrlOnly && (keyval == GDK_KEY_k || keyval == GDK_KEY_p))
  {
    Gtk::ListBoxRow *row = options.get_selected_row();
    int rowIdx = row ? row->get_index() : 1;
    Gtk::ListBoxRow *nextRow = options.get_row_at_index(rowIdx - 1);
    options.select_row(nextRow ? *nextRow : *row);
  } else if (keyval == GDK_KEY_Return) {
    onEntryActivated();
  }

  return true;
}

template <class T>
void SearchWindow<T>::rowActivated(const Gtk::ListBoxRow *row)
{
  if (mode->onActivation(dynamic_cast<const MyRow<T>*>(row)->item.data))
    get_application()->quit();
}

template <class T>
void SearchWindow<T>::rowSelected(const Gtk::ListBoxRow *row)
{
  // TODO: Move scrollableWindow
}

template <class T>
void SearchWindow<T>::onEntryActivated()
{
  auto row = dynamic_cast<MyRow<T> *>(options.get_selected_row());
  if (mode->onActivation(row->item.data))
    get_application()->quit();
}
