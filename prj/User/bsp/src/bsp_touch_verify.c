#include "bsp.h"

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