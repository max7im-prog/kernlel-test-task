
#include <linux/init.h> /* Needed for the macros */
#include <linux/jiffies.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include <linux/timer.h>
#include <linux/timer_types.h>

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
static const char g_baseDir[] = "/var/temp/test_module";
static char g_fullPath[MAX_PATH_LEN];
static struct timer_list g_helloTimer;

static void helloTimerCallback(struct timer_list *t) {
  static unsigned int counter = 0;
  pr_info("Hello from kernel module (%u)", ++counter);

  // if (snprintf(g_fullPath, sizeof(g_fullPath), "%s%s", g_baseDir, g_filename)
  // >=
  //     sizeof(g_fullPath)) {
  //   // TODO: handle truncation
  // }

  mod_timer(&g_helloTimer, jiffies + msecs_to_jiffies(g_timerPeriodMs));
}

static int __init init_hello(void) {
  timer_setup(&g_helloTimer, &helloTimerCallback, 0);
  mod_timer(&g_helloTimer, jiffies + msecs_to_jiffies(g_timerPeriodMs));
  return 0;
}

static void __exit cleanup_hello(void) { del_timer_sync(&g_helloTimer); }

module_init(init_hello);
module_exit(cleanup_hello);
