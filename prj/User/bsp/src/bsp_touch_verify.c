#include "bsp.h"

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