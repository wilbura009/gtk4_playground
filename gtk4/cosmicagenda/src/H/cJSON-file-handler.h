#ifndef CJSON_FILE_HANDLER_H
#define CJSON_FILE_HANDLER_H

#include <stdio.h>
#include "cJSON.h"

int cjson_file_handler_file_exists (const char *filename);
int cjson_file_handler_open  (FILE **file, const char *mode);
int cjson_file_handler_read  (FILE **file, char **buffer);
int cjson_file_handler_parse (FILE **file, char **buffer, cJSON **json);
int cjson_file_handler_write (FILE **file, cJSON *json);
int cjson_file_handler_close (FILE **file);

void cjson_file_handler_init (FILE **file, char **buffer, cJSON **json);

#endif
