
#include <stdbool.h>
#include "linmath.h"
#include "raster_log.h"
#include "raster_window.h"
#include "raster_input.h"

extern raster_app app;

raster_status
raster_window_create(const char* title, int width, int height)
{	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif /* __APPLE__ */

	app.window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!app.window)
	{
		raster_log("failed to create window");

		glfwTerminate();
		return RASTER_FAILED;
	}

	glfwSetFramebufferSizeCallback(app.window, raster_input_resize_cb);
	glfwSetKeyCallback(app.window, raster_input_key_cb);
	glfwSetCursorPosCallback(app.window, raster_input_cursor_cb);
	glfwSetScrollCallback(app.window, raster_input_scroll_cb);
	glfwSetMouseButtonCallback(app.window, raster_input_mouse_cb);

	glfwMakeContextCurrent(app.window);

	return RASTER_OK;
}
