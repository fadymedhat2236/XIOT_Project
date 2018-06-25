#include "tm4c123gh6pm.h"
#include "myProject.h"

unsigned char ledOn='0';

void SystemInit(){}

int main()
{
	myfunction(4,1);//testing the function for pushbutton connected to pf4 and led conected to pf1
	__enable_irq();
	while(1)
	{

	}
	return 0;
}