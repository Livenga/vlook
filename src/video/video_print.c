#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <linux/videodev2.h>
#define _PRINT_CAPABILITY
#include "../../include/vlook.h"

void
print_video_capability(struct v4l2_capability cap) {
  extern capability_field field[];
  capability_field *field_ptr = (capability_field *)field;

  printf("Driver:  %s\n", (char *)cap.driver);
  printf("Card:    %s\n", (char *)cap.card);
  printf("BusInfo: %s\n", (char *)cap.bus_info);

  printf("Version: 0x%x\n", (int)cap.version);

  printf("[Support Capabilities]\n");
  while(field_ptr->name != NULL) {
    printf("%32s:\t%s\n", field_ptr->name,
        (field_ptr->id & cap.capabilities) > 0 ?
        "\033[1;32mSupported.\033[0m" : "\033[1;35mNot supported.\033[0m");
    field_ptr++;
  }
}
