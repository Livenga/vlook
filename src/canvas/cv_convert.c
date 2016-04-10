#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/canvas.h"

void
yuyv2rgb(canvas *dest,
         const canvas_yuyv src) {
  int i, j, z;
  int y, u, v;
  uchar *yuyv;
  
  double r, g, b;

  dest->width  = src.width;
  dest->height = src.height;

  yuyv = src.yuyv;

  z = 0;
  dest->d = (uchar **)calloc(src.height, sizeof(uchar *));
  for(i = 0; i < src.height; i++) {
    dest->d[i] = (uchar *)calloc(src.width * 3, sizeof(uchar));

    for(j = 0; j < src.width; j++) {
      if(!z) y = yuyv[0];
      else   y = yuyv[2];
      u = yuyv[1] - 128;
      v = yuyv[3] - 128;

      r = y + (1.4065 * v);
      g = y - (0.3455 * u) - (0.7169 * v);
      b = y + (1.7790 * u);

      dest->d[i][j * 3 + 0] = (uchar)((r > 255.0) ? 255.0 : (r < 0.0) ? 0.0 : r);
      dest->d[i][j * 3 + 1] = (uchar)((g > 255.0) ? 255.0 : (g < 0.0) ? 0.0 : g);
      dest->d[i][j * 3 + 2] = (uchar)((b > 255.0) ? 255.0 : (b < 0.0) ? 0.0 : b);

      if(z++) {
        z     = 0;
        yuyv += 4;
      }
    }
  }
}
