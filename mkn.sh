#!/usr/bin/env bash
set -exu
CWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" && cd $CWD

do_incs() {
    mkn -p dep_get
    mkdir -p inc/GL
    cp -r dep/gl/inc/GL/* inc/GL/
    cp -r dep/glew/g/include/GL/* inc/GL/
    cp -r dep/glu/g/include/GL/* inc/GL
    cp -r dep/glfw/g/include/* inc/
    cp -r dep/glm/g/glm inc/
}

[ ! -d "inc" ] && do_incs
