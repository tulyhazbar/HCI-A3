
#include "neopixel/neopixel.h"
#include "Adafruit_LSM9DS0/Adafruit_Sensor.h"
#include "Adafruit_LSM9DS0/Adafruit_LSM9DS0.h"
#include "math.h"



// IMPORTANT FOR THE LED STRIP : Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D3
#define PIXEL_COUNT 16
#define PIXEL_TYPE WS2812B//****************************



//********************************************************** *******************************************

                                            ///FOR THE SENSOR


/*******************************************************************************************************
 */
/* Assign a unique base ID for this sensor */
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000


#define LSM9DS0_XM_CS 10
#define LSM9DS0_GYRO_CS 9


void displaySensorDetails(void)
{
    

  sensor_t accel, mag, gyro, temp;

  lsm.getSensor(&accel, &mag, &gyro, &temp);
  
  

  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(accel.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(accel.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(accel.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(accel.max_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Min Value:    ")); Serial.print(accel.min_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Resolution:   ")); Serial.print(accel.resolution); Serial.println(F(" m/s^2"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(gyro.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(gyro.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(gyro.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(gyro.max_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Min Value:    ")); Serial.print(gyro.min_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Resolution:   ")); Serial.print(gyro.resolution); Serial.println(F(" rad/s"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);

  
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
//********************************************************** *******************************************

                                            ///FOR THE LED STRIP 


//*******************************************************************************************************



// Pattern types supported:
enum  pattern {  SCANNER };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
    public:

    // Member Variables:  
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern
    
    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position
    
    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
 
    
    void (*OnComplete)();  // Callback on completion of pattern
    
    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
    }
    
    // Update the pattern
    void Update()
    {
        if((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();
            ScannerUpdate();
            
         switch(ActivePattern)
            {
                
              
                case SCANNER:
                  ScannerUpdate();
                    break;
                
                
            }
    
    
    
        }
    }
  
    // Increment the Index and reset at the end
    void Increment()
    {
        if (Direction == FORWARD )
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        
        
         Serial.println("Increment success"); Serial.flush();
    }
    
    // Reverse pattern direction
    
  
    
    
   
    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, uint8_t interval)
    {
        ActivePattern = SCANNER;
        Interval = interval;
        TotalSteps = (numPixels() *2 - 1) ;
        Color1 = color1;
        Index = 0;
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    { 
        for (int i = 0; i < numPixels(); i++)
        {   
            if (i == Index)  // Scan Pixel to the right
            {
                 setPixelColor(i, Color1);
            }
            
            
            else if (i == TotalSteps - Index) // Scan Pixel to the left
            {
                 setPixelColor(i, Color1);
            }
            else // Fading tail
            {
                 setPixelColor(i, DimColor(getPixelColor(i)));
            }
        }
        show();
        Increment();
    }
    
    

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
        // Shift R, G and B components one bit to the right
        uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
        return dimColor;
    }
    

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
        for (int i = 0; i < numPixels(); i++)
        {
            setPixelColor(i, color);
        }
        show();
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
        return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
        return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
        return color & 0xFF;
    }
    
    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
        {
            return Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else
        {
            WheelPos -= 170;
            return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }
};


void stripComplete();

// Define some NeoPatterns for the two rings and the stick


//  as well as some completion routines

NeoPatterns strip( PIXEL_COUNT, PIXEL_PIN , WS2812B, &stripComplete); // maybe it will give an error just plug in the first two numbers

// Initialize everything and prepare to start


  int Index = 0; 

void setup(void)
{
 
    
  //while (!Serial);  // wait for flora/leonardo

  Serial.begin(9600);
  Serial.println(F("LSM9DS0 9DOF Sensor Test")); Serial.println("");

  /* Initialise the sensor */
  if(!lsm.begin())
  {
    /* There was a problem detecting the LSM9DS0 ... check your connections */
    Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
    while(1);
  }
    
  Serial.println(F("Found LSM9DS0 9DOF"));

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Setup the sensor gain and integration time */
  configureSensor();

  /* We're ready to go! */
  Serial.println("");
  

     // Initialize all the pixelStrips
    
    strip.begin();
    
    // Kick off a pattern
    strip.Scanner(strip.Color(255,255,0), 100);
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
int numPixels = 16; 
    
  /* Get a new sensor event */
  sensors_event_t accel, mag, gyro, temp;
  
  //Serial.println("Get accel"); Serial.flush();

  lsm.getEvent(&accel, &mag, &gyro, &temp);
  
  
  // calculating the pitch 
  float pitch = 180* atan2(accel.acceleration.x , sqrt((accel.acceleration.y *accel.acceleration.y ) + (accel.acceleration.z * accel.acceleration.z) ))/3.14159265358979323846264338327950288;
 
  
  
 Serial.print("  pitch: "); Serial.print(pitch);     Serial.println("degree");

  
  


//strip.Update(); 




        
            

    if (pitch >15)  
    
    {
            Index++;
        
                    if (Index >=numPixels)
                    
                                {       
                                         Index = 0; 
                                         strip.setPixelColor(7, 0,0,255);
                                         strip.setPixelColor(8, 0,0,255);
                                         strip.setPixelColor(9, 0,0,255);
                                         strip.setPixelColor(10, 0,0,255);
                                         
                                         
                                         delay(1000);
                                    
                                }
        
     }
     else if  (pitch <15)
        
     {
         
             --Index;
             
                    if (Index<=0)
                    
                                {
                                        Index = numPixels-1 ;  
                                        strip.setPixelColor(7, 255,0,0);
                                         strip.setPixelColor(8, 255,0,0);
                                         strip.setPixelColor(9, 255,0,0);
                                         strip.setPixelColor(10, 255,0,0);
                                         delay(1000);
                                 }
         
         }
         
   
    
        for (int i = 0 ; i<numPixels; i++){
    
                      if (i == Index)  // Scan Pixel to the right
                                {
                                         strip.setPixelColor(i, 0,255,0);
                                }
                        
                      else 
                                 {
                                     
                                         strip.setPixelColor(i, 0,0,0);
                                
                                 }
                                 
                                 
            strip.show();
            delay(20);

}


char publishString[100];
sprintf(publishString,"%f",pitch);
Particle.publish("Side",publishString);
    
}



void stripComplete()
{
  
    // Random color change for next scan
    strip.Color1 = strip.Wheel(random(255));
}


