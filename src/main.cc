#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/label.h>
#include "window.hh"
#include "exec_mode.hh"

#include <argparse/argparse.hpp>

using namespace std;

int main(int argc, char *argv[])
{
  auto program = argparse::ArgumentParser("launch");
  program.add_argument("mode")
    .default_value("exec")
    .choices("exec");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& err) {
    cerr << err.what() << endl;
    cerr << program;
    return 1;
  }

  string mode = program.get("mode");
  cout << "Mode: " << mode << endl;

  auto app = Gtk::Application::create();
  if (mode == "exec") {
    return app->make_window_and_run<SearchWindow<ExecData>>(0, nullptr, make_unique<ExecMode>());
  }

  return 1;
}
