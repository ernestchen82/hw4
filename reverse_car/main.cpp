#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D10), pin6(D13);
BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);

void RPC_reverse(Arguments *in, Reply *out);
RPCFunction rpcRE(&RPC_reverse, "reverse");

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}

void RPC_reverse(Arguments *in, Reply *out)
{
    int d1 = in->getArg<double>();
    int d2 = in->getArg<double>();
    int f = in->getArg<double>();
   if (f == 1)
   {
      car.goStraight(50);
      ThisThread::sleep_for((d1+13)*100);
      car.stop();
      ThisThread::sleep_for(3s);
      car.turn(47,-1);
      ThisThread::sleep_for(1s);
      car.stop();
      ThisThread::sleep_for(3s);
      car.goStraight(50);
      ThisThread::sleep_for((d2+20)*100);
      car.stop();
   }
   else 
   {
      car.goStraight(-50);
      ThisThread::sleep_for((d1+13)*100);
      car.stop();
      ThisThread::sleep_for(3s);
      car.turn(47,-1);
      ThisThread::sleep_for(1s);
      car.stop();
      ThisThread::sleep_for(3s);
      car.goStraight(-50);
      ThisThread::sleep_for((d2+20)*100);
      car.stop();
   }
    
}