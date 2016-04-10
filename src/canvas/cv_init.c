#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/canvas.h"

canvas_yuyv
cv_yuyv_init(const int  width,
             const int  height,
             const long length) {
  canvas_yuyv cv_yuyv;

  cv_yuyv.width  = width;
  cv_yuyv.height = height;
  cv_yuyv.length = length;
  cv_yuyv.yuyv   = (uchar *)calloc(length, sizeof(uchar));

#if defined(_CANVAS_DEBUG)
  printf("%dx%d\t%d(length: %ld)\n", width, height,
      width * height, length);
#endif
  return cv_yuyv;
}
