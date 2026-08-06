#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Konygin");
MODULE_DESCRIPTION("Kernel timer module: 30s interval, stops after 5 mins");
MODULE_VERSION("1.0");

static const unsigned long TIMER_INTERVAL_MS = 30000;
static const unsigned int MAX_MINUTES = 5;

static struct timer_list g_timer;
static int tick_count = 1;
static unsigned long start_jiffies;
static unsigned long target_time_finish;

static void timer_callback(struct timer_list *t) {
  ++tick_count;

  if (time_before(jiffies, target_time_finish)) {
    unsigned int delta = jiffies_to_msecs(jiffies - start_jiffies);
    pr_info("min=%d: Hello, timer!\n", delta / 60000);
    unsigned long target_time = start_jiffies + msecs_to_jiffies(TIMER_INTERVAL_MS * tick_count);
    mod_timer(&g_timer, target_time);
  } else {
    pr_info("Timer stopped: 5 minutes elapsed.\n");
  }
}

static int __init my_timer_init(void) {
  unsigned int timeout_mks = 1000000 / HZ;
  pr_info("Модуль загружен. HZ=%d (тик каждые %u.%03u мс)\n", HZ,
          timeout_mks / 1000, timeout_mks % 1000);

  timer_setup(&g_timer, timer_callback, 0);

  start_jiffies = jiffies;
  target_time_finish = jiffies + msecs_to_jiffies(MAX_MINUTES * 60000);
  unsigned long target_time = start_jiffies + msecs_to_jiffies(TIMER_INTERVAL_MS * tick_count);
  mod_timer(&g_timer, target_time);

  return 0;
}

static void __exit my_timer_exit(void) {
  timer_shutdown_sync(&g_timer);
  pr_info("Timer module unloaded.\n");
}

module_init(my_timer_init);
module_exit(my_timer_exit);
