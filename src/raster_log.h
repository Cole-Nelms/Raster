
#ifndef RASTER_LOG_H
#define RASTER_LOG_H

#include "raster_types.h"

#define STRFY_EXPAND(arg) #arg
#define STRFY(arg) STRFY_EXPAND(arg)

#ifdef NDEBUG
	#define raster_log(format, ...) raster_log_format("[LOG] " format,							\
																										__VA_ARGS__)
#else
	#define raster_log(format, ...) raster_log_format("[LOG] "											\
																										__FILE__											\
																										"("														\
																										STRFY(__LINE__)								\
																										")"														\
																										": " format,									\
																										__VA_ARGS__)
#endif /* NDEBUG */

raster_status
raster_log_file_create(void);

raster_status
raster_log_file_destroy(void);

raster_status
raster_log_format(const char* format, ...);

#endif /* RASTER_LOG_H */