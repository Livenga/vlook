#ifndef _VLOOK_H
#define _VLOOK_H

#define Version          "1.0.0"
#define DevicePath       "/dev/video0"
#define DefaultExtension "jpg"
#define SaveDir          "./pictures"

#ifndef __LINUX_VIDEODEV2_H
#include <linux/videodev2.h>
#endif


#ifndef _CANVAS_H
#include "canvas.h"
#endif


/* src/io.c */
extern int
xioctl(int fd, 
       unsigned long request,
       void *arg);


/* src/utility.c */
extern void
memzero(void *s, size_t n);
extern int
confirm_directory(const char *path);
extern void
get_savepath(const char *dir,
             const char *extension,
             char *dest);


/* src/camera/cam_utility.c */
extern int
cam_set_format(int video_fd);
extern int
cam_request_buffer(int video_fd,
                   const int count);
extern void *
set_map_pointer(int  video_fd,
                long *length);
extern int
cam_enqueue_map(int video_fd,
                const int count);
extern int
cam_switch_stream(int video_fd,
                  const int request);
extern int
cam_dequeue_map(int video_fd);


/* src/camera/cam_print.c */
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
print_video_capability(int video_fd);

#endif
