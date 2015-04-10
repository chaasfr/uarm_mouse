#include <Arduino.h>
#include <robotarm.h>
#include <SoftwareSerial.h>
void setup();
void establishContact();
void loop();
void graber();
void MonitorSerial();
void test();
void MouseControl();
#line 1 "src/MotionDemo.ino"
//#include <robotarm.h>
//#include <SoftwareSerial.h>

robotarm rarm;           
int dataBuf[2] = {0};
int stateMachine = 0, counter=0, check=0;
int  heightTemp  = 0, stretchTemp = 0;
int incoming;
int rxBuf;
int rxBuff;
SoftwareSerial portTwo(6,7); // Timer conflict. The virtual serial disables interrupt
							 // which causes servos to wiggle wiggle wiggle
							 


void setup() 
{
	Serial.begin(9600);
	//~ portTwo.begin(9600);
	establishContact();
	Serial.println("initiating servos...");
	rarm.init();          // attach the servos
	rarm.setPosition(0,0,0);
	Serial.println("initiated");
	delay(500);
	Serial.println("Ready");
}

void establishContact() {
	while (Serial.available() <= 0)
	{
		Serial.println("waiting for processing to start");
		//~ portTwo.println("waiting for processing to start...");
		delay(300);
	}
}


void loop()
{
//	MonitorSerial();
//	test();
	MouseControl();
//	graber();
//	rarm.setPosition(0,0,0);
//	delay(1000);
}

void graber()
{
	incoming=0;
	if (Serial.available())
	{
		incoming = Serial.read();
		if (incoming!=0)
		{
			rarm.grab();
		}
	}
}


void MonitorSerial()
{
	if ( Serial.available())
	{
		byte rxBuff = Serial.read();
		if(check == 0)
		{
			check = rxBuff == 255? 1:0;
			//~ portTwo.print(rxBuff);
		}
		else if(check == 1)
		{
			check = rxBuff == 170? 2:0;
			//~ portTwo.print(" ");
			//~ portTwo.print(rxBuff);
		}	
		else if(check == 2)
		{
			dataBuf[counter++] = rxBuff;
			if(counter > 1)  // receive 4 byte data
			{
				stateMachine = 0;
				counter=0;
				if (dataBuf[0] != 255 && dataBuf[1]!=170){
					//~ portTwo.print(" ");
					//~ portTwo.print(dataBuf[0]);
					//~ portTwo.print(" ");
					//~ portTwo.println(dataBuf[1]);
				}	
			}
		}
	}
}

void test()
{
	rarm.setPosition(0,0,0);
	delay(1000);
	rarm.setAngle(SERVO_R, 90);
	delay(1000);
	rarm.setAngle(SERVO_R, 0);
	delay(1000);
}

void MouseControl()
{
	if ( Serial.available())
	{
		byte rxBuf = Serial.read();
		if(stateMachine == 0)
		{
			stateMachine = rxBuf == 255? 1:0;
		}
		else if(stateMachine == 1)
		{
			stateMachine = rxBuf == 170? 2:0;
		}	
		else if(stateMachine == 2)
		{
			dataBuf[counter++] = rxBuf;
			if(counter > 1)  // receive 4 byte data
			{
					stateMachine = 0;
					counter=0;
					if(dataBuf[0]!=255 && dataBuf[1]!=170)
					{
					if(dataBuf[0]<0) stretchTemp=256+dataBuf[0];
					else stretchTemp=dataBuf[0];
					if(dataBuf[1]<0) heightTemp=256+dataBuf[1]; 
					else heightTemp = dataBuf[1];
					//~ portTwo.print("stetchTemp= ");
					//~ portTwo.print(stretchTemp);
					//~ portTwo.print(" heightTemp= ");
					//~ portTwo.println(heightTemp); 
					//~ rarm.setPosition(stretchTemp, heightTemp, 0);
					//~ rarm.setAngle(SERVO_L,stretchTemp);
					rarm.setAngle(SERVO_R,heightTemp);
					dataBuf[0]=0;
					dataBuf[1]=255;
				}
			}
		}
	}
}
