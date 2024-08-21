#!/usr/bin/env bash
set -exu
CWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" && cd $CWD

GLEW_VER=$(mkn -G gl.glew.version -C dep/glew/mkn.prop.yaml)

do_incs() {
    mkdir -p inc/GL
    cp -r dep/gl/inc/GL/* inc/GL/
    cp -r dep/glew/glew-${GLEW_VER}/include/GL/* inc/GL/
    cp -r dep/glu/g/include/GL/* inc/GL
    cp -r dep/glfw/g/include/* inc/
    cp -r dep/glm/g/glm inc/
}

[ ! -d "inc" ] && do_incs
