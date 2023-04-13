#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cJSON.h"
#include "cJSON-file-handler.h"

#define JSON_FILE "cosmicagenda.json"

int
cjson_file_handler_file_exists (const char *filename)
{
  FILE *file;
  if ((file = fopen(filename, "r")))
  {
    fclose(file);
    return 1;
  }
  return 0;
}

int
cjson_file_handler_open (FILE **file, const char *mode)
{
  *file = NULL;
  *file = fopen(JSON_FILE, mode);

  if (*file == NULL)
  {
    fprintf(stderr, "cjson_file_handler_open: Error opening %s.\n", JSON_FILE);
    return 1;
  }
  return 0;
}

int
cjson_file_handler_read(FILE **file, char **buffer)
{
  // TODO: add error handling for a return code

  // Get file size
  fseek(*file, 0, SEEK_END); 
  long fsize = ftell(*file);
  fseek(*file, 0, SEEK_SET);

  // Read file into buffer
  *buffer = malloc(fsize + 1);
  fread(*buffer, fsize, 1, *file);
  (*buffer)[fsize] = '\0';

  return 0;
}

int
cjson_file_handler_write (FILE **file, cJSON *json_array)
{
  char *json_string = cJSON_Print(json_array);
  fputs(json_string, *file);
  free(json_string);
  return 0;
}


int
cjson_file_handler_close(FILE **file)
{
  if (fclose(*file))
  {
    fprintf(stderr, "cjson_file_handler_close: Error could not close file %s.\n", JSON_FILE);
    if (ferror(*file))
    {
      fprintf(stderr, "cjson_file_handler_close: I/O error occured.\n"); 
      fprintf(stderr, "  errno: %s.\n", strerror(errno));
      return 1;
    }
  }
  return 0;
}

int
cjson_file_handler_parse(FILE **file, char **buffer, cJSON **json_array)
{
  *json_array = cJSON_Parse(*buffer);
  if (*json_array == NULL)
  {
    fprintf(stderr, "cjson_file_handler_parse: Error parsing buffer.\n");

    if (cjson_file_handler_close(file) != 0)
      fprintf(stderr, "cjson_file_handler_parse: Error closing file.\n");

    free(*buffer);
    return 1;
  }
  
  return 0;
}

void cjson_file_handler_init(FILE **file, char **buffer, cJSON **json_array)
{
  // If the file exists read it into the json_array and close it.
  // Then reopen the file for writing.
  if (cjson_file_handler_file_exists(JSON_FILE))
  {
    cjson_file_handler_open(file, "r");
    cjson_file_handler_read(file, buffer);
    cjson_file_handler_parse(file, buffer, json_array);
    cjson_file_handler_close(file);

    cjson_file_handler_open(file, "w");
  }
  // If the file does not exist create it and open it for writing.
  else
  {
    cjson_file_handler_open(file, "w");
  }
}
