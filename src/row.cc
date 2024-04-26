#include "row.hh"

template <class T> MyRow<T>::MyRow(const Item<T> &item) : item(item) {
  label.set_label(item.displayName);

  hbox.set_orientation(Gtk::Orientation::HORIZONTAL);
  hbox.set_spacing(10);
  if (item.icon) {
    image.set(item.icon);
    hbox.append(image);
  }
  hbox.append(label);

  set_child(hbox);
}
