#ifndef _VLOOK_H
#define _VLOOK_H

#define DevicePath "/dev/video0"

#ifndef __LINUX_VIDEODEV2_H
#include <linux/videodev2.h>
#endif

/* src/io.c */
extern int
xioctl(int fd, 
       unsigned long request,
       void *arg);

/* src/canvas/cv_png.c */
extern int
pnwrite(const char *png_path,
        const int width,
        const int height,
        const unsigned char **colors);


/* src/video/video_print.c */
#ifdef _PRINT_CAPABILITY
typedef struct capability_field {
  __u32 id;
  char     *name;
} capability_field;

capability_field field[] = {
  {V4L2_CAP_VIDEO_CAPTURE, "V4L2_CAP_VIDEO_CAPTURE"},
  {V4L2_CAP_VIDEO_OUTPUT, "V4L2_CAP_VIDEO_OUTPUT"},
  {V4L2_CAP_VIDEO_OVERLAY, "V4L2_CAP_VIDEO_OVERLAY"},
  {V4L2_CAP_VBI_CAPTURE, "V4L2_CAP_VBI_CAPTURE"},
  {V4L2_CAP_VBI_OUTPUT, "V4L2_CAP_VBI_OUTPUT"},
  {V4L2_CAP_SLICED_VBI_CAPTURE, "V4L2_CAP_SLICED_VBI_CAPTURE"},
  {V4L2_CAP_SLICED_VBI_OUTPUT, "V4L2_CAP_SLICED_VBI_OUTPUT"},
  {V4L2_CAP_RDS_CAPTURE, "V4L2_CAP_RDS_CAPTURE"},
  {V4L2_CAP_VIDEO_OUTPUT_OVERLAY, "V4L2_CAP_VIDEO_OUTPUT_OVERLAY"},
  {V4L2_CAP_HW_FREQ_SEEK, "V4L2_CAP_HW_FREQ_SEEK"},
  {V4L2_CAP_RDS_OUTPUT, "V4L2_CAP_RDS_OUTPUT"},
  {V4L2_CAP_VIDEO_CAPTURE_MPLANE, "V4L2_CAP_VIDEO_CAPTURE_MPLANE"},
  {V4L2_CAP_VIDEO_OUTPUT_MPLANE, "V4L2_CAP_VIDEO_OUTPUT_MPLANE"},
  {V4L2_CAP_VIDEO_M2M_MPLANE, "V4L2_CAP_VIDEO_M2M_MPLANE"},
  {V4L2_CAP_VIDEO_M2M, "V4L2_CAP_VIDEO_M2M"},
  {V4L2_CAP_TUNER, "V4L2_CAP_TUNER"},
  {V4L2_CAP_AUDIO, "V4L2_CAP_AUDIO"},
  {V4L2_CAP_RADIO, "V4L2_CAP_RADIO"},
  {V4L2_CAP_MODULATOR, "V4L2_CAP_MODULATOR"},
  {V4L2_CAP_SDR_CAPTURE, "V4L2_CAP_SDR_CAPTURE"},
  {V4L2_CAP_EXT_PIX_FORMAT, "V4L2_CAP_EXT_PIX_FORMAT"},
  {V4L2_CAP_SDR_OUTPUT, "V4L2_CAP_SDR_OUTPUT"},
  {V4L2_CAP_READWRITE, "V4L2_CAP_READWRITE"},
  {V4L2_CAP_ASYNCIO, "V4L2_CAP_ASYNCIO"},
  {V4L2_CAP_STREAMING, "V4L2_CAP_STREAMING"},
  {V4L2_CAP_DEVICE_CAPS, "V4L2_CAP_DEVICE_CAPS"},
  {0, NULL},
};
#endif

extern void
print_video_capability(struct v4l2_capability cap);

/// ...
#endif
