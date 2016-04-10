#ifndef _CANVAS_H
#define _CANVAS_H

//#define _CANVAS_DEBUG

#define CapWidth  (640)
#define CapHeight (480)

typedef unsigned char uchar;

typedef struct _canvas_yuyv {
  int   width;
  int   height;
  long  length;
  uchar *yuyv;
} canvas_yuyv;


typedef struct _canvas {
  int   width;
  int   height;
  uchar **d;
} canvas;


/* src/canvas/cv_init.c */
extern canvas_yuyv
cv_yuyv_init(const int  width,
             const int  height,
             const long length);

/* src/canvas/cv_convert.c */
extern void
yuyv2rgb(canvas *dest,
         const canvas_yuyv src);


/* src/canvas/cv_png.c */
extern int
pnwrite(const char *png_path,
        const canvas png_cv);

/* src/canvas/cv_jpeg.c */
extern int
jwrite(const char *jpath,
       const canvas jcv);


/* src/canvas/cv_utility.c */
extern void
destroy_canvas_yuyv(canvas_yuyv *dest);
extern void
destroy_canvas(canvas *dest);

#endif
