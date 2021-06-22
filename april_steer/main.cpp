#include"mbed.h"
#include "bbcar.h"
#define PI 3.14159265


BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
InterruptIn btnRecord(USER_BUTTON);

Ticker servo_ticker;
PwmOut pin5(D10), pin6(D13);
BBCar car(pin5, pin6, servo_ticker);

Thread thread1;
Thread thread2;
const int MaxBufferSize = 32;
char buffer[MaxBufferSize];
char recv[1];
int  len = 0;
int angle = 180;
int d = 40;
bool record = true;

void command()
{
   while (record)
   {
      if(uart.readable())
      {
         for (int s=0; s < 3; s++)
         {
            uart.read(recv,sizeof(recv));
            buffer[len++] = recv[0];
            buffer[len] = '\0';
         }
         sscanf(buffer, "%d", &angle);
         if (recv[0] == '\n') {
                printf("%d\n" ,angle);
                len = 0;
            }
      }
   }
   if(angle < 170)
   {
      car.turn(47,1);
      ThisThread::sleep_for((angle-90)*10);
      car.stop();
      int move = sin((180-angle)*PI/180)*d;
      car.goStraight(50);
      ThisThread::sleep_for((move+5)*100);
      car.stop();
      car.turn(47,-1);
      ThisThread::sleep_for(1s);
      car.stop();
   }

   if(angle > 190)
   {
      car.turn(50,-1);
      ThisThread::sleep_for((270-angle)*10);
      car.stop();
      int move = sin((angle-180)*PI/180)*d;
      car.goStraight(50);
      ThisThread::sleep_for((move+5)*100);
      car.stop();
      car.turn(42,1);
      ThisThread::sleep_for(1s);
      car.stop();
   }
   
}

void steer()
{
   record = false;
}



int main(){
   uart.set_baud(9600);
   //thread1.start(steer);
   thread2.start(command);
   btnRecord.rise(steer);
}