#include "bsp.h"

#define TOUCH_VERIFY_ADDR      0xFFFFFFFF /* 指纹模块的默认地址 */

#define FRAME_HRADER_H         0xEF /* 包头 */
#define FRAME_HRADER_L         0x01

#define ANALYSIS_BUF_SIZE      256
static uint8_t analysis_buf[ANALYSIS_BUF_SIZE] = {0};

#define TV_RX_FRAME_SIZE_MIN   (9+2)

/*
	addr      地址默认0xFFFFFFFF
	包标识     01：命令包  02：数据包，且有后续包  08：最后一个数据包，即结束包
	所有的数据包都要加包头：0xEF01
*/
void tv_Send(uint32_t addr, uint8_t identification, uint16_t len, uint8_t data[])
{
	uint8_t i = 0 ;
	uint32_t chk_sum = 0 ;
	uint8_t buf[128];
	memset(buf, 0, sizeof(buf));


	/* 发送包头 */
	buf[0] = FRAME_HRADER_H;
	buf[1] = FRAME_HRADER_L;

	/* 发送地址 */
	buf[2] = (uint8_t)((addr>>24)&0xFF);
	buf[3] = (uint8_t)((addr>>16)&0xFF);
	buf[4] = (uint8_t)((addr>>8 )&0xFF);
	buf[5] = (uint8_t)((addr    )&0xFF);

	/* 发送包标识 */
	buf[6] = identification;
	chk_sum += buf[6];

	/* 发送长度 */
	buf[7] = (uint8_t)((len>>8 )&0xFF);
	buf[8] = (uint8_t)((len    )&0xFF);

	chk_sum += buf[7];
	chk_sum += buf[8];

	/* 数据部分，长度时数据部分的长度和校验和的长度全部 */
	for(i=0;i<len - 2;i++)
	{
		buf[9+i] = data[i];
		chk_sum += data[i];
	}

	/* 校验和 */
	buf[9+i] = (uint8_t)((chk_sum>>8 )&0xFF);
	buf[9+i+1] = (uint8_t)((chk_sum  )&0xFF);
	
	/* 调用硬件发送 */
	comSendBuf(COM2, buf, 9+len);
}




void tv_GetEcho(void)
{
	uint8_t buf[] = {0x53};
	tv_Send(TOUCH_VERIFY_ADDR, 0x01, 0x03, buf);
}

/* 解析模块上传的数据 */
void tv_Analysis(void)
{
	/* 字节索引 */
    static uint16_t index = 0 ;
	static uint32_t addr = 0 ; /* 地址 */
	static uint8_t identification = 0 ; /* 标识符 */
	static uint16_t len = 0 ; /* 帧长度 */
	static uint8_t ensure_code = 0 ; /* 确认码 */

    /* 字段 */
    uint8_t data = 0 ;

    /* 解析数据 */
    while(comGetChar(COM2,&data))
    {
		printf("%02x ", data);
        analysis_buf[index % ANALYSIS_BUF_SIZE] = data ;
        index++;

        if(index == 1)       /* 帧头 第1个  */
        {
            if(data != FRAME_HRADER_H)
            {
                index = 0 ;
            }
        }
        else if(index == 2)  /* 帧头 第2个  */
        {
            if(data != FRAME_HRADER_L)
            {
                index = 0 ;
            }
        }
        else if(index == 6)  /* 地址 */
        {
            
            
        }
        else if(index == 7)  /* 标识符 */
        {
            identification = analysis_buf[6];
        }
        else if(index == 9)  /* 长度 */
        {
            len = ((uint16_t)analysis_buf[7] << 8) | ((uint16_t)analysis_buf[8]);
        }
        else if(index >= TV_RX_FRAME_SIZE_MIN && index >= 9 + len)  /* 接收完毕 */
        {
            uint8_t i = 0 ;
			uint32_t chk_sum = 0 ;

			for(i=0;i<index-6-2;i++)
			{
				chk_sum += analysis_buf[6+i];
			}

			if(((uint8_t)((chk_sum>>8 )&0xFF)==analysis_buf[index-2]) && ((uint8_t)((chk_sum )&0xFF)==analysis_buf[index-1]))
			{
				
				if(identification == 0x07)
				{
					ensure_code = analysis_buf[9];
					printf("ensure_code:%02X \r\n", ensure_code);
				}
				else
				{
					printf("校验通过\r\n");
				}
			}
			else
			{
				printf("校验失败\r\n");
			}

			index = 0 ;
			addr = 0 ; 
			identification = 0 ; 
			len = 0 ; 
			ensure_code = 0 ; /* 确认码 */
        }
       
    }
}




/*
	应答是将有关命令执行情况与结果上报给主控，应答包含有参数，并可跟后续数据包。
	主控只有在 收到应答包后才能确认收包情况与指令执行情况
	包标识 07
*/
const char *EnsureMessage(uint8_t ensure) 
{
	const char *p;
	switch(ensure)
	{
		case 0x00: p = "表示指令执行完毕或OK";break;
		case 0x01: p = "表示数据包接收错误";break;
		case 0x02: p = "表示传感器上没有手指";break;
		case 0x03: p = "表示录入指纹图像失败";break;
		case 0x04: p = "表示指纹图像太干、太淡而生不成特征";break;
		case 0x05: p = "表示指纹图像太湿、太糊而生不成特征";break;
		case 0x06: p = "表示指纹图像太乱而生不成特征";break;
		case 0x07: p = "表示指纹图像正常，但特征点太少（或面积太小）而生不成特征";break;
		case 0x08: p = "表示指纹不匹配";break;
		case 0x09: p = "表示没搜索到指纹";break;
		case 0x0a: p = "表示特征合并失败";break;
		case 0x0b: p = "表示访问指纹库时地址序号超出指纹库范围";break;
		case 0x0c: p = "表示从指纹库读模板出错或无效";break;
		case 0x0d: p = "表示上传特征失败";break;
		case 0x0e: p = "表示模组不能接收后续数据包";break;
		case 0x0f: p = "表示上传图像失败";break;
		case 0x10: p = "表示删除模板失败";break;
		case 0x11: p = "表示清空指纹库失败";break;
		case 0x12: p = "表示不能进入低功耗状态";break;
		case 0x13: p = "表示口令不正确";break;
		case 0x14: p = "表示系统复位失败";break;
		case 0x15: p = "表示缓冲区内没有有效原始图而生不成图像";break;
		case 0x17: p = "表示残留指纹或两次采集之间手指没有移动过";break;
		case 0x18: p = "表示读写FLASH 出错";break;
		case 0x1a: p = "无效寄存器号";break;
		case 0x1b: p = "寄存器设定内容错误号";break;
		case 0x1c: p = "记事本页码指定错误";break;
		case 0x1d: p = "端口操作失败";break;
		case 0x1e: p = "自动注册（enroll）失败";break;
		case 0x1f: p = "指纹库满";break;
		case 0x20: p = "设备地址错误";break;
		case 0x21: p = "密码有误";break;
		case 0x22: p = "指纹模板非空";break;
		case 0x23: p = "指纹模板为空";break;
		case 0x24: p = "指纹库为空";break;
		case 0x25: p = "录入次数设置错误";break;
		case 0x26: p = "超时";break;
		case 0x27: p = "指纹已存在";break;
		case 0x28: p = "指纹模板有关联";break;
		case 0x29: p = "传感器初始化失败";break;
		case 0x2A: p = "模组信息非空";break;
		case 0x2B: p = "模组信息为空";break;
		case 0x33: p = "图像面积小";break;
		case 0x34: p = "图像不可用";break;
		case 0x35: p = "非法数据";break;
		case 0x40: p = "注册次数少于规定次数";break;
		default  : p = "模块返回确认码有误";break;
	}
	return p;	
}

