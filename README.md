# mkn.gl

**OpenGL aggregate: single include / library paths for GL, GLU, GLEW, GLFW, GLM and GLI**

Headers for every library are in [inc](inc), the `all` profile compiles GLFW and GLEW into one
library. C++ helpers for windows, shaders, input and views are in [inc/mkn/gl](inc/mkn/gl).

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
