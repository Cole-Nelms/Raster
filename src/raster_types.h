
#ifndef RASTER_TYPES_H
#define RASTER_TYPES_H

#include <stdbool.h>
#include "linmath.h"
#include "glad.h"
#include <glfw/glfw3.h>

typedef enum raster_status
{
	/* generic */
	RASTER_OK,
	RASTER_FAILED,
} raster_status;

typedef struct raster_GLuint
{
	GLuint				data;
	raster_status status;
} raster_GLuint;

typedef struct raster_buffer
{
	uint8_t* data;
	size_t	 size;
	size_t	 length;
} raster_buffer;

typedef struct raster_app
{
	float delta_time;
	float last_frame_time;

	bool mouse_right, mouse_left;

	bool first_cursor, invalid_offset;
	float cursor_last_X, cursor_last_Y;

	GLFWwindow* window;
	GLuint			shader_program;

	float camera_yaw, camera_pitch, camera_distance;
	vec3 camera_position, camera_forward, camera_up;
	mat4x4 model, view, projection;
} raster_app;

#endif /* RW_TYPES_H */
