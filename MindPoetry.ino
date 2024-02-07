// Mind Poetry
// Roni Bandini, June 2019
// MIT License

#include <UTFT.h>
#include <Brain.h>
#include <SoftwareSerial.h>

extern uint8_t BigFont[];
UTFT myGLCD(ILI9481,38,39,40,41);

SoftwareSerial softSerial(10, 11);

long myValue=0;

int x, newx;
int y, newy;
int xb, newxb;
int yb, newyb;
int xc, newxc;
int yc, newyc;

int graphSize=60;
int focused;
int strike;
int finished=0;
int poetryLine=70;
int outCounter=0;
int poetryIndex=0;
int poetryEnd=9;
int i;  
int threshold=65;
int signalStrength=25;

#define ARRAYSIZE 25
//String poetry[ARRAYSIZE] = { "La noche se astillo", "de estrellas", "mirandome alucinada","el aire arroja odio","embellecido su rostro","con musica","Pronto nos iremos","arcano sueno","antepasado de mi","sonrisa","el mundo esta demacrado","y hay candado pero no llaves","y hay pavor pero no lagrimas" };

String poetry[ARRAYSIZE] = { "nothing which we are to","perceive in this world equals","the power of your intense","fragility:whose texture","compels me with the colour","of its countries,","rendering death and","forever with each breathing","(i do not know what it is", "about you that closes", "and opens;only something","in me understands","the voice of your eyes","is deeper than all roses)","nobody,not even the rain","has such small hands" };
String title="Somewhere I have, EE Cummings";

//String poetry[ARRAYSIZE] = { "Si te atreves a sorprender","la verdad de esta vieja pared","y sus fisuras, desgarraduras,","formando rostros, esfinges,","manos, clepsidras,","seguramente vendra","una presencia para tu sed,","probablemente partira","esta ausencia que te bebe.","","","","",""};
//String title="Cuarto solo - Pizarnik";


Brain brain(softSerial);

void setup()
{
  
  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);

  // Start the software serial.
  softSerial.begin(9600);

  // Start the hardware serial.
  Serial.begin(9600);

  Serial.println("Mind Poetry started");

  myGLCD.clrScr();
  myGLCD.fillScr(VGA_BLACK);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("Mind Poetry", CENTER, 90);
  myGLCD.print("Roni Bandini", CENTER, 120);
  myGLCD.print("@RoniBandini", CENTER, 150);

  delay(3000);

  myGLCD.clrScr();

  // draw title
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print(title, CENTER, 0);    

  
  // draw horizontal lines
   myGLCD.setColor(VGA_SILVER);
   for (int i=20; i<graphSize+20; i+=15)
   myGLCD.drawLine(0, i, 470, i);  
  
  // graph variables    
  x=0;
  newx=0;
  y=graphSize;
  newy=graphSize;

  xb=0;
  newxb=0;
  yb=graphSize;
  newyb=graphSize;

  xc=0;
  newxc=0;
  yc=graphSize;
  newyc=graphSize;

  focused=0;
  strike=0;
  
}

void loop()
{  
   
    // MindFlex headset strength is ok and data is coming
    if (brain.update()) {             
            
          // for serial monitor
          Serial.println(brain.readCSV());
          Serial.println(brain.readErrors());

       // only when signal is strong 
       if (brain.readSignalQuality()<signalStrength){
                                                   
          // proportional Y for graph          
          newy  =graphSize-((brain.readAttention())*graphSize/120);
          newyb =graphSize-((brain.readMeditation())*graphSize/120);                                        
          newyc =graphSize-(brain.readLowBeta()*graphSize/50000);         

         // determine whether the user is focused
          if ( brain.readAttention()>threshold ){           
            focused=1;                                    
            }                   
         else {            
            focused=0;    
            outCounter++;                                  
            }  

         if (focused==1 && finished==0){
                        // print poetry
                        myGLCD.setColor(VGA_LIME); 
                        myGLCD.print(poetry[poetryIndex], CENTER, poetryLine);
                        poetryIndex=poetryIndex+1;
                        poetryLine=poetryLine+15;                                                                             
          }// focused
         
                    
            
          if (poetryIndex>poetryEnd){
            
              finished=1;
            
              // final screen
              myGLCD.clrScr();

              myGLCD.fillScr(VGA_BLACK);
              myGLCD.setColor(VGA_WHITE);
              myGLCD.print("Mind Poetry finished", CENTER, 90);
              myGLCD.print("Out of focus: "+String(outCounter)+" times", CENTER, 120);
              myGLCD.print("@RoniBandini", CENTER, 150);
              
              poetryIndex=0;
              poetryLine=70;
              finished=0;
              delay(6000);

              myGLCD.clrScr();
              i=0;
              // draw horizontal lines
              myGLCD.setColor(VGA_SILVER);
              for (int i=20; i<graphSize+20; i+=15)
              myGLCD.drawLine(0, i, 470, i); 

              x=0;
              xb=0;
              xc=0;
              
            
            }
                                    
          
          myGLCD.setColor(VGA_LIME);
          myGLCD.drawLine(x,y,newx,newy);

          myGLCD.setColor(VGA_RED);
          myGLCD.drawLine(xb,yb,newxb,newyb);
          
          // add x
          if (x>479){
            x=0;
            }

            if (newx>479){
            // reset x counter and clear screen
            newx=0;
            //myGLCD.clrScr();
             finished=1;
            }

            x=newx;
            newx=newx+10;
            y=newy;

           // increment x
          if (xb>479){
            xb=0;
            }

            if (newxb>479){
            newxb=0;
             finished=1;
            }

            xb=newxb;
            newxb=newxb+10;
            yb=newyb;

          // increment x
          if (xc>479){
            xc=0;
            }

            if (newxc>479){
            newxc=0;
            finished=1;
            }

            xc=newxc;
            newxc=newxc+10;
            yc=newyc;


        delay(2000);

        } // good reading
  
      } // brain update 
    
} // loop
   
     
 
