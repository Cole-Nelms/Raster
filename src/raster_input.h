
#ifndef RASTER_INPUT_H
#define RASTER_INPUT_H

#include "raster_types.h"

void
raster_input_resize_cb(GLFWwindow* window, int width, int height);

void
raster_input_key_cb(GLFWwindow* window, int key, int scancode, int action, int mods);

void
raster_input_cursor_cb(GLFWwindow* window, double X, double Y);

void
raster_input_mouse_cb(GLFWwindow* window, int button, int action, int mods);

void
raster_input_scroll_cb(GLFWwindow* window, double xoffset, double yoffset);

#endif /* RASTER_INPUT_H */