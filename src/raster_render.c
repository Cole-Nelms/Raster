
#include "raster_filesystem.h"
#include "raster_log.h"
#include "raster_window.h"

extern raster_app app;

raster_GLuint
raster_render_shader_create(const GLuint type, const char*	 path)
{
	raster_GLuint shader;
	shader.data = glCreateShader(type);

	raster_buffer shader_src = raster_filesystem_file_read(path);

	if (!shader_src.data)
	{
		shader.status = RASTER_FAILED;
		goto exit;
	}

	glShaderSource(shader.data, 1, &shader_src.data, NULL);
	glCompileShader(shader.data);

	GLint result;
	glGetShaderiv(shader.data, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		shader.status = RASTER_FAILED;

		char desc[512];
		glGetShaderInfoLog(shader.data, 512, NULL, desc);

		switch (type)
		{
			case GL_VERTEX_SHADER:
				raster_log("failed to compile vertex shader: %s\n", desc);
				goto exit;

			case GL_FRAGMENT_SHADER:
				raster_log("failed to compile fragment shader: %s\n", desc);
				goto exit;
		}
	}

	shader.status = RASTER_OK;

	exit:
		free(shader_src.data);
		return shader;
}

raster_GLuint
raster_render_shader_program_create(GLuint vert, GLuint frag)
{
	raster_GLuint program;
	program.data = glCreateProgram();

	glAttachShader(program.data, vert);
	glAttachShader(program.data, frag);
	glLinkProgram(program.data);

	GLint	result;
	glGetProgramiv(program.data, GL_LINK_STATUS, &result);

	if (!result)
	{
		program.status = RASTER_FAILED;

		char	desc[512];
		glGetProgramInfoLog(program.data, 512, NULL, desc);

		raster_log("failed to compile shader program: %s\n", desc);
		return program;
	}

	program.status = RASTER_OK;
	return program;
}

raster_status
raster_render_create(const char* title, const int width, const int height)
{
	raster_log_file_create();

	raster_window_create(title, width, height);

	if (!app.window)
		return RASTER_FAILED;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		raster_log("failed to load OpenGL");
		return RASTER_FAILED;
	}

	glViewport(0, 0, width, height);

	raster_GLuint vert = raster_render_shader_create(GL_VERTEX_SHADER, "shaders/vert.glsl");
	raster_GLuint frag = raster_render_shader_create(GL_FRAGMENT_SHADER, "shaders/frag.glsl");
	raster_GLuint program = raster_render_shader_program_create(vert.data, frag.data);

	if (vert.status != RASTER_OK || frag.status != RASTER_OK || program.status != RASTER_OK)
		goto exit;

	/* cursor */
	app.first_cursor = true;

	app.cursor_last_X = 0.0;
	app.cursor_last_Y = 0.0;

	/* delta time */
	app.delta_time			 = 0.0f;
	app.last_frame_time = 0.0f;

	/* camera orientation */
	app.camera_yaw			= -90.0f;
	app.camera_pitch		= 0.0f;
	app.camera_distance = 3.0f;

	app.camera_position[0] = 0.0f;
	app.camera_position[1] = 0.0f;
	app.camera_position[2] = app.camera_distance;

	app.camera_forward[0] = 0.0f;
	app.camera_forward[1] = 0.0f;
	app.camera_forward[2] = 0.0f;

	app.camera_up[0] = 0.0f;
	app.camera_up[1] = 1.0f;
	app.camera_up[2] = 0.0f;

	/* model, view, projection */
	mat4x4_identity(app.model);
	mat4x4_identity(app.view);
	mat4x4_identity(app.projection);

	/* set shader */
	app.shader_program = program.data;
	glUseProgram(app.shader_program);

	/* background color */
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	/* enable depth */
	glEnable(GL_DEPTH_TEST);

	exit:
		glDeleteShader(vert.data);
		glDeleteShader(frag.data);
}

void
raster_render_destroy(void)
{
	glDeleteProgram(app.shader_program);
	glfwTerminate();
	raster_log_file_destroy();
}