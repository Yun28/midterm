#include "mbed.h"
#include <stdlib.h>
#include "uLCD_4DGL.h"
BusIn DIN(D3,D4,D5,D6,D7);
RawSerial pc(USBTX, USBRX);
uLCD_4DGL uLCD(D1, D0, D14);

int sended = 0;
char input[5];

void callback()
{
    int i = 0;

    sended = 1;
    // Note: you need to actually read from the serial to clear the RX interrupt
    pc.printf("Loading Waveform ...\r\n");
    //while (i <= 4) {
	if (pc.readable()) {
    		for (i = 0; i < 4; i++) {
	    		input[i] = pc.getc();
    		}
    	}
    //}
    input[4] = '\0';
    pc.printf("Waveform Loaded\r\n");
    sended = 1;
    return;
}

int main(void)
{
  int sum = 0;
  int i, a;
  char buffer[5];

  pc.attach(&callback,Serial::RxIrq);
  while (true)
  {
	a = 0;
	for (i = 3; i>=0; i--) a += DIN[i]*pow(2,i);
	int encode;
	
	uLCD.locate(0,4);
  	uLCD.printf("0b%d\n",a);
	if (a != 0)
	{
		uLCD.locate(0,0);
		sum += a;
		itoa(sum,buffer,2);
  		uLCD.printf("0b%s\n",buffer);
		wait(1);
	}
	if (sended == 1)
	{
		//uLCD.locate(0,6);
  		//uLCD.printf("jkl\n");
		pc.printf("Input: %s\r\n",input);
		pc.printf("Sum Key: %x\r\n",sum);
		for (i = 0; i <= 4; i++)
		{
			//uLCD.locate(0,7);
  			//uLCD.printf("%c",input[i]);
			encode = input[i] ^ ((sum << 3) + 4);
			pc.printf("'%c': %x --> 0x%x\r\n", input[i], input[i], encode);
		}
		sended = 0;
	}
  }
}
