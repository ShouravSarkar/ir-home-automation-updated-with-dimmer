#include "IRremote.h"
#include <TimerOne.h>     

int receiver = 9; 
IRrecv irrecv(receiver);           
decode_results results;            

#define load1 A5
#define load2 A4
#define load3 A3
#define load4 A2
#define load5 A1
#define load6 A0
#define ac_load 12
int resetPin = 4;


int a=0,b=0,c=0,d=0,e=0,f=0,g=0,n=0;

      
volatile int i=0,j=0;               // Variable to use as a counter
volatile boolean zero_cross=0,zero_cross_2=0;  // Boolean to store a "switch" to tell us if we have crossed zero
int AC_pin = 5;                 // Output to Opto Triac
int AC_pin2 = 6;

int dim2 = 128;                   // led control
int dim = 128;                  // Dimming level (0-128)  0 = on, 128 = 0ff                  
int freqStep = 75;    // This is the delay-per-brightness step in microseconds.

void setup() { 
  Serial.begin(9600);
  digitalWrite(resetPin, HIGH);  
  irrecv.enableIRIn(); // Start the IR receiver (classic remote)
  pinMode(AC_pin, OUTPUT);                          // Set the Triac pin as output
  pinMode(AC_pin2, OUTPUT);
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  attachInterrupt(1, zero_cross_detect_2, RISING);
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep); 
  pinMode(load1, OUTPUT);
  pinMode(load2, OUTPUT);
  pinMode(load3, OUTPUT);
  pinMode(load4, OUTPUT);
  pinMode(load5, OUTPUT);
  pinMode(load6, OUTPUT);
  pinMode(ac_load, OUTPUT);
  digitalWrite(13,HIGH);   
   pinMode(resetPin, OUTPUT);  
}

void zero_cross_detect() 
{    
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i=0;

  digitalWrite(AC_pin, LOW);

}
void zero_cross_detect_2() 
{    
  zero_cross_2 = true;               // set the boolean to true to tell our dimming function that a zero cross has occured

  j=0;
  
  digitalWrite(AC_pin2, LOW);
}                                      
// Turn on the TRIAC at the appropriate time

void dim_check() 
{                   
  if(zero_cross == true) {              
    if(i>=dim) {                     
      digitalWrite(AC_pin, HIGH);  // turn on light       
      i=0;  // reset time step counter                         
      //zero_cross=false;    // reset zero cross detection
    } 
  else {
      i++;  // increment time step counter                     
      }
     }
  if(zero_cross_2 == true) {              
    if(j>=dim2) {                     
      digitalWrite(AC_pin2, HIGH);  // turn on light       
      j=0;  // reset time step counter                         
     // zero_cross=false;    // reset zero cross detection
    } 
  else {
      j++;  // increment time step counter                     
      }
     }


}                                      


void translateIR() // takes action based on IR code received
{
  switch(results.value)
  {
  case 33464415: 
  case 551534655:
    {
   
    if (dim<127)  
   {
    dim = dim + 12;
    if (dim>127) 
    {
      dim=128; // in vechiul sketch era 127
    }
    }}
    
    break;

  case 33448095: 
  case 551502015:
    {
    
      {
  if (dim>5)  
  {
     dim = dim - 12;
  if (dim<0) 
    {
      dim=0;  // in vechiul sketch era 1
    } }}}
    break;
  
  
  
  case 33439935: 
  case 551518335 :
    {
   
    if (dim2<127)  
   {
    dim2 = dim2 + 12;
    if (dim2>127) 
    {
      dim2=128; // in vechiul sketch era 127
    }
    }}
    
    break;

  case 33472575: 
  case 551485695:
 
    {
    
      {
  if (dim2>5)  
  {
     dim2 = dim2 - 12;
  if (dim2<0) 
    {
      dim2=0;  // in vechiul sketch era 1
    } }}}
    break;

  case 33444015  : //for load1
  case 551520375:
     {
        a++;
        int x=a%2;
        //Serial.println(x);
        digitalWrite(load1, x);
     }
   break;

  case 33478695 : // for load2
  case 551504055:
    {
      b++;
        int x=b%2;
        digitalWrite(load2, x); 
    }
   break;
   case 33486855 :// for load3
   case  551536695:
    {
      c++;
        int x=c%2;
        digitalWrite(load3, x); 
    }
   break;
   case 33435855 : // for load4
   case 551495895:
    {
      d++;
        int x=d%2;
        digitalWrite(load4, x); 
    }
   break;
    case 33468495 : // for load5
    case 551528535:
    {
      e++;
        int x=e%2;
        digitalWrite(load5, x); 
    }
   break;
    case 33452175 : // for load6
    case 551512215:
    {
      f++;
        int x=f%2;
        digitalWrite(load6, x); 
    }
   break;
   case 33423615 : // for load7
   case 551544855:
    {
      g++;
        int x=g%2;
        digitalWrite(ac_load, x); 
    }
   break;
    
    
    case 33441975 :
    case 551489775:
    {
      n++;
        int x=0;
        digitalWrite(load1, x);
        digitalWrite(load2, x);
        digitalWrite(load3, x);
        digitalWrite(load4, x);
        digitalWrite(load5, x);
        digitalWrite(load6, x);
        digitalWrite(ac_load, x);
        digitalWrite(resetPin, LOW);
        
    }
    break;
  }}
  
void loop() {  
 if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
     // receive the next value
    Serial.println(results.value,DEC);
     delay(100);
    
     irrecv.resume(); // Receive the next value
     //delay(100);
   }
  }  
