/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 串行Flash读写例程例程。核心文件为 bsp_spi_flash.c
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2015-08-30 armfly  首发
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */
/* We must always include pt.h in our protothreads code. */
#include "pt.h"

#include <stdio.h> /* For printf(). */
#include "shell.h"

/* Two flags that the two protothread functions use. */
static int protothread1_flag, protothread2_flag;
Shell shell;

/**
 * The first protothread function. A protothread function must always
 * return an integer, but must never explicitly return - returning is
 * performed inside the protothread statements.
 *
 * The protothread function is driven by the main loop further down in
 * the code.
 */
static int
protothread1(struct pt *pt)
{
  /* A protothread function must begin with PT_BEGIN() which takes a
     pointer to a struct pt. */
  PT_BEGIN(pt);

  /* We loop forever here. */
  while(1) {
    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, protothread2_flag != 0);
    shellPrint(&shell, "     1      时间:%s\r\n","2022年8月9日22:51:48");

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
static int
protothread2(struct pt *pt)
{
  PT_BEGIN(pt);

  while(1) {
    /* Let the other protothread run. */
    protothread2_flag = 1;

    /* Wait until the other protothread has set its flag. */
    PT_WAIT_UNTIL(pt, protothread1_flag != 0);
    shellPrint(&shell, "     2      时间:%s\r\n","2022年8月9日22:51:48");
    
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
short shellWrite(char *data, unsigned short len)
{
	comSendBuf(COM1, (uint8_t*)data, len);
    return len;
}


/**
 * @brief ??shell?
 * 
 * @param data ??
 * @param len ????
 * 
 * @return short ?????
 */
short shellRead(char *data, unsigned short len)
{
	if(comGetChar(COM1, (uint8_t*)data))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

char shellBuffer[512];



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

	bsp_Init();		/* 硬件初始化 */
	
	shell.read = shellRead;
	shell.write = shellWrite;
	shellInit(&shell, shellBuffer, 512);
	
	/* Initialize the protothread state variables with PT_INIT(). */
	PT_INIT(&pt1);
	PT_INIT(&pt2);
	
	
	
	
  /*
   * Then we schedule the two protothreads by repeatedly calling their
   * protothread functions and passing a pointer to the protothread
   * state variables as arguments.
   */
	while(1) 
	{
		shellTask(&shell);
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
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func_argv, func_argv, test);

/* func_string 666 'A' "hello world" */
int func_string(int i, char ch, char *str)
{
    printf("input int: %d, char: %c, string: %s\r\n", i, ch, str);
	
	return 200;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), func_string, func_string, test);


/* func_argv "hello world" */
int reboot(int argc, char *agrv[])
{
  shellClear();         /* serial send clear string */
  bsp_DelayMS(100);     /* wait "clear string" send completed */
  __set_FAULTMASK(1);   /* close all isr used for rst immediately*/
  NVIC_SystemReset();   /* reset */
  return 1;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), reboot, reboot, reboot);


