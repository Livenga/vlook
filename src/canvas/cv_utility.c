#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/canvas.h"

void
destroy_canvas_yuyv(canvas_yuyv *dest) {
  memset((void *)dest->yuyv, '\0', dest->length);
  free(dest->yuyv);
  dest->yuyv = NULL;

  dest->width  = 0;
  dest->height = 0;
}

void
destroy_canvas(canvas *dest) {
  int i;

  for(i = 0; i < dest->height; i++) {
    memset((void *)dest->d[i], '\0', dest->width * 3);
    free(dest->d[i]);
    dest->d[i] = NULL;
  }
  free(dest->d);
  dest->d = NULL;

  dest->width  = 0;
  dest->height = 0;
}
