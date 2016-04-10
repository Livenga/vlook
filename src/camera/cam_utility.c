#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>

#include <linux/videodev2.h>
#include "../../include/vlook.h"

int
cam_set_format(int video_fd) {
  struct v4l2_format fmt;
  memzero((void *)&fmt, sizeof(struct v4l2_format));

  /* set video device format */
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = CapWidth;
  fmt.fmt.pix.height      = CapHeight;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  return xioctl(video_fd, VIDIOC_S_FMT, &fmt);
}


int
cam_request_buffer(int video_fd,
                   const int count) {
  struct v4l2_requestbuffers req;
  memzero((void *)&req, sizeof(struct v4l2_requestbuffers));

  req.count  = count;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  return xioctl(video_fd, VIDIOC_REQBUFS, &req);
}


void *
set_map_pointer(int video_fd,
                long *length) {
  int  status;
  void *map_ptr;

  struct v4l2_buffer buf;
  memzero((void *)&buf, sizeof(struct v4l2_buffer));

  buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index  = 0;
  status     = xioctl(video_fd, VIDIOC_QUERYBUF, &buf);
  if(status < 0) return NULL;

  *length = buf.length;

  map_ptr = mmap(NULL, buf.length, PROT_READ, MAP_SHARED,
      video_fd, buf.m.offset);

  if(map_ptr == MAP_FAILED) return NULL;

  return map_ptr;
}


int
cam_enqueue_map(int video_fd,
                const int count) {
  struct v4l2_buffer buf;
  memset((void *)&buf, '\0', sizeof(struct v4l2_buffer));

  buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index  = count;

  return xioctl(video_fd, VIDIOC_QBUF, &buf);
}


int
cam_switch_stream(int video_fd,
                  const int request) {
  int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  return xioctl(video_fd, request, (int *)&type);
}


int
cam_dequeue_map(int video_fd) {
  int    status;
  fd_set fds;

  struct v4l2_buffer buf;
  memset((void *)&buf, '\0', sizeof(struct v4l2_buffer));

  FD_ZERO(&fds);
  FD_SET(video_fd, &fds);

  select(video_fd + 1, &fds, NULL, NULL, NULL);

  if(FD_ISSET(video_fd, &fds)) {
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    status = xioctl(video_fd, VIDIOC_DQBUF, &buf);

    if(buf.bytesused < buf.length)
      perror("ioctl(VIDIOC_DQBUF)");
  }
  return status;
}
