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

#define TIMER_INTERVAL_MS 30000
#define MAX_MINUTES 5

static struct timer_list g_timer;
static int tick_count = 0;

static void timer_callback(struct timer_list *t) {
  tick_count++;

  int minutes = tick_count / 2;

  if (minutes <= MAX_MINUTES) {
    pr_info("min=%d: Hello, timer!\n", minutes);
    if (minutes < MAX_MINUTES) {
      mod_timer(&g_timer, jiffies + msecs_to_jiffies(TIMER_INTERVAL_MS));
    } else {
      pr_info("Timer stopped: 5 minutes elapsed.\n");
    }
  }
}

static int __init my_timer_init(void) {
  unsigned int timeout_mks = 1000000 / HZ;
  pr_info("Модуль загружен. HZ=%d (тик каждые %u.%03u мс)\n", HZ,
          timeout_mks / 1000, timeout_mks % 1000);

  timer_setup(&g_timer, timer_callback, 0);

  mod_timer(&g_timer, jiffies + msecs_to_jiffies(TIMER_INTERVAL_MS));

  return 0;
}

static void __exit my_timer_exit(void) {
  timer_shutdown_sync(&g_timer);
  pr_info("Timer module unloaded.\n");
}

module_init(my_timer_init);
module_exit(my_timer_exit);
