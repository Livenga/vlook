#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include "../../include/canvas.h"

#define JpegQuality (80)

int
jwrite(const char *jpath,
       const canvas jcv) {
  FILE   *jfp;

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;


  if((jfp = fopen(jpath, "wb")) == NULL) {
    perror(jpath);
    return EOF;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, jfp);

  cinfo.image_width      = jcv.width;
  cinfo.image_height     = jcv.height;
  cinfo.input_components = 3;
  cinfo.in_color_space   = JCS_RGB;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, JpegQuality, TRUE);

  jpeg_start_compress(&cinfo, TRUE);
  jpeg_write_scanlines(&cinfo, jcv.d, jcv.height);

  jpeg_finish_compress(&cinfo);

  jpeg_destroy_compress(&cinfo);
  fclose(jfp);

  return 0;
}
