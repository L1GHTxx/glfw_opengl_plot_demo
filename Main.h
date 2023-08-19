#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "implot/implot.h"
#include "implot/implot_internal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stbimage/stb_image.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Macro to disable console on Windows
#if defined(_WIN32) && defined(APP_NO_CONSOLE)
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

const int widthResolution = 1920;
const int hieghtResolution = 1080;

using namespace std;

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

void renderPlot();
void imguiPlot();
void renderPlot_with_multi_axis();