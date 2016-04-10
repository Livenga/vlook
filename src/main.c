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
#include "../include/vlook.h"

#define CAP_WIDTH (640)
#define CAP_HEIGHT (480)

int
main(int argc,
     char *argv[]) {
  extern int errno;

  char   device_path[1024];
  int    video_fd, status;
  long   length;

  void   *map_ptr = NULL;
  struct v4l2_capability v4l2_cap;
  struct v4l2_format         fmt;
  struct v4l2_requestbuffers req;
  struct v4l2_buffer         buf;

  /* initialize */
  memset((void *)device_path, '\0', sizeof(device_path));
  if(argc != 2) strcpy(device_path, (const char *)DevicePath);
  else sprintf(device_path, "/dev/video%d", atoi(argv[1]));

  memset((void *)&fmt, '\0', sizeof(struct v4l2_format));
  memset((void *)&req, '\0', sizeof(struct v4l2_requestbuffers));
  memset((void *)&buf, '\0', sizeof(struct v4l2_buffer));

  if((video_fd = open(device_path, O_RDWR, 0)) < 0) {
    fprintf(stderr, "%s open(2): %s\n", device_path, strerror(errno));
    return EOF;
  }


  /* print capabilities */
  status = ioctl(video_fd, VIDIOC_QUERYCAP, &v4l2_cap);
  //print_video_capability(v4l2_cap);


  /* set video device format */
  fmt.type            = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width   = CAP_WIDTH;
  fmt.fmt.pix.height  = CAP_HEIGHT;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  status = ioctl(video_fd, VIDIOC_S_FMT, &fmt);
  printf("%d\n", status);

  printf("%dx%d\t(%d)\n",
      fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.width * fmt.fmt.pix.height);


  /* requested image buffer */
  req.count  = 1;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  status     = ioctl(video_fd, VIDIOC_REQBUFS, &req);


  buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index  = 0;
  status     = xioctl(video_fd, VIDIOC_QUERYBUF, &buf);

  map_ptr = mmap(NULL, buf.length, PROT_READ, MAP_SHARED, video_fd, buf.m.offset);
  if(map_ptr == MAP_FAILED) {
    perror("mmap(2)");
  }
  length = buf.length;

  memset((void *)&buf, '\0', sizeof(struct v4l2_buffer));
  buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index  = 0;
  status = xioctl(video_fd, VIDIOC_QBUF, &buf);
  printf("%d\n", status);


  /* start capture (stream on) */
  int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  status = xioctl(video_fd, VIDIOC_STREAMON, &type);
 

  /* queue */
  fd_set fds;

  FD_ZERO(&fds);
  FD_SET(video_fd, &fds);

  status = select(video_fd + 1, &fds, NULL, NULL, NULL);

  if(FD_ISSET(video_fd, &fds)) {
    memset((void *)&buf, '\0', sizeof(struct v4l2_buffer));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    status = xioctl(video_fd, VIDIOC_DQBUF, &buf);

    printf("Byte used: %d\n", buf.bytesused);
    printf("Length   : %d\n", buf.length);
    if(buf.bytesused < length)
      perror("ioctl(VIDIOC_DQBUF)");
  }

  unsigned char *capture_buffer = (unsigned char *)calloc(buf.length, sizeof(unsigned char));
  memcpy((void *)capture_buffer, (const void *)map_ptr, buf.length);

  unsigned char *yuyv = capture_buffer;
  int i, j;
  int z = 0;
  int y, u, v;
  double r, g, b;
  unsigned char **colors;

  colors = (unsigned char **)calloc(CAP_HEIGHT, sizeof(unsigned char *));
  for(i = 0; i < CAP_HEIGHT; i++) {
    colors[i] = (unsigned char *)calloc(CAP_WIDTH * 3, sizeof(unsigned char));

    for(j = 0; j < CAP_WIDTH; j++) {
      if(!z) {
        y = yuyv[0];
      } else {
        y = yuyv[2];
      }
      
      u = yuyv[1] - 128;
      v = yuyv[3] - 128;


      r = y + (1.4065 * v);
      g = y - (0.3455 * u) - (0.7169 * v);
      b = y + (1.7790 * u);

      colors[i][j * 3 + 0] = (unsigned char)((r > 255.0) ? 255.0 : (r < 0.0) ? 0.0 : r);
      colors[i][j * 3 + 1] = (unsigned char)((g > 255.0) ? 255.0 : (g < 0.0) ? 0.0 : g);
      colors[i][j * 3 + 2] = (unsigned char)((b > 255.0) ? 255.0 : (b < 0.0) ? 0.0 : b);

#if 0
      printf("0x%02X%02X%02X\n",
          colors[i][j * 3 + 0],
          colors[i][j * 3 + 1],
          colors[i][j * 3 + 2]);
#endif

      if(z++) {
        z = 0;
        yuyv += 4;
      }
    }
  }

  pnwrite("./sample.png", CAP_WIDTH, CAP_HEIGHT, (const unsigned char **)colors);

  for(i = 0; i < CAP_HEIGHT; i++) {
    memset(colors[i], '\0', CAP_WIDTH * 3);
    free(colors[i]); colors[i] = NULL;
  }
  free(colors); colors = NULL;

  memset((void *)capture_buffer, '\0', buf.length);
  free(capture_buffer);
  capture_buffer = NULL;

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  status = xioctl(video_fd, VIDIOC_STREAMOFF, &type);


  munmap(map_ptr, length);

  /* post processing */
  close(video_fd);

  return 0;
}
