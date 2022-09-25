
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raster_filesystem.h"
#include "raster_log.h"

raster_buffer
raster_filesystem_file_read(const char* path)
{
	raster_buffer buffer;
	buffer.size		= 0;
	buffer.length	= 0;
	buffer.data		= NULL;

	FILE* file = fopen(path, "rb");

	if (!file)
	{
		raster_log("failed to open '%s': %s\n", path, strerror(errno));
		goto failed;
	}

	if (fseek(file, 0, SEEK_END))
	{
		raster_log("'fseek' failed on '%s': %s\n", path, strerror(errno));
		goto failed;
	}

	size_t file_length = ftell(file);

	if (file_length == -1)
	{
		raster_log("'ftell' failed on '%s': %s\n", path, strerror(errno));
		goto failed;
	}

	if (fseek(file, 0, SEEK_SET))
	{
		raster_log("'fseek' failed on '%s': %s\n", path, strerror(errno));
		goto failed;
	}

	buffer.size = file_length + 1;
	buffer.data = malloc(buffer.size);

	if (!buffer.data)
	{
		raster_log("failed to allocate memory: %s\n", strerror(errno));
		goto failed;
	}

	buffer.length = fread(buffer.data, sizeof(char), file_length, file);

	if (buffer.length != file_length)
	{
		raster_log("failed to read '%s': %s\n", path, strerror(errno));
		goto failed;
	}

	buffer.data[buffer.size - 1] = '\0';

	if (fclose(file))
	{
		raster_log("failed to close '%s': %s\n", path, strerror(errno));
		goto failed;
	}

	return buffer;

	failed:
		free(buffer.data);
		buffer.length = 0;
		buffer.size		= 0;

		return buffer;
}
