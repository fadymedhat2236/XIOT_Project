#include "tm4c123gh6pm.h"
#include "myProject.h"

unsigned short outputPinNumber;
unsigned short inputPinNumber;
unsigned char ledOn='0';

void portf_init()
{
	SYSCTL_RCGCGPIO_R |=1<<5;
	GPIO_PORTF_LOCK_R=0x4c4f434b;
	GPIO_PORTF_CR_R=0xFF;
	GPIO_PORTF_DIR_R |=(1<<1) | (1<<2) | (1<<3);
	GPIO_PORTF_DIR_R &=~((1<<0) | (1<<4));
	GPIO_PORTF_DEN_R |=(1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4);
	GPIO_PORTF_PUR_R |=(1<<4) | (1<<0);
	
	//INTERRUPT
	if(inputPinNumber==0)
	{
		GPIO_PORTF_IS_R &=~(1<<0);
		GPIO_PORTF_IBE_R &=~(1<<0);
		GPIO_PORTF_IEV_R &=~(1<<0);
		GPIO_PORTF_ICR_R |=(1<<0); //CLEAR FLAG
		GPIO_PORTF_IM_R |=(1<<0);
		NVIC_EN0_R |=(1<<30);
	}
	else
	{
		GPIO_PORTF_IS_R &=~(1<<4);
		GPIO_PORTF_IBE_R &=~(1<<4);
		GPIO_PORTF_IEV_R &=~(1<<4);
		GPIO_PORTF_ICR_R |=(1<<4); //CLEAR FLAG
		GPIO_PORTF_IM_R |=(1<<4);
		NVIC_EN0_R |=(1<<30);
	}
}

void timer0A_init()
{
	SYSCTL_RCGCTIMER_R |=0x01;
	TIMER0_CTL_R&=~0x00000001; //disable timer
	TIMER0_CFG_R=0x00000000; //mode is 32 bit
	TIMER0_TAMR_R=0x00000002;	//periodic mode
	TIMER0_TAMR_R |=0x00000010;
	TIMER0_TAILR_R=48000000-1;
	TIMER0_CTL_R |=0x00000020;
	TIMER0_CTL_R |=0x00000001;
}

void UART0_INIT()
{
	SYSCTL_RCGCGPIO_R |=1<<0;		//ENABLE CLOCK FOR PORTA
	SYSCTL_RCGCUART_R |=1<<0;  	//ENABLE CLOCK FOR UART0
	GPIO_PORTA_AFSEL_R=0x03;
	GPIO_PORTA_PCTL_R |=(1<<0) | (1<<4);
	GPIO_PORTA_DEN_R |=(1<<0) | (1<<1);
	UART0_CTL_R=~1;		//DISABLE UART0
	UART0_IBRD_R=104;
	UART0_FBRD_R=11;
	UART0_LCRH_R =(1<<5) | (1<<6);
	UART0_CTL_R =(1<<0) | (1<<8) | (1<<9);
	UART0_CC_R=0x00;
}

void adc0_init()
{
	int delay;
	SYSCTL_RCGCADC_R |=1<<0;//enable clock for adc0
	delay=SYSCTL_RCGCADC_R;
	delay=SYSCTL_RCGCADC_R;
	delay=SYSCTL_RCGCADC_R;
	ADC0_ACTSS_R &=~0x8;
	ADC0_EMUX_R &=~0xF000;
	ADC0_EMUX_R |=0x5000;
	ADC0_SSMUX3_R=0;
	ADC0_SSCTL3_R |=0X0E;
	ADC0_IM_R |= 1<<3;
	NVIC_EN0_R |=1<<17;
	ADC0_ACTSS_R |=0x8;
	
}

void printChar(char c)
{
	while((UART0_FR_R & (1<<5)) !=0);
	UART0_DR_R=c;
}

void printString(char* s)
{
	while(*s)
	{
		printChar(*(s++));
	}
}

//interrupt handlers
void GPIOF_Handler()
{
	if(inputPinNumber==0)
		GPIO_PORTF_ICR_R |=(1<<0);
	else
		GPIO_PORTF_ICR_R |=(1<<4);
	if(outputPinNumber==1)
		GPIO_PORTF_DATA_R ^=1<<1;
	else if(outputPinNumber==2)
		GPIO_PORTF_DATA_R ^=1<<2;
	else
		GPIO_PORTF_DATA_R ^=1<<3;
	printString("LED ");
	if(ledOn=='0')
	{
		ledOn='1';
		printString("ON");
	}
	else
	{
		ledOn='0';
		printString("OFF");
	}
	printString("\n\r");
}

void ADC0SS3_Handler()
{
	int temperature =(147.5 - (247.5 * ADC0_SSFIFO3_R)/4096.0);
	int tens=(temperature/10);
	int units=temperature%10;
	printChar('0'+tens);
	printChar('0'+units);
	printString(" degree celsius\n\r");
	ADC0_ISC_R|=8;
}

void myfunction(unsigned short input,unsigned short output)
{
	outputPinNumber=output;//setting global pin number
	inputPinNumber=input;
	portf_init();
	timer0A_init();//clock is 16MHZ
	UART0_INIT();
	adc0_init();
}
