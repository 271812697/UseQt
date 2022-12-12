#include "../pch.h"
#include "path.h"

namespace utils::paths {

    std::filesystem::path solution;

    std::string root, source, resource;
    std::string font, model, screenshot, shader, texture;

    /* current working directory could either be the vs2019 project folder or the target
       folders that contain the executables, thus it can vary depending on how and where
       you are running it from: open a debug exe? a release exe? a win32 exe? a x64 exe?
       or from within the visual studio editor? what if users move the exes to the root?
       to let our app start up normally without relying on the current working directory
       it's best to recursively search the parent path upwards until we hit the root and
       then count from there. As long as we are somewhere inside the project's directory
       tree, this is guaranteed to work.
    */

    void SearchPaths() {

    }

}
