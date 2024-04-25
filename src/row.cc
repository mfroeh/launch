#include "row.hh"

template <class T>
MyRow<T>::MyRow(const Item<T>& item) : item(item) {
  label.set_label(item.displayName);
  hbox.set_orientation(Gtk::Orientation::HORIZONTAL);
  hbox.append(label);
  set_child(hbox);
}
