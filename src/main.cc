#include "desktop_mode.hh"
#include "exec_mode.hh"
#include "hypr_mode.hh"
#include "window.hh"
#include <gtkmm/application.h>
#include <gtkmm/label.h>
#include <iostream>

#include <argparse/argparse.hpp>
#include <memory>

using namespace std;

int main(int argc, char *argv[]) {
  auto program = argparse::ArgumentParser("launch");
  program.add_argument("mode").default_value("desktop").choices(
      "exec", "desktop", "window");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    cerr << err.what() << endl;
    cerr << program;
    return 1;
  }

  string mode = program.get("mode");
  cout << "Mode: " << mode << endl;

  auto app = Gtk::Application::create();
  if (mode == "exec") {
    return app->make_window_and_run<SearchWindow<ExecData>>(
        0, nullptr, make_unique<ExecMode>());
  } else if (mode == "desktop") {
    return app->make_window_and_run<SearchWindow<DesktopData>>(
        0, nullptr, make_unique<DesktopMode>());
  } else if (mode == "window") {
    return app->make_window_and_run<SearchWindow<HyprData>>(
        0, nullptr, make_unique<HyprMode>());
  }

  return 1;
}
