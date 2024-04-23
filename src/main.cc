#include <boost/lexical_cast.hpp>
#include <iostream>
#include <gtkmm/application.h>
#include "gtkmm/label.h"
#include "window.hh"

#if defined(__clang__)
#define CC "clang++"
#elif defined (__GNUC__)
#define CC "g++"
#else
#define CC "<unknown compiler>"
#endif

class MyWindow : public Gtk::ApplicationWindow {
public:
    MyWindow() {
        set_title("GTKmm4 ListBoxRow Example");
        set_default_size(300, 200);

        // Create a ListBox
        Gtk::ListBox listbox;

        // Create ListBoxRows with text
        for (int i = 1; i <= 5; ++i) {
            auto row = Gtk::ListBoxRow();

            // Create a label with text
            auto label = Gtk::Label("Item " + std::to_string(i));

            // Set the label as the content of the ListBoxRow
            row.set_child(label);

            // Add the ListBoxRow to the ListBox
            listbox.append(row);
        }

        // Add the ListBox to the window
        set_child(listbox);
    }
};

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.examples.base");
  return app->make_window_and_run<SearchWindow>( argc, argv);
}
