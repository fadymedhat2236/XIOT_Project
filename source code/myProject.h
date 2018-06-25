#ifndef __myProject_h
#define __myProject_h
//-------------------------------------helper functions---------------------------------
//initialization functions
void portf_init();
void timer0A_init();
void UART0_INIT();
void adc0_init();

//printing functions to the terminal
void printChar(char c);
void printString(char* s);

//interrupt handlers
void GPIOF_Handler();
void ADC0SS3_Handler();

//required function
void myfunction(unsigned short input,unsigned short output);

#endif