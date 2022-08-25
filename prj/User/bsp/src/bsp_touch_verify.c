#include "bsp.h"

#define TOUCH_VERIFY_ADDR      0xFFFFFFFF /* ָ��ģ���Ĭ�ϵ�ַ */

#define FRAME_HRADER_H         0xEF /* ��ͷ */
#define FRAME_HRADER_L         0x01

#define ANALYSIS_BUF_SIZE      256
static uint8_t analysis_buf[ANALYSIS_BUF_SIZE] = {0};

#define TV_RX_FRAME_SIZE_MIN   (9+2)

/*
	addr      ��ַĬ��0xFFFFFFFF
	����ʶ     01�������  02�����ݰ������к�����  08�����һ�����ݰ�����������
	���е����ݰ���Ҫ�Ӱ�ͷ��0xEF01
*/
void tv_Send(uint32_t addr, uint8_t identification, uint16_t len, uint8_t data[])
{
	uint8_t i = 0 ;
	uint32_t chk_sum = 0 ;
	uint8_t buf[128];
	memset(buf, 0, sizeof(buf));


	/* ���Ͱ�ͷ */
	buf[0] = FRAME_HRADER_H;
	buf[1] = FRAME_HRADER_L;

	/* ���͵�ַ */
	buf[2] = (uint8_t)((addr>>24)&0xFF);
	buf[3] = (uint8_t)((addr>>16)&0xFF);
	buf[4] = (uint8_t)((addr>>8 )&0xFF);
	buf[5] = (uint8_t)((addr    )&0xFF);

	/* ���Ͱ���ʶ */
	buf[6] = identification;
	chk_sum += buf[6];

	/* ���ͳ��� */
	buf[7] = (uint8_t)((len>>8 )&0xFF);
	buf[8] = (uint8_t)((len    )&0xFF);

	chk_sum += buf[7];
	chk_sum += buf[8];

	/* ���ݲ��֣�����ʱ���ݲ��ֵĳ��Ⱥ�У��͵ĳ���ȫ�� */
	for(i=0;i<len - 2;i++)
	{
		buf[9+i] = data[i];
		chk_sum += data[i];
	}

	/* У��� */
	buf[9+i] = (uint8_t)((chk_sum>>8 )&0xFF);
	buf[9+i+1] = (uint8_t)((chk_sum  )&0xFF);
	
	/* ����Ӳ������ */
	comSendBuf(COM2, buf, 9+len);
}




void tv_GetEcho(void)
{
	uint8_t buf[] = {0x53};
	tv_Send(TOUCH_VERIFY_ADDR, 0x01, 0x03, buf);
}

/* ����ģ���ϴ������� */
void tv_Analysis(void)
{
	/* �ֽ����� */
    static uint16_t index = 0 ;
	static uint32_t addr = 0 ; /* ��ַ */
	static uint8_t identification = 0 ; /* ��ʶ�� */
	static uint16_t len = 0 ; /* ֡���� */
	static uint8_t ensure_code = 0 ; /* ȷ���� */

    /* �ֶ� */
    uint8_t data = 0 ;

    /* �������� */
    while(comGetChar(COM2,&data))
    {
		printf("%02x ", data);
        analysis_buf[index % ANALYSIS_BUF_SIZE] = data ;
        index++;

        if(index == 1)       /* ֡ͷ ��1��  */
        {
            if(data != FRAME_HRADER_H)
            {
                index = 0 ;
            }
        }
        else if(index == 2)  /* ֡ͷ ��2��  */
        {
            if(data != FRAME_HRADER_L)
            {
                index = 0 ;
            }
        }
        else if(index == 6)  /* ��ַ */
        {
            
            
        }
        else if(index == 7)  /* ��ʶ�� */
        {
            identification = analysis_buf[6];
        }
        else if(index == 9)  /* ���� */
        {
            len = ((uint16_t)analysis_buf[7] << 8) | ((uint16_t)analysis_buf[8]);
        }
        else if(index >= TV_RX_FRAME_SIZE_MIN && index >= 9 + len)  /* ������� */
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
					printf("У��ͨ��\r\n");
				}
			}
			else
			{
				printf("У��ʧ��\r\n");
			}

			index = 0 ;
			addr = 0 ; 
			identification = 0 ; 
			len = 0 ; 
			ensure_code = 0 ; /* ȷ���� */
        }
       
    }
}




/*
	Ӧ���ǽ��й�����ִ����������ϱ������أ�Ӧ������в��������ɸ��������ݰ���
	����ֻ���� �յ�Ӧ��������ȷ���հ������ָ��ִ�����
	����ʶ 07
*/
const char *EnsureMessage(uint8_t ensure) 
{
	const char *p;
	switch(ensure)
	{
		case 0x00: p = "��ʾָ��ִ����ϻ�OK";break;
		case 0x01: p = "��ʾ���ݰ����մ���";break;
		case 0x02: p = "��ʾ��������û����ָ";break;
		case 0x03: p = "��ʾ¼��ָ��ͼ��ʧ��";break;
		case 0x04: p = "��ʾָ��ͼ��̫�ɡ�̫��������������";break;
		case 0x05: p = "��ʾָ��ͼ��̫ʪ��̫��������������";break;
		case 0x06: p = "��ʾָ��ͼ��̫�Ҷ�����������";break;
		case 0x07: p = "��ʾָ��ͼ����������������̫�٣������̫С��������������";break;
		case 0x08: p = "��ʾָ�Ʋ�ƥ��";break;
		case 0x09: p = "��ʾû������ָ��";break;
		case 0x0a: p = "��ʾ�����ϲ�ʧ��";break;
		case 0x0b: p = "��ʾ����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ";break;
		case 0x0c: p = "��ʾ��ָ�ƿ��ģ��������Ч";break;
		case 0x0d: p = "��ʾ�ϴ�����ʧ��";break;
		case 0x0e: p = "��ʾģ�鲻�ܽ��պ������ݰ�";break;
		case 0x0f: p = "��ʾ�ϴ�ͼ��ʧ��";break;
		case 0x10: p = "��ʾɾ��ģ��ʧ��";break;
		case 0x11: p = "��ʾ���ָ�ƿ�ʧ��";break;
		case 0x12: p = "��ʾ���ܽ���͹���״̬";break;
		case 0x13: p = "��ʾ�����ȷ";break;
		case 0x14: p = "��ʾϵͳ��λʧ��";break;
		case 0x15: p = "��ʾ��������û����Чԭʼͼ��������ͼ��";break;
		case 0x17: p = "��ʾ����ָ�ƻ����βɼ�֮����ָû���ƶ���";break;
		case 0x18: p = "��ʾ��дFLASH ����";break;
		case 0x1a: p = "��Ч�Ĵ�����";break;
		case 0x1b: p = "�Ĵ����趨���ݴ����";break;
		case 0x1c: p = "���±�ҳ��ָ������";break;
		case 0x1d: p = "�˿ڲ���ʧ��";break;
		case 0x1e: p = "�Զ�ע�ᣨenroll��ʧ��";break;
		case 0x1f: p = "ָ�ƿ���";break;
		case 0x20: p = "�豸��ַ����";break;
		case 0x21: p = "��������";break;
		case 0x22: p = "ָ��ģ��ǿ�";break;
		case 0x23: p = "ָ��ģ��Ϊ��";break;
		case 0x24: p = "ָ�ƿ�Ϊ��";break;
		case 0x25: p = "¼��������ô���";break;
		case 0x26: p = "��ʱ";break;
		case 0x27: p = "ָ���Ѵ���";break;
		case 0x28: p = "ָ��ģ���й���";break;
		case 0x29: p = "��������ʼ��ʧ��";break;
		case 0x2A: p = "ģ����Ϣ�ǿ�";break;
		case 0x2B: p = "ģ����ϢΪ��";break;
		case 0x33: p = "ͼ�����С";break;
		case 0x34: p = "ͼ�񲻿���";break;
		case 0x35: p = "�Ƿ�����";break;
		case 0x40: p = "ע��������ڹ涨����";break;
		default  : p = "ģ�鷵��ȷ��������";break;
	}
	return p;	
}

