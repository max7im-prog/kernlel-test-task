#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MODULE_NAME "hello"
#define SYSFS_BASE "/sys/module/" MODULE_NAME "/parameters/"

static int write_param(const char *param, const char *value) {
  char path[256];
  int fd, ret;

  snprintf(path, sizeof(path), SYSFS_BASE "%s", param);

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
  int opt;
  char *output_file = NULL;
  char *time_sec_str = NULL;

  while ((opt = getopt(argc, argv, "o:t:")) != -1) {
    switch (opt) {
    case 'o':
      output_file = optarg;
      break;
    case 't':
      time_sec_str = optarg;
      break;
    default:
      fprintf(stderr, "Usage: %s [-o output_file] [-t time_in_seconds]\n",
              argv[0]);
      return 1;
    }
  }

  if (!output_file && !time_sec_str) {
    fprintf(stderr, "Usage: %s [-o output_file] [-t time_in_seconds]\n",
            argv[0]);
    return 1;
  }

  if (output_file) {
    if (write_param("g_filename", output_file) < 0) {
      fprintf(stderr, "Failed to set output file\n");
      return 1;
    }
    printf("Output file set to '%s'\n", output_file);
  }

  if (time_sec_str) {
    // Convert seconds to milliseconds
    long time_sec = strtol(time_sec_str, NULL, 10);
    if (time_sec <= 0) {
      fprintf(stderr, "Invalid time value: %s\n", time_sec_str);
      return 1;
    }

    char time_ms[32];
    snprintf(time_ms, sizeof(time_ms), "%ld", time_sec * 1000);

    if (write_param("g_timerPeriodMs", time_ms) < 0) {
      fprintf(stderr, "Failed to set timer period\n");
      return 1;
    }
    printf("Timer period set to %ld seconds (%s ms)\n", time_sec, time_ms);
  }

  return 0;
}
