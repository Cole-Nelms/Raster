
#ifndef RASTER_RENDER_H
#define RASTER_RENDER_H

#include "raster_types.h"

raster_GLuint
raster_render_shader_create(const GLuint type, const char* path);

raster_GLuint
raster_render_shader_program_create(const GLuint vert, const GLuint frag);

raster_status
raster_render_create(const char* title, const int width, const int height);

void
raster_render_destroy(void);

#endif /* RASTER_RENDER_H */