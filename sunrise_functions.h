#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Time.h>
#include <TimerOne.h>

#include <Wire.h>
#include <DS1307RTC.h>

#include <EEPROM.h>

// GENERAL
void initDevices();
enum ControlMode { CONTROL_MAIN, CONTROL_SET_CLOCK_TIME, CONTROL_SET_ALARM_TIME};
extern enum ControlMode current_control_mode;

// LAMP CONTROL
extern const int LAMP_PIN;                   // must be 9 or 10 to use Timer1
extern int lamp_current_brightness; // from 0 to 1023
extern int lamp_target_brightness;
extern int lamp_brightness_delta;
extern const int PROGMEM gamma[];

void initLamp();
void serviceLamp();
void setLampPrivate(int brightness);
bool isLampChanging();
void setLampInstant(int brightness);
void setLampGentle(int brightness, int speed);
void toggleLamp();

// DISPLAY CONTROL
extern Adafruit_PCD8544 display;
extern const int BACKLIGHT_PIN; // must be 9 or 10 to use Timer1
extern const int BACKLIGHT_DEFAULT_BRIGHTNESS;
extern int8_t display_contrast;

void initDisplay();
void updateDisplay();
void showMessageOnDisplay(char msg[], int delaytime);
void drawAlarmToggleOnDisplay();
void drawTimeOnDisplay();
void printTwoDigits(int digits);
void setDisplayBacklight(int brightness);

// CLOCK AND ALARM
//extern AlarmID_t Alarm1_ID;
//extern time_t Alarm1_time;
extern int last_sec;
extern bool Alarm1_isOn;
extern int8_t Alarm1_hour;
extern int8_t Alarm1_min;
extern bool Alarm1_isAM;
extern int Alarm1_duration;
extern int Alarm1_timeout;
extern bool Alarm1_running_last_cycle;
const int EEPROM_ADDR_CHECK = 0;
const int EEPROM_ADDR_ALARM_HOUR = 1;
const int EEPROM_ADDR_ALARM_MIN = 2;
const int EEPROM_ADDR_LCD_CONTRAST = 3;

void initClockAndAlarm(); //inits RTC
void setAlarm1(int h, int m);
void toggleAlarm1();
bool isAlarm1Running();
double getAlarm1RunningTime();
