#include "bsp.h" /* 底层硬件驱动 */
#include "pt.h"
#include "shell.h"
#include "multi_timer.h"

/* Two flags that the two protothread functions use. */
static int protothread1_flag, protothread2_flag;


static int protothread1(struct pt *pt)
{
  PT_BEGIN(pt);

  while (1)
  {
    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, protothread2_flag != 0);
    printf("     1      时间:%s\r\n", "2022年8月9日22:51:48");

    /* We then reset the other protothread's flag, and set our own
       flag so that the other protothread can run. */
    protothread2_flag = 0;
    protothread1_flag = 1;

    /* And we loop. */
  }

  /* All protothread functions must end with PT_END() which takes a
     pointer to a struct pt. */
  PT_END(pt);
}

/**
 * The second protothread function. This is almost the same as the
 * first one.
 */
static int protothread2(struct pt *pt)
{
  PT_BEGIN(pt);

  while (1)
  {
    /* Let the other protothread run. */
    protothread2_flag = 1;

    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, protothread1_flag != 0);
    printf("     2      时间:%s\r\n", "2022年8月9日22:51:48");

    /* We then reset the other protothread's flag. */
    protothread1_flag = 0;

    /* And we loop. */
  }
  PT_END(pt);
}

/**
 * Finally, we have the main loop. Here is where the protothreads are
 * initialized and scheduled. First, however, we define the
 * protothread state variables pt1 and pt2, which hold the state of
 * the two protothreads.
 */
static struct pt pt1, pt2;

/**
 * @brief ??shell?
 *
 * @param data ??
 * @param len ????
 *
 * @return short ?????????
 */


Timer timer1;
Timer timer2;
int time_cnt = 0;
void cb_timer1(void *arg)
{
  printf("cb_timer1\r\n");
}

void cb_timer2(void *arg)
{
  ++time_cnt;
  // printf("cb_timer2\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
  /*
    ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
  配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM
  */

  bsp_Init(); /* 硬件初始化 */

  shellPortInit();

  /* Initialize the protothread state variables with PT_INIT(). */
  PT_INIT(&pt1);
  PT_INIT(&pt2);

  timer_init(&timer1, cb_timer1, 500, 500, 0);
  timer_init(&timer2, cb_timer2, 500, 1000, 0);

  // timer_start(&timer1);
  // timer_start(&timer2);

  while (1)
  {
    shell_loop();
    timer_loop();
    // protothread1(&pt1);
    // protothread2(&pt2);
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

SHELL_EXPORT_VAR(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_VAR_INT), time_cnt, &time_cnt, time_cnt);
