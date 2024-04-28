{ lib
, stdenv
, pkg-config
, cmake
, nlohmann_json
, rapidfuzz-cpp
, argparse
, clang-tools
, boost
, catch2
, gtkmm4
, enableTests ? false
}:
stdenv.mkDerivation {
  name = "launch";

  # good source filtering is important for caching of builds.
  # It's easier when subprojects have their own distinct subfolders.
  src = lib.sourceByRegex ./. [
    "^src.*"
    "^test.*"
    "CMakeLists.txt"
  ];

  # We now list the dependencies similar to the devShell before.
  # Distinguishing between `nativeBuildInputs` (runnable on the host
  # at compile time) and normal `buildInputs` (runnable on target
  # platform at run time) is an important preparation for cross-compilation.
  nativeBuildInputs = [ cmake clang-tools pkg-config ];
  buildInputs = [ boost gtkmm4 rapidfuzz-cpp argparse nlohmann_json ];

  # Test inputs
  checkInputs = [ catch2 ];

  # Instruct the build process to run tests.
  # The generic builder script of `mkDerivation` handles all the default
  # command lines of several build systems, so it knows how to run our tests.
  doCheck = enableTests;
  cmakeFlags = lib.optional (!enableTests) "-DTESTING=off";
}
