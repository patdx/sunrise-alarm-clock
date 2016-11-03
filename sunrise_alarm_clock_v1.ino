#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Time.h>
#include <TimerOne.h>

// For RTC:
#include <Wire.h>
#include <DS1307RTC.h>

// For data storage
#include <EEPROM.h>

#include "sunrise_functions.h"

#include <Keypad.h>

const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
byte rowPins[ROWS] = {2, 3, 4, 5}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

long previousMillis = 0;
int interval = 0;

const int backlight_timeout = 15; //time in seconds
int backlight_timer = 0; //


  void setup() {
    // Serial.begin(9600);
    initDevices(); // inits lamp, display, and clock/alarm
  }

  void loop() {
    // eventually this should get replaced with state machine
    // "what screen is showing right now?" logic
    if (last_sec != second()) {
      // runs if at least once second has elapsed
      // note: this could be a buggy implementation

      // showMessageOnDisplay("updating", 100); // debug command
      updateDisplay();
      last_sec = second(); // only update after updateDisplay();
    
    
    //check alarm once per second
    bool Alarm1_running_this_cycle = isAlarm1Running();
    if (Alarm1_running_this_cycle){
      double running_time = getAlarm1RunningTime();
      if (running_time <= Alarm1_duration){
        //alarm is current fading in
        int brightness = 1023.0*((running_time+1)/((double)Alarm1_duration+1.0));
        setLampGentle(brightness, 1);
      } else if (running_time <= Alarm1_timeout){
        setLampGentle(1023, 1);
      }
    }
    if (Alarm1_running_last_cycle && !Alarm1_running_this_cycle){
      //so alarm timeout just occurred or for some reason
      //alarm is not longer running
      //therefore...turn off the light!
      setLampGentle(0,1);  
    }
    Alarm1_running_last_cycle = Alarm1_running_this_cycle;
    
    if (backlight_timer < backlight_timeout){
      setDisplayBacklight(BACKLIGHT_DEFAULT_BRIGHTNESS);
      backlight_timer += 1;
    } else if (backlight_timer == backlight_timeout)
    {
      setDisplayBacklight(0);
      backlight_timer += 1; //so this doesn't get called over and over
    }
    
  }

    // serviceDisplay();

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      serviceLamp();
    }
   
    

    char key = keypad.getKey();
    // char keystr[2] = "x";
    // keystr[0] = key;

    if (key) {
      setDisplayBacklight(BACKLIGHT_DEFAULT_BRIGHTNESS);
      backlight_timer = 0;
      if (key == '0') {
        // LAMP OFF
        //       setLampInstant(0);
        //       updateDisplay();
      }
      if (key == '*') {
        // LAMP TOGGLE
        if (!isAlarm1Running())
        {
          toggleLamp();
          //updateDisplay();
        } else {
          showMessageOnDisplay("Turn off alarm to toggle lamp.",1500);
        }
      }
      if (key == '#') {
        if (isAlarm1Running())
        {
        }
        
        toggleAlarm1();
        updateDisplay();
      }
      if (key == '1') {
		current_control_mode = CONTROL_SET_ALARM_TIME;
      }
      if (key == '2') {
		current_control_mode = CONTROL_SET_CLOCK_TIME;
      }
      if (key == '3') {
		current_control_mode = CONTROL_MAIN;
      }
      if (key == '4') {
		//H-
		if (current_control_mode == CONTROL_SET_ALARM_TIME)
		{
			Alarm1_hour -= 1;
			if (Alarm1_hour == -1)
			{
				Alarm1_hour = 23;
			}
			Alarm1_isAM = (Alarm1_hour < 12);
			EEPROM.write(EEPROM_ADDR_ALARM_HOUR,Alarm1_hour);
		}
		if (current_control_mode == CONTROL_SET_CLOCK_TIME)
		{
			int new_hour = hour()-1;
			if (new_hour == -1)
			{
				new_hour = 23;
			}
			setTime(new_hour, minute(), second(), 1, 1, 2014);
			RTC.set(now());
		}
		updateDisplay();
      }
      if (key == '5') {
		//H+
		if (current_control_mode == CONTROL_SET_ALARM_TIME)
		{
			Alarm1_hour += 1;
			if (Alarm1_hour == 24)
			{
				Alarm1_hour = 0;
			}
			Alarm1_isAM = (Alarm1_hour < 12);
			EEPROM.write(EEPROM_ADDR_ALARM_HOUR,Alarm1_hour);
		}
		if (current_control_mode == CONTROL_SET_CLOCK_TIME)
		{
			int new_hour = hour()+1;
			if (new_hour == 24)
			{
				new_hour = 0;
			}
			setTime(new_hour, minute(), second(), 1, 1, 2014);
			RTC.set(now());
		}
		updateDisplay();
      }
      if (key == '6') {
		// contrast+
		display_contrast++;
		display.setContrast(display_contrast);
		EEPROM.write(EEPROM_ADDR_LCD_CONTRAST,display_contrast);
		display.clearDisplay();
		display.print(display_contrast);
		display.display();
		delay(100);
      }
      if (key == '7') {
		//M-
		if (current_control_mode == CONTROL_SET_ALARM_TIME)
		{
			Alarm1_min -= 5;
			if (Alarm1_min < 0)
			{
				Alarm1_min = 55;
			}
			EEPROM.write(EEPROM_ADDR_ALARM_MIN,Alarm1_min);
		}
		if (current_control_mode == CONTROL_SET_CLOCK_TIME)
		{
			int new_min = minute()-1;
			if (new_min == -1)
			{
				new_min = 59;
			}
			setTime(hour(), new_min, second(), 1, 1, 2014);
			RTC.set(now());
		}
		updateDisplay();
      }
      if (key == '8') {
		//M+
		if (current_control_mode == CONTROL_SET_ALARM_TIME)
		{
			Alarm1_min += 5;
			if (Alarm1_min >= 60)
			{
				Alarm1_min = 0;
			}
			EEPROM.write(EEPROM_ADDR_ALARM_MIN,Alarm1_min);
		}
		if (current_control_mode == CONTROL_SET_CLOCK_TIME)
		{
			int new_min = minute()+1;
			if (new_min == 60)
			{
				new_min = 0;
			}
			setTime(hour(), new_min, second(), 1, 1, 2014);
			RTC.set(now());
		}
		updateDisplay();
      }
      if (key == '9') {
		// contrast-
		display_contrast--;
		display.setContrast(display_contrast);
		EEPROM.write(EEPROM_ADDR_LCD_CONTRAST,display_contrast);
		display.clearDisplay();
		display.print(display_contrast);
		display.display();
		delay(100);
      }
    }



    // DEBUG OPTIONS
    // if (key) {
    //   // showMessageOnDisplay(&key, 200); // this might not be address safe
    //   if (key == '0') {
    //     setLampInstant(0);
    //   }
    //   if (key == '1') {
    //     setLampInstant(1);
    //   }
    //   if (key == '2') {
    //     setLampInstant(2);
    //   }
    //   if (key == '3') {
    //     setLampInstant(3);
    //   }
    //   if (key == '4') {
    //     Timer1.setPeriod(2000);
    //     Timer1.start();
    //     setLampInstant(lamp_current_brightness);
    //     showMessageOnDisplay("2000 uS = 500 Hz", 200);
    //   }
    //   if (key == '5') {
    //     Timer1.setPeriod(3333);
    //     Timer1.start();
    //     setLampInstant(lamp_current_brightness);
    //     showMessageOnDisplay("3333 uS = 300 Hz", 200);
    //   }
    //   if (key == '6') {
    //     Timer1.setPeriod(5000);
    //     Timer1.start();
    //     setLampInstant(lamp_current_brightness);
    //     showMessageOnDisplay("5000 uS = 200 Hz", 200);
    //   }
    //   if (key == '7') {
    //     setLampGentle(0, 1);
    //   }
    //   if (key == '8') {
    //     setLampGentle(100, 1);
    //   }
    //   if (key == '9') {
    //     setLampGentle(1023, 1);
    //   }
    //   if (key == '*') {
    //     setLampInstant(1023);
    //   }
    // }
  }
