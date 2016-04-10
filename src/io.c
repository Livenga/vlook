#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>

int
xioctl(int fd, 
       unsigned long request,
       void *arg) {
  extern int errno;
  int status;

  do {
    status = ioctl(fd, request, arg);
  } while(status == -1 && EINTR == errno);

  return status;
}
