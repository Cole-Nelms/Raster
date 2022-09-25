
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef _WIN32

	#include <direct.h>

	#define mkdir(arg) _mkdir(arg)

#elif defined(__unix__) || defined(__APPLE__)

	#include <sys/stat.h>
	#include <sys/types.h>

	#define mkdir(arg) mkdir(arg, 0664)

#endif /* _WIN32 */

#include "raster_log.h"

#define DIR_NAME "log/"
#define FILE_PATH DIR_NAME "info.log"

static FILE* raster_log_file = NULL;

raster_status
raster_log_file_create(void)
{
	int result = mkdir(DIR_NAME);

	if (result != 0 && errno != EEXIST)
	{
		fprintf(stderr, "[INVALID LOG FILE] (%s)\n", strerror(errno));
		return RASTER_FAILED;
	}

	raster_log_file = fopen(FILE_PATH, "w");

	if (!raster_log_file)
	{
		fprintf(stderr, "[INVALID LOG FILE] (%s)\n", strerror(errno));
		return RASTER_FAILED;
	}

	return RASTER_OK;
}

raster_status
raster_log_file_destroy(void)
{
	if (!raster_log_file)
	{
		fprintf(stderr, "[INVALID LOG FILE] (%s)\n", strerror(errno));
		return RASTER_FAILED;
	}

	int result = fclose(raster_log_file);
	raster_log_file = NULL;

	if (result)
	{
		fprintf(stderr, "[INVALID LOG FILE] (%s)\n", strerror(errno));
		return RASTER_FAILED;
	}

	return RASTER_OK;
}

raster_status
raster_log_format(const char* format, ...)
{
	if (!raster_log_file)
	{
		fprintf(stderr, "[INVALID LOG FILE] ");

		va_list arg_list;
		va_start(arg_list, format);

		vfprintf(stderr, format, arg_list);

		va_end(arg_list);

		return RASTER_FAILED;
	}

	va_list arg_list;
	va_start(arg_list, format);

	vfprintf(stderr, format, arg_list);
	vfprintf(raster_log_file, format, arg_list);

	va_end(arg_list);

	return RASTER_OK;
}
