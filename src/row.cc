#include "row.hh"

MyRow::MyRow(const std::filesystem::path& path) : path(path) {
  label.set_label(path.filename().c_str());
  hbox.set_orientation(Gtk::Orientation::HORIZONTAL);
  hbox.append(label);
  set_child(hbox);
}
