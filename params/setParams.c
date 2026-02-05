#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int set_module_param(const char *module, const char *param, const char *value) {
  char path[256];
  int fd, ret;

  snprintf(path, sizeof(path), "/sys/module/%s/parameters/%s", module, param);

  fd = open(path, O_WRONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }

  ret = write(fd, value, strlen(value));
  if (ret < 0) {
    perror("write");
    close(fd);
    return -1;
  }

  close(fd);
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <module> <param> <value>\n", argv[0]);
    return 1;
  }

  if (set_module_param(argv[1], argv[2], argv[3]) < 0) {
    fprintf(stderr, "Failed to set parameter\n");
    return 1;
  }

  printf("Parameter %s set to '%s' successfully\n", argv[2], argv[3]);
  return 0;
}
