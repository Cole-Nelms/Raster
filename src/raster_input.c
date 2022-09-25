
#include <stdbool.h>
#include "raster_input.h"

extern raster_app app;

inline static const float
to_radians(const float degrees)
{
	const float pi = 22.0f / 7.0f;
	return degrees * (pi / 180.0f);
}

inline static void
calculate_camera_position(void)
{
	app.camera_position[0] = (cos(to_radians(app.camera_yaw)) * cos(to_radians(app.camera_pitch))) * app.camera_distance;
	app.camera_position[1] = ((sin(to_radians(app.camera_pitch)) * app.camera_distance) * -1);
	app.camera_position[2] = (cos(to_radians(app.camera_pitch)) * -1) * sin(to_radians(app.camera_yaw)) * app.camera_distance;
}

void
raster_input_resize_cb(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
raster_input_cursor_cb(GLFWwindow* window, double X, double Y)
{
	if (app.first_cursor)
	{
		app.first_cursor = false;

		app.cursor_last_X = X;
		app.cursor_last_Y = Y;
	}

	if (!app.mouse_left)
	{
		app.invalid_offset = true;
		return;
	}

	float offset_X = X - app.cursor_last_X;
	float offset_Y = Y - app.cursor_last_Y;
	app.cursor_last_X = X;
	app.cursor_last_Y = Y;

	const float sensitivity = 0.25f;

	if (app.invalid_offset)
	{	
		offset_X = 0;
		offset_Y = 0;
	}
	else
	{
		offset_X *= sensitivity;
		offset_Y *= sensitivity;
	}

	app.camera_yaw	 += offset_X;
	app.camera_pitch += offset_Y;

	if (app.camera_pitch > 80.0f)
		app.camera_pitch = 80.0f;

	if (app.camera_pitch < -80.0f)
		app.camera_pitch = -80.0f;

	calculate_camera_position();

	app.invalid_offset = false;
}

void
raster_input_mouse_cb(GLFWwindow* window, int button, int action, int mods)
{	
	switch (action)
	{
		case GLFW_PRESS:
			switch (button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
					app.mouse_left = true;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					break;

				case GLFW_MOUSE_BUTTON_RIGHT:
					app.mouse_right = true;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					break;
			}
			break;

		case GLFW_RELEASE:
			switch (button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
					app.mouse_left = false;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					break;

				case GLFW_MOUSE_BUTTON_RIGHT:
					app.mouse_right = false;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					break;
			}
			break;


	}
}

void
raster_input_scroll_cb(GLFWwindow* window, double xoffset, double yoffset)
{
	app.camera_distance += ((yoffset * 0.25f) * -1.0f);

	if (app.camera_distance <= 0.1f)
		app.camera_distance = 0.1f;

	calculate_camera_position();
}

void
raster_input_key_cb(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
			switch (key)
			{
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
			}
			break;
	}
}
