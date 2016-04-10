#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <getopt.h>

#include "../include/vlook.h"

/* src/main.c */
static void
print_help(const char *app);
static void
print_version(const char *app);


int
main(int argc,
     char *argv[]) {
  extern int errno;
  static int f_help, f_ver, f_ext, f_device, f_verbose, f_cpb;

  const struct option opts[] = {
    {"device",     required_argument, 0, 'd'},
    {"extension",  required_argument, 0, 'e'},
    {"help",       no_argument,       0, 'h'},
    {"version",    no_argument,       0, 'V'},
    {"verbose",    no_argument,       0, 'v'},
    {"capability", no_argument,       0, 'c'},
    {0, 0, 0, 0},
  };

  int    opt, optindex;
  int    video_fd, status, cnt_fd;
  long   length;

  char   device_path[1024], extension[12];
  char   save_dir[256], save_path[1024];

  void        *map_ptr = NULL;
  canvas      cv_cap;
  canvas_yuyv cv_yuyv;


  /* initialize */
  memset((void *)extension,   '\0', sizeof(extension));
  memset((void *)device_path, '\0', sizeof(device_path));
  memset((void *)save_dir,    '\0', sizeof(save_dir));
  memset((void *)save_path,   '\0', sizeof(save_path));


  /* option */
  while((opt = getopt_long(argc, argv, "hvVdcea:",
          opts, &optindex)) != EOF) {
    switch(opt) {
      case 'h': f_help = 1; break;
      case 'V': f_ver  = 1; break;
      case '?': f_help = 1; break;
      case 'c': f_cpb  = 1; break;
      default:  f_help = 1; break;

      case 'v': f_verbose = 1; break;
      case 'd': f_device    = 1;
                strcpy((char *)device_path,
                    (const char *)((optarg == NULL) ? argv[optind] : optarg));
                break;
      case 'e': f_ext = 1;
                strcpy((char *)extension,
                    (const char *)((optarg == NULL) ? argv[optind] : optarg));
                break;
    }
  }

  if(f_help || f_ver) { // Help & Version
    if(f_help) print_help(argv[0]);
    if(f_ver)  print_version(argv[0]);
  }

  if(!f_device)
    strncpy(device_path, DevicePath, strlen(DevicePath));
  if(!f_ext)
    strncpy(extension, DefaultExtension, 3);

  if(f_verbose) {
    printf("[Device Path] %s\n", device_path);
    printf("[Extension]   %s\n", extension);
  }

  
  if(confirm_directory(SaveDir) == 0)
    strncpy(save_dir, SaveDir, strlen(SaveDir));
  else
    strncpy(save_dir, ".", 1);


  /* Open video device */
  cnt_fd = 0;
  do {
    if((video_fd = open(device_path, O_RDWR, 0)) < 0) {
      fprintf(stderr, "%s open(2): %s\n", device_path, strerror(errno));
      sleep(1);
      if(cnt_fd++ == 10) return EOF;
    }
  } while(video_fd < 0);


  if(f_cpb) {
    print_video_capability(video_fd);
    close(video_fd);
    return 0;
  }

  /* set video device format */
  status = cam_set_format(video_fd);
  if(status < 0) {
    perror("ioctl(VIDIOC_S_FMT)");
    goto close_fd;
  }


  /* requested image buffer */
  status     = cam_request_buffer(video_fd, 1);
  if(status < 0) {
    perror("ioctl(VIDIOC_REQBUFS)");
    goto close_fd;
  }


  /* set mmap */
  map_ptr = set_map_pointer(video_fd, &length);
  if(map_ptr == MAP_FAILED) {
    perror("mmap(2)");
    goto close_fd;
  }


  /* enqueue void buffer */
  status = cam_enqueue_map(video_fd, 0);
  if(status < 0) {
    perror("ioctl(VIDIOC_QBUF)");
    munmap(map_ptr, length);
    goto close_fd;
  }


  cv_yuyv = cv_yuyv_init(CapWidth, CapHeight, length);

  /* start capture (stream on) */
  cam_switch_stream(video_fd, VIDIOC_STREAMON);

  /* dequeue */
  cam_dequeue_map(video_fd);

  memmove((void *)cv_yuyv.yuyv, (const void *)map_ptr, length);
  yuyv2rgb(&cv_cap, cv_yuyv);

  get_savepath(save_dir, extension, save_path);
  if(f_verbose) fprintf(stderr, "saved %s\n", save_path);
  if(strcmp(extension, DefaultExtension) == 0)
    jwrite(save_path, cv_cap);
  else
    pnwrite(save_path, cv_cap);


  cam_switch_stream(video_fd, VIDIOC_STREAMOFF);


  /* post processing */
  destroy_canvas_yuyv(&cv_yuyv);
  destroy_canvas(&cv_cap);
  munmap(map_ptr, length);

  close(video_fd);

  return 0;

close_fd:
  close(video_fd);
  return EOF;
}

static void
print_help(const char *app) {
  fprintf(stderr, "Usave: %s [Options...]\n\n", app);
  fprintf(stderr, "[Options]\n");
  fprintf(stderr, "  -h, --help:                  Print Help.\n");
  fprintf(stderr, "  -v, --version:               Print Version.\n\n");
  fprintf(stderr, "  -d, --device=[Device]:       Used Device file.\n");
  fprintf(stderr, "  -e, --extension=[Extension]: Used Extension.\n\n");
  exit(0);
}

static void
print_version(const char *app) {
  fprintf(stderr, "%s %s\n", app, Version);
  exit(0);
}
