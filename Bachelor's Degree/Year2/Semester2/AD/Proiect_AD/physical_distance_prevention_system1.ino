#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

// this constant won't change. It's the pin number of the sensor's output:
const int pingPin = 7;
#define NEOPIXELPIN 8
#define BUZZERPIN 13
int ledsNo = 12;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// initialise the strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);


long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  strip.begin();
  pinMode(BUZZERPIN, OUTPUT);
}

long getDuration() {
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  return pulseIn(pingPin, HIGH);
}

char getColor(long centimeters) {
  if (centimeters >= 150) {
    // case GREEN (SAFE)
   	return 0;
  } else if (centimeters >= 100) {
   	// 100 <= centimeters < 150
    
    // case YELLOW (WARNING)
    return 1;
  } else {
   	// centimeters < 100 
    
    // case RED (EMERGENCY/HIGH RISK OF INFECTION)
    return 2;
  }
}

char getNoLeds(long centimeters) {
  if (centimeters >= 150) {
    if (centimeters >= 200) {
    	return 4;  
    } else if (centimeters >= 175) {
      	// 175 <= centimeters < 200
     	return 8; 
    } else {
      	// 150 <= centimeters < 175
     	return 12; 
    }
  } else if (centimeters >= 100) {
    if (centimeters >= 133){
    	return 4;
    } else if (centimeters >= 116) {
      	// 116 <= centimeters < 133
     	return 8; 
    } else {
     	// 100 <= centimeters < 116
    	return 12;
    }
  } else {
   	// centimers < 100
    if (centimeters >= 75) {
      	// 75 <= centimeters < 100
     	return 4; 
    } else if (centimeters >= 50) {
     	// 50 <= centimeters < 75
      	return 8;
    } else {
     	// centimeters < 50
      	return 12;
    }
  }
}

void setLeds(char color, char noLedsOpened) {
  strip.clear();
  for (int crtLed = 0; crtLed < noLedsOpened; crtLed++) {
    if (color == 0) {
      strip.setPixelColor(crtLed, strip.Color(0, 50, 0));
    } else if (color == 1) {
	  strip.setPixelColor(crtLed, strip.Color(50, 50, 0));
    } else {
      // color = 2
      strip.setPixelColor(crtLed, strip.Color(50, 0, 0));
    }
  }
  strip.show();   
}

void printLCD(long cm, char color) {
  if (color == 0) {
    lcd.clear();
	lcd.print("SAFE"); 
    digitalWrite(BUZZERPIN, LOW);
  } else if (color == 1) {
    lcd.clear();
    lcd.print("RAISE DISTANCE!"); 
  	digitalWrite(BUZZERPIN, LOW);
  } else if (color == 2) {
     lcd.clear();
     lcd.print("DANGER!"); 
     digitalWrite(BUZZERPIN, HIGH);
  }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print((float) cm/100);
  lcd.print("m");
}

void loop() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration = getDuration();
 
  long inches, cm;

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  char color = getColor(cm);
  char noLedsOpened = getNoLeds(cm);
  
  setLeds(color, noLedsOpened);
  
  printLCD(cm, color);
  delay(100);
}
 