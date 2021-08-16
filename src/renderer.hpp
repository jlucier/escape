#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLCall(x) \
    GlClearError(); \
    x; \
    GlCheckError(#x, __FILE__, __LINE__);

void GlClearError();
void GlCheckError(const char* func, const char* file, int line);
