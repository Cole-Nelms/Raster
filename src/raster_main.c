
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "raster_log.h"
#include "raster_types.h"
#include "raster_window.h"
#include "raster_render.h"
#include "raster_input.h"

raster_app app;

static float verts[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int
main(void)
{
	/***********************************************
	 * initialize																	 *
	 ***********************************************/

	raster_render_create("test", 800, 600);

	/***********************************************
	 * vertex setup																 *
	 ***********************************************/

	/* gen buffers */
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	/* bind buffers */
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/* set buffers */
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	/***********************************************
	 * vertex attrs																 *
	 ***********************************************/

	static size_t vert_stride = (5 * sizeof(float));

	/* vertex attr */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vert_stride, (void*)0);
	glEnableVertexAttribArray(0);

	/* texture coord attr */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vert_stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/***********************************************
	 * texture																		 *
	 ***********************************************/

	/* gen textures */
	unsigned int tex;
	glGenTextures(1, &tex);

	/* bind texture 0 */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	/* texture wrapping/filtering */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* flip texture vertically */
	stbi_set_flip_vertically_on_load(true);

	/* load texture */
	int width, height, channels;
	unsigned char* data = stbi_load("data/textures/lava.jpg", &width, &height, &channels, 0);

	if (!data)
	{
		raster_log("failed to load texture");
		return RASTER_FAILED;
	}

	/* set texture 0, and generated mipmaps */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	/* set texture uniform to 0 */
	int tex_loc = glGetUniformLocation(app.shader_program, "u_tex");
	glUniform1i(tex_loc, 0);

	/***********************************************
	 * main loop																	 *
	 ***********************************************/

	while (!glfwWindowShouldClose(app.window))
	{
		{
			/* calculate delatime */
			const float time = glfwGetTime();

			app.delta_time			= time - app.last_frame_time;
			app.last_frame_time = time;
		}

		/* window width/height */
		int width, height;
		glfwGetFramebufferSize(app.window, &width, &height);

		/* aspect ration */
		float ratio = (float)width / (float)height;

		/***********************************************
		 * model, view, projection										 *
		 ***********************************************/

		mat4x4_identity(app.model);
		mat4x4_look_at(app.view, app.camera_position, app.camera_forward, app.camera_up);
		mat4x4_perspective(app.projection, 45.0f, ratio, 0.1f, 100.0f);

		/* uniforms */
		int model_loc = glGetUniformLocation(app.shader_program, "u_model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, (const GLfloat*)app.model);

		int view_loc = glGetUniformLocation(app.shader_program, "u_view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, (const GLfloat*)app.view);

		int unif_proj_loc = glGetUniformLocation(app.shader_program, "u_proj");
		glUniformMatrix4fv(unif_proj_loc, 1, GL_FALSE, (const GLfloat*)app.projection);

		/* clear buffers */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* draw command */
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(app.window);
		glfwPollEvents();
	}

	/***********************************************
	 * cleanup  																	 *
	 ***********************************************/

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	raster_render_destroy();

	return EXIT_SUCCESS;
}
