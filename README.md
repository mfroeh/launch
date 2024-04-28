* Usage: `launch {desktop, exec, window} (default: desktop)`
* Use `nix build` or `nix run` to build/run application
* During development don't create nix derivation but just
    1. `nix develop`
    2. `mkdir build && cd build && cmake .. && make && ./launch`
    3. `cd .. && ln -s build/compile_commands.json .`
    4. Rebuild whenever you add a dependency to `CMakeLists.txt` to update `compile_commands.json`
