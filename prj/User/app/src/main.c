#include "bsp.h" /* 底层硬件驱动 */
#include "pt.h"
#include "shell.h"
#include "multi_timer.h"

static struct pt pt1, pt2;
static Timer timer1, timer2;


static int protothread1(struct pt *pt)
{
  PT_BEGIN(pt);

  while (1)
  {

    PT_YIELD(pt);
  }

  PT_END(pt);
}

static int protothread2(struct pt *pt)
{
  PT_BEGIN(pt);

  while (1)
  {

    PT_YIELD(pt);
  }

  PT_END(pt);
}

void cb_timer1(void *arg)
{
  bsp_LedToggle(1);
}

void cb_timer2(void *arg)
{
  printf("touch:%d\r\n",GET_TOUCH_STATE());

}

int main(void)
{
  bsp_Init(); /* 硬件初始化 */
  shellPortInit();

  /* thread */
  PT_INIT(&pt1);
  PT_INIT(&pt2);

  /* timer */
  timer_init(&timer1, cb_timer1, 500, 500, 0);
  timer_init(&timer2, cb_timer2, 500, 1000, 0);
  timer_start(&timer1);
  timer_start(&timer2);

  while (1)
  {
    shell_loop();
    timer_loop();
    protothread1(&pt1);
    protothread2(&pt2);
  }
}

/* func_argv "hello world" */
int func_argv(int argc, char *agrv[])
{
  printf("%dparameter(s)\r\n", argc);
  for (char i = 1; i < argc; i++)
  {
    printf("%s\r\n", agrv[i]);
  }
  return 100;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func_argv, func_argv, test);

/* func_string 666 'A' "hello world" */
int func_string(int i, char ch, char *str)
{
  printf("input int: %d, char: %c, string: %s\r\n", i, ch, str);

  return 200;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), func_string, func_string, test);

/* func_argv "hello world" */
int reboot(int argc, char *agrv[])
{
  shellClear();       /* serial send clear string */
  bsp_DelayMS(100);   /* wait "clear string" send completed */
  __set_FAULTMASK(1); /* close all isr used for rst immediately*/
  NVIC_SystemReset(); /* reset */
  return 1;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), reboot, reboot, reboot);

/* func_argv "hello world" */
int timer_start_2(int argc, char *agrv[])
{
  timer_start(&timer2);
  return 1;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), timer_start_2, timer_start_2, timer_start_2);

// SHELL_EXPORT_VAR(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_VAR_INT), time_cnt, &time_cnt, time_cnt);
