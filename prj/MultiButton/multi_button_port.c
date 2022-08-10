#include "multi_button.h"
#include "bsp.h"


enum Button_IDs {
	btn1_id,
	btn2_id,
};

struct Button btn1;
struct Button btn2;

/* 按键口对应的RCC时钟 */
#define RCC_ALL_KEY 	(RCC_APB2Periph_GPIOA)


#define GPIO_PORT_K1    GPIOA
#define GPIO_PIN_K1	    GPIO_Pin_5


uint8_t read_button_GPIO(uint8_t button_id)
{
	switch(button_id)
	{
		case btn1_id:
			return GPIO_ReadInputDataBit(GPIO_PORT_K1, GPIO_PIN_K1);
		case btn2_id:
			return GPIO_ReadInputDataBit(GPIO_PORT_K1, GPIO_PIN_K1);
		default:
			return 0;
	}
}



void BTN1_PRESS_DOWN_Handler(void* btn)
{
	printf("BTN1_PRESS_DOWN_Handler\r\n");
}

void BTN1_PRESS_UP_Handler(void* btn)
{
	printf("BTN1_PRESS_UP_Handler\r\n");
}

void BTN1_PRESS_REPEAT_Handler(void* btn)
{
	printf("BTN1_PRESS_REPEAT_Handler\r\n");
}

void BTN1_SINGLE_Click_Handler(void* btn)
{
	printf("BTN1_SINGLE_Click_Handler\r\n");
}

void BTN1_DOUBLE_Click_Handler(void* btn)
{
	printf("BTN1_DOUBLE_Click_Handler\r\n");
}

void BTN1_LONG_PRESS_START_Handler(void* btn)
{
	printf("BTN1_LONG_PRESS_START_Handler\r\n");
}

void BTN1_LONG_PRESS_HOLD_Handler(void* btn)
{
	printf("BTN1_LONG_PRESS_HOLD_Handler\r\n");
}

void button_port_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_ALL_KEY, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	
	button_init(&btn1, read_button_GPIO, 0, btn1_id);

	button_attach(&btn1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
	button_attach(&btn1, PRESS_UP,         BTN1_PRESS_UP_Handler);
	button_attach(&btn1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler);
	button_attach(&btn1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);
	button_attach(&btn1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);
	button_attach(&btn1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler);

	button_start(&btn1);

}



