# mkn.gl

**OpenGL aggregate: single include / library paths for GL, GLU, GLEW, GLFW, GLM and GLI**

Headers for every library are in [inc](inc), the `all` profile compiles GLFW and GLEW into one
library. C++ helpers are in [inc/mkn/gl](inc/mkn/gl), all included by `mkn/gl.hpp`:

| header | |
|---|---|
| `mkn/gl/fw.hpp` | `fw::init()`, GLFW |
| `mkn/gl/fw/window.hpp` | `fw::create_window(WindowParams)`, size, title, GL version, profile, visibility, vsync |
| `mkn/gl/ew.hpp` | `ew::init()`, GLEW for the current context |
| `mkn/gl/shaders.hpp` | compile and link shaders from sources or files, errors throw with the GL log |
| `mkn/gl/sl/program.hpp` | `sl::Program`, an owned shader program |
| `mkn/gl/dds.hpp` | `dds::load`, DXT1/3/5 DDS textures |

```cpp
#include "mkn/gl.hpp"

mkn::gl::fw::init();
auto* window = mkn::gl::fw::create_window({.width = 800, .height = 600, .title = "demo"});
mkn::gl::ew::init();
auto program = mkn::gl::sl::Program::from_source(vertex_source, fragment_source);
```

## Example

[ex/cube.cpp](ex/cube.cpp) draws a cube with a first person camera, [ex/view.hpp](ex/view.hpp):
mouse looks, WASD or arrows move, Escape quits.

```bash
mkn clean build run -dtOp example -a -fPIC -j '{"cpp": "-std=c++20"}'
```

`-j` keeps `-std=c++20` off the C sources of GLFW and GLEW.

## Usage

In a project `mkn.yaml`:

```yaml
dep: mkn.gl
```

Build:

```bash
mkn clean build -dtOa -fPIC -p all
```

## Updating the bundled headers

[mkn.sh](mkn.sh) fetches the upstream libraries into `dep/` via the `dep_get` profile and copies
their headers into `inc/` when `inc/GL` does not exist.

## Dependencies

[mkn.kul](https://github.com/mkn/mkn.kul), system `libGL`
