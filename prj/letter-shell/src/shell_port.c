#include "bsp.h" 
#include "shell.h"


static Shell shell;
static char shellBuffer[512];

short shellWrite(char *data, unsigned short len)
{
  comSendBuf(COM1, (uint8_t *)data, len);
  return len;
}


short shellRead(char *data, unsigned short len)
{
  if (comGetChar(COM1, (uint8_t *)data))
  {
    return 1; /* return get bytes cnt */
  }
  else
  {
    return 0;
  }
}


void shellPortInit(void)
{
    shell.read = shellRead;
    shell.write = shellWrite;
    shellInit(&shell, shellBuffer, 512);
}


void shell_loop(void)
{
    shellTask(&shell);
}

