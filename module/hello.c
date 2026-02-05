
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/init.h> /* Needed for the macros */
#include <linux/jiffies.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include <linux/timer.h>
#include <linux/timer_types.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/workqueue_types.h>

#define MAX_PATH_LEN 256

/* Module info */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Max7im");
MODULE_DESCRIPTION("Hello world printer");

/* Module parameters */
static unsigned int g_timerPeriodMs = 1000;
module_param(g_timerPeriodMs, uint, 0);
MODULE_PARM_DESC(g_timerPeriodMs, "Period in ms between runs");

static char *g_filename = "output.txt";
module_param(g_filename, charp, 0);
MODULE_PARM_DESC(g_filename,
                 "A name of a file in output directory to store logs");

/* globals */
static const char g_baseDir[] = "/var/tmp/test_module/";
static struct timer_list g_helloTimer;
static struct work_struct g_writeWork;

static void writeToFileWork(struct work_struct *w) {

  struct file *filp;
  loff_t pos = 0;
  char buf[128];
  int len;
  char fullPath[MAX_PATH_LEN];
  static unsigned int s_counter = 0;

  if (snprintf(fullPath, sizeof(fullPath), "%s%s", g_baseDir, g_filename) >=
      sizeof(fullPath)) {
    pr_err("File name exceeds max size: %lu", sizeof(fullPath));
    return;
  }

  len = snprintf(buf, sizeof(buf), "Hello from kernel module (%u)\n",
                 ++s_counter);
  if (len >= sizeof(buf)) {
    pr_err("Failed to create hello string");
    return;
  }

  filp = filp_open(fullPath, O_WRONLY | O_APPEND | O_CREAT, 0644);

  if (IS_ERR(filp)) {
    pr_err("Failed to open file: \"%s\"", fullPath);
    return;
  }

  kernel_write(filp, buf, len, &pos);

  filp_close(filp, NULL);
}

static void helloTimerCallback(struct timer_list *t) {
  schedule_work(&g_writeWork);
  mod_timer(&g_helloTimer, jiffies + msecs_to_jiffies(g_timerPeriodMs));
}

static int __init init_hello(void) {
  INIT_WORK(&g_writeWork, &writeToFileWork);

  timer_setup(&g_helloTimer, &helloTimerCallback, 0);
  mod_timer(&g_helloTimer, jiffies + msecs_to_jiffies(g_timerPeriodMs));

  return 0;
}

static void __exit cleanup_hello(void) {
  del_timer_sync(&g_helloTimer);
  cancel_work_sync(&g_writeWork);
}

module_init(init_hello);
module_exit(cleanup_hello);
