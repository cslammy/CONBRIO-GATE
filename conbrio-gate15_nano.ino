//loaded 11-12-18 6:12PM

//attiny interrupt test
//https://thewanderingengineer.com/2014/08/11/pin-change-interrupts-on-attiny85/

//version 3 basic idea
//version 4 fixed interrupt so it works like a gate
//version 5 get random shit going
//version 6 control interval w' pot
//version 7 cleaned up interrupt gate and general fixes.
//version 8 got most of Attiny going but ran out of pins
//version 9 ported to nano (more pins!)
//version 13 got rid of interrupts, changed gate logic, tweaked pot values.
//version 14 added LED I/O, set delays
//version 15 switched reversi logic (buffer no longer inverts--new hardware)

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



const byte blink=13; // built in LED port on nano
boolean gateon = LOW; // logic used for gated elements LOW or HIGH
int x = 0;
int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;

float upwards = 0;
long randomnumber = 0;
//int printme = 0;
int headsortails = 0;
int interval = 0;  //how long for initial interval read
int interval2 = 0; // use this to control blinking
int speedy = 0; // length between blinks
boolean reverski=HIGH; // do we go fast to slow or slow to fast?
int randomvalue = 15;  // add musicality to your dukey.
int xx = 1;
void setup()
{
    Serial.begin( 9600 );
   
// interrupt on for Nano
  


  pinMode(A1, INPUT); // interval 0 - 300
  pinMode(A3, INPUT); // speedy.  distance between each pulse.
  pinMode(3, INPUT); //reverski.  fast to slow or slow to fast
  pinMode(2, INPUT); // gate in used as INT0
  pinMode(8, OUTPUT); //LED for gate output 
  pinMode(6, OUTPUT); // LED for gate input
  pinMode(7, OUTPUT); // LED for reverski status
  pinMode(blink, OUTPUT); // internal LED on Nano

  
 // attachInterrupt(digitalPinToInterrupt(3), myisr2, HIGH); // reverski signal 
//  attachInterrupt(digitalPinToInterrupt(2), myisr, HIGH); // get gate signal; LOW is TRUE 
}


//"waitaround" lets you avoid delay() stupidity.  220 is about 1 second on a 1Mhz ATTINY85
void waitaround (int x)
{
 for (a = 0;a < x;a++)
  waitaround3(x);
}

void waitaround2 (int x)


{
 for (b = 0;b < x;b++)
 x = x;
}

void waitaround3 (int x)
{
  for (c = 0;c < x;c++)
  waitaround2(x);
}





void loop()

{
delay(10);
reverski = digitalRead(3);
gateon = digitalRead(2);
//Serial.println(reverski);  

//this is getslower routine.

if (reverski == LOW)
    {
      
    digitalWrite(7,LOW); // turn LED **OFF** which means we are in get slower mode
    
    if (xx == 0)
    {
    interval = (analogRead(A1));
    xx = 1;
    }
    //Serial.println("reverski is:");
    //Serial.println(reverski);
       if (gateon==LOW)
       {
         digitalWrite(6,HIGH); // turn on gate in status LED
   //    Serial.println("gateon in getslowweris:");

     //  Serial.println("gateon upwards = interval; is true in getslower loop");
 
   

 /*      Serial.print("interval:");      
       Serial.println(interval);
       Serial.print("speedy is:");
       Serial.println(speedy);

  */      
   
 
        
        headsortails = random(1,2);
        
        randomnumber = random(randomvalue);   
       
         digitalWrite(blink,HIGH);
         digitalWrite(8,HIGH); // lite LED that gate out is on
         digitalWrite(5,HIGH); // Send 5V to output buffer
         
         waitaround(interval* .5);
         digitalWrite(blink,LOW);
         digitalWrite(8,LOW); // un-lite LED that gate out is on
         digitalWrite(5,LOW); // Send GND to output buffer     
                  
                  waitaround(interval* .5);


          speedy = (analogRead(A3)/5);
       
          if (headsortails == 1)
           {
           interval = interval + speedy + randomnumber; 
           }
          else
           {
           interval = interval + speedy;
           }


         } 
         if (gateon==HIGH)
         {
          digitalWrite(blink,LOW);
          digitalWrite(6, LOW); // turn off input status LED
          xx = 0;    
         }

      
   }  // end if reverski


//THIS IS THE GET FASTER ROUTINE.

if (reverski == HIGH)
    {
         digitalWrite(7, HIGH);  // turn reverski LED **ON** which means we are get faster mode.
         
         delay(10);
         if (xx == 1)
            {
              interval2 = (analogRead(A1));             
            }       
         if (gateon==LOW)
            { 
             digitalWrite(6,HIGH); // turn on gate input status LED 
            xx = 0; // don't get a new read for interval2   
 //         Serial.println("gateon is LOW in get faster loop");
         
            headsortails = random(1,2);
            randomnumber = random(randomvalue);   
            waitaround(interval2);   
            digitalWrite(blink,HIGH);
            digitalWrite(8,HIGH); // lite LED that gate out is on
            digitalWrite(5,HIGH); // Send 5V to output buffer
            
            waitaround(interval2);
            digitalWrite(blink,LOW);
                   
            digitalWrite(8,LOW); // un-lite LED that gate out is on
            digitalWrite(5,LOW); // Send gnd to output buffer
            speedy = (analogRead(A3)/20);

            

            
            if (headsortails == 1)
            
                            
                {
                interval2 = interval2 - (speedy-randomnumber);
                }

           
                              
              else
                {
                interval2 = interval2 - (speedy + randomnumber);
                }

               if (interval2 < 5)
               {
                interval2 = 70;
               }
        
              
        } // end while gateon == LOW
        if (gateon==HIGH)
        {
          digitalWrite(blink,LOW);
          digitalWrite(6,LOW); // turn off gate input status LED
          
          xx = 1;
          speedy = 0;
        } // end gateon = high
    } // END IF REVERSKI HIGH

} // end loop ()
