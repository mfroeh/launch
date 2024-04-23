#include <boost/lexical_cast.hpp>
#include <iostream>
#include <gtkmm/application.h>
#include "helloworld.hh"

#if defined(__clang__)
#define CC "clang++"
#elif defined (__GNUC__)
#define CC "g++"
#else
#define CC "<unknown compiler>"
#endif

int main(int argc, char* argv[]) {
  std::cout << "Hello World!\n"
    << "Compiler: " << CC << " " << __VERSION__ << '\n'
    << "Boost: "
    << (BOOST_VERSION / 100000) << '.'
    << (BOOST_VERSION / 100 % 1000) << '.'
    << (BOOST_VERSION % 100) << '\n';

  auto app = Gtk::Application::create("org.gtkmm.examples.base");
  return app->make_window_and_run<HelloWorld>( argc, argv);
}
