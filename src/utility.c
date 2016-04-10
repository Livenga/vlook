#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void
memzero(void *s, size_t n) {
  memset(s, '\0', n);
}


int
confirm_directory(const char *path) {
  if(access(path, F_OK | W_OK) == 0) return 0;
  else {
    if(mkdir(path, 0755) != 0) {
      perror(path);
      return EOF;
    }
  }
  return 0;
}


void
get_savepath(const char *dir,
             const char *extension,
             char       *dest) {
  time_t current_tt = time(NULL);
  struct tm *current_str;

  if((current_str = localtime((const time_t *)&current_tt)) != NULL) {
    sprintf(dest, "%s/%4d%02d%02d_%02d%02d%02d.%s", dir,
        current_str->tm_year + 1900, current_str->tm_mon + 1,
        current_str->tm_mday,        current_str->tm_hour,
        current_str->tm_min,         current_str->tm_sec,
        extension);
  }
}
