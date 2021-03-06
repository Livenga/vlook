#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <png.h>
#include "../../include/canvas.h"

int
pnwrite(const char *png_path,
        const canvas png_cv) {
  FILE *png_fp;
  png_structp png_ptr;
  png_infop info_ptr;

  if((png_fp = fopen(png_path, "wb")) == NULL) {
    perror(png_path);
    return EOF;
  }

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);

  png_init_io(png_ptr, png_fp);

  png_set_IHDR(png_ptr, info_ptr,
      png_cv.width, png_cv.height, 8, PNG_COLOR_TYPE_RGB,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);
  png_write_image(png_ptr, (const png_bytepp)png_cv.d);
  png_write_end(png_ptr, info_ptr);

  png_destroy_info_struct(png_ptr, &info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  fclose(png_fp);

  return 0;
}
