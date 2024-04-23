* Use `nix build` or `nix run` to build/run application
* During development don't create nix derivation but just
    1. `nix develop`
    2. `mkdir build && cd build && cmake .. && make && ./nix-c++-template`
    3. `cd .. && ln build/compile_commands.json .`
    4. Rebuild whenever you add a dependency to `CMakeLists.txt`
