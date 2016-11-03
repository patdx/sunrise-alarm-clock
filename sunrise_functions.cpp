#include "sunrise_functions.h"

// GENERAL
void initDevices() {
  //initLamp();
  initDisplay();
  initClockAndAlarm();
}


enum ControlMode current_control_mode = CONTROL_MAIN;

//////////////////////////////////
// LAMP CONTROL
//////////////////////////////////

const int LAMP_PIN = 9;          // must be 9 or 10 to use Timer1
int lamp_current_brightness = 0; // from 0 to 1023
int lamp_target_brightness = 0;
int lamp_brightness_delta = 0; // number of pwm points per servicing
const int PROGMEM gamma[] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,9,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,13,13,13,13,13,14,14,14,14,15,15,15,15,16,16,16,16,17,17,17,17,18,18,18,18,19,19,19,19,20,20,20,21,21,21,21,22,22,22,22,23,23,23,24,24,24,25,25,25,25,26,26,26,27,27,27,28,28,28,29,29,29,30,30,30,31,31,31,32,32,32,33,33,33,34,34,35,35,35,36,36,36,37,37,37,38,38,39,39,39,40,40,40,41,41,42,42,42,43,43,44,44,44,45,45,46,46,47,47,47,48,48,49,49,50,50,50,51,51,52,52,53,53,53,54,54,55,55,56,56,57,57,58,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,66,67,67,68,68,69,69,70,70,71,71,72,72,73,73,74,74,75,75,76,77,77,78,78,79,79,80,80,81,81,82,83,83,84,84,85,85,86,86,87,88,88,89,89,90,91,91,92,92,93,93,94,95,95,96,96,97,98,98,99,99,100,101,101,102,103,103,104,104,105,106,106,107,108,108,109,109,110,111,111,112,113,113,114,115,115,116,117,117,118,119,119,120,121,121,122,123,123,124,125,125,126,127,127,128,129,130,130,131,132,132,133,134,134,135,136,137,137,138,139,139,140,141,142,142,143,144,145,145,146,147,148,148,149,150,150,151,152,153,154,154,155,156,157,157,158,159,160,160,161,162,163,164,164,165,166,167,167,168,169,170,171,171,172,173,174,175,175,176,177,178,179,180,180,181,182,183,184,185,185,186,187,188,189,190,190,191,192,193,194,195,195,196,197,198,199,200,201,201,202,203,204,205,206,207,208,209,209,210,211,212,213,214,215,216,217,217,218,219,220,221,222,223,224,225,226,227,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,311,312,313,314,315,316,317,318,319,320,321,322,324,325,326,327,328,329,330,331,332,334,335,336,337,338,339,340,341,343,344,345,346,347,348,349,351,352,353,354,355,356,357,359,360,361,362,363,364,366,367,368,369,370,371,373,374,375,376,377,379,380,381,382,383,385,386,387,388,389,391,392,393,394,395,397,398,399,400,402,403,404,405,406,408,409,410,411,413,414,415,416,418,419,420,421,423,424,425,426,428,429,430,431,433,434,435,437,438,439,440,442,443,444,446,447,448,449,451,452,453,455,456,457,459,460,461,463,464,465,466,468,469,470,472,473,474,476,477,478,480,481,482,484,485,487,488,489,491,492,493,495,496,497,499,500,501,503,504,506,507,508,510,511,513,514,515,517,518,519,521,522,524,525,526,528,529,531,532,533,535,536,538,539,541,542,543,545,546,548,549,551,552,553,555,556,558,559,561,562,564,565,566,568,569,571,572,574,575,577,578,580,581,583,584,586,587,589,590,592,593,595,596,597,599,600,602,603,605,606,608,610,611,613,614,616,617,619,620,622,623,625,626,628,629,631,632,634,635,637,639,640,642,643,645,646,648,649,651,653,654,656,657,659,660,662,664,665,667,668,670,671,673,675,676,678,679,681,683,684,686,687,689,691,692,694,695,697,699,700,702,704,705,707,708,710,712,713,715,717,718,720,721,723,725,726,728,730,731,733,735,736,738,740,741,743,745,746,748,750,751,753,755,756,758,760,761,763,765,767,768,770,772,773,775,777,778,780,782,784,785,787,789,790,792,794,796,797,799,801,803,804,806,808,809,811,813,815,816,818,820,822,823,825,827,829,830,832,834,836,838,839,841,843,845,846,848,850,852,854,855,857,859,861,863,864,866,868,870,872,873,875,877,879,881,882,884,886,888,890,892,893,895,897,899,901,903,904,906,908,910,912,914,915,917,919,921,923,925,927,928,930,932,934,936,938,940,942,943,945,947,949,951,953,955,957,959,960,962,964,966,968,970,972,974,976,978,979,981,983,985,987,989,991,993,995,997,999,1001,1004,1007,1010,1012,1014,1016,1018,1019,1020,1021,1022,1023};


void initLamp() {
  // pinMode(LAMP_PIN, OUTPUT);
  // TCCR1A |= _BV(COM1A1);
  // digitalWrite(LAMP_PIN, HIGH); //should enable pullup does this help?

  Timer1.initialize(2000); // 2000 = 500hz, 3333 = 300Hz (At 500 reaching limit
  // of resolution but flicker free)
  // Timer1.start(); //not sure if this is needed

  // So there appears to be some sort of glitch where running pwm() to init the
  // pin causes a LOW signal to be sent for a non-zero amount of time. Boo. From
  // what I gather, I really need to write all this Timer1 code by hand and have
  // a good way of debugging it. Bleurgh. In that case this solution is good
  // enough thank you.

  Timer1.pwm(LAMP_PIN, 0); // init at full brightness
  setLampInstant(1023); // tell my tracking code it's all full brightness
  setLampGentle(0, 5); // init fade out
  //  delay(200);
  // setLampInstant(0);       // sets to 0 brightness
}
void setLampPrivate(int brightness) {
  // this might be excessive but I am explicitly writing this
  // function so I know that the PWM feature isn't being used
  // when at 1023 or 0
  // if (brightness <= 0) {
  //   Timer1.disablePwm(LAMP_PIN);
  //   //Timer1.stop();
  //   digitalWrite(LAMP_PIN, HIGH);
  //   lamp_current_brightness = 0;
  // } else if (brightness >= 1023) {
  //   Timer1.disablePwm(LAMP_PIN);
  //   digitalWrite(LAMP_PIN, LOW);
  //   lamp_current_brightness = 1023;
  //   //Timer1.start();
  // } else {


  if(lamp_current_brightness != brightness){
    //if a difference is detected...then change brightness
	//read our PROGMEM table for adjusted brightness
	int gammabrightness = pgm_read_word(&gamma[brightness]);
    //Timer1.setPwmDuty(LAMP_PIN,1023 - brightness); // this command can only be used after initialized
    Timer1.setPwmDuty(LAMP_PIN,1023 - gammabrightness);
	int last_brightness = lamp_current_brightness;
    lamp_current_brightness = brightness;
    if(brightness == 0 || last_brightness == 0){
      //if lamp WAS off or has just BECOME off then refresh
      //lamp icon on screen
	  updateDisplay();  
    }
  }
}
void serviceLamp() {
  // this function may change
  // given the gamma transformation i want to set up for brightness
  // it may require some tweaks to maximize lower levels of brightness
  // and higher levels of brightness
  // additionally, a potential issue could be processing time
  // if there are not enough cycles to do this in real time (unlikely but indeed
  // possible) then i should approximate next brightness value by linear
  // interpolation. on top of that, once a gamma value table is prepared i may
  // want to set up linear interpolation to have even changes between steps
  // (mostly important for wake up lamp function, say, as those early brightness
  // jumps (note: test!) could be very noticable. )

  if (lamp_brightness_delta != 0) {
    int new_brightness = lamp_current_brightness + lamp_brightness_delta;

    // for positive delta cap at target brightness value
    // failsafe so we don't keep updating if units are not even
    // multiples and it skips over

    // note: assuming that target_brightness has already been error checked
    if (lamp_brightness_delta > 0) {
      // positive delta, so
      // check that not overreached target
      new_brightness = min(new_brightness, lamp_target_brightness);
      // check that not overreached absolute max
      new_brightness = min(new_brightness, 1023);
      } else {
      // negative delta, same thing
      new_brightness = max(new_brightness, lamp_target_brightness);
      new_brightness = max(new_brightness, 0);
    }

    if (new_brightness == 0 || new_brightness == 1023 ||
    new_brightness == lamp_current_brightness) {
      // lamp has reached a stopping point
      // completely off, completely on, or to target level
      // so no more brightness change please
      // reset all values to be nice

      lamp_target_brightness = new_brightness;
      lamp_brightness_delta = 0;
      // setLampPrivate() will change lamp_current_brightness
    }
    setLampPrivate(new_brightness);
  }
}
bool isLampChanging() {
  // wrapped for checking lamp_brightness_data
  // at this point in code it is known that brightness_delta
  // is reset to 0 when target is reached
  return (lamp_brightness_delta != 0);
}
void setLampInstant(int brightness) {
  // public command for user setting of lamp
  // use this not setLampPrivate or else lamp_target_brightness
  // could cause problems
  setLampPrivate(brightness);
  lamp_brightness_delta = 0; // assumed we are now happy with brightness
  lamp_target_brightness = brightness;
}
void setLampGentle(int brightness, int speed) {
  // use of speed TBD
  // TODO: add in interval setter
  // this command can be used to set gradual changes in brightness
  // serviced through updateLamp()? (TBD)
  lamp_target_brightness = brightness;

  if (lamp_target_brightness < lamp_current_brightness) {
    lamp_target_brightness = brightness;
    lamp_brightness_delta = -abs(speed);
    } else if (lamp_target_brightness > lamp_current_brightness) {
    lamp_target_brightness = brightness;
    lamp_brightness_delta = abs(speed);
  }

  // note that is target == current nothing happens

  // serviceLamp(); shouldn't call because this could accelerate the first
  // change
}
void toggleLamp() {
  if (lamp_target_brightness==0){
    setLampGentle(1023,1); //this should be humanspeed
    } else if (lamp_target_brightness>0){
    setLampGentle(0,1);
  }
}

// DISPLAY CONTROL

// init display
//old declaration
//Adafruit_PCD8544 display = Adafruit_PCD8544(A1, A0, 11, 12, 13);
Adafruit_PCD8544 display = Adafruit_PCD8544(A0, A1, A2); //now using hardware SPI


static const unsigned char alarm_icon[] PROGMEM = {0xf8, 0xd8, 0x88, 0x88,
0x88, 0x0,  0xd8};
const int BACKLIGHT_PIN = A3;
const int BACKLIGHT_DEFAULT_BRIGHTNESS = 200;
int8_t display_contrast = 50; //default contrast


void initDisplay() {
  // inits LCD controller and backlight PWM
  // backlight on pin 10

  // LCD Controller
  display.begin();
  display.setContrast(display_contrast); // works for red 5110 breakout@3.3v
  display.clearDisplay();  // clears the screen and buffer
  display.display();       // hides ada logo

  // Backlight PWM
  // pinMode(LAMP_PIN, OUTPUT);
  //setDisplayBacklight(BACKLIGHT_DEFAULT_BRIGHTNESS);
}
void updateDisplay() {
  // eventually logic for
  display.clearDisplay();
  drawAlarmToggleOnDisplay();
  drawTimeOnDisplay();
  display.display();
}
void showMessageOnDisplay(char msg[], int delaytime) {
  // simple mechanism for feedback
  // optional delay before returning
  //(which should be useful for debug messages)
  // this will not call updateDisplay or anything after FYI
  display.clearDisplay();
  display.print(msg);
  display.display();
  if (delaytime > 0) {
    delay(delaytime);
  }
}
void drawAlarmToggleOnDisplay() {
  // DRAW ALARM ON/OFF
  // ALSO SHOWS LAMP ON/OFF INDICATOR ('*')

	int h = Alarm1_hour % 12;
	int m = Alarm1_min;
	bool AM = Alarm1_isAM;
	bool is_alarm_on = Alarm1_isOn;
	bool is_lamp_on = (lamp_current_brightness!=0);

	int total_line_width;

	// Calculate total line width in order to center
	if (is_alarm_on || current_control_mode == CONTROL_SET_ALARM_TIME)
	{
		bool is_lamp_on = (lamp_current_brightness!=0);
		int time_char_count = (h < 10 && h != 0) ? 7 : 8;
		total_line_width = 6 * time_char_count + 3 + 9*(is_lamp_on);
	}
	else if (is_lamp_on)
	{
		total_line_width = 6;
	}
	  
	//disp_x gets offset for centering
	int disp_x = 0.5 * (84 - total_line_width);
	int disp_y = 8;

	// actually draw stuff
	display.setTextSize(1);
	if (is_lamp_on){
		display.setCursor(disp_x, disp_y);
		display.print('*');
		disp_x += 9;
	}
	if ((is_alarm_on || current_control_mode == CONTROL_SET_ALARM_TIME) && !(current_control_mode == CONTROL_SET_ALARM_TIME && second() % 2))
	{
		display.drawBitmap(disp_x, disp_y, alarm_icon, 5, 7, WHITE, BLACK);
		display.setCursor(disp_x + 9, disp_y);
		// display.print("12:30am");
		display.print(h == 0 ? 12 : h); //return 12am if hour is '0'
		display.print(':');
		printTwoDigits(m);
		display.print((AM) ? "am" : "pm");
	}


}
void drawTimeOnDisplay() {
  /* AdaGFX font is 5x7 and it uses 6x8 spacing to keep characters
  * from touching. I want to have the time CENTERED so I need to
  * figure out the req'd spacing for that. Additionally, I may
  * want to figue out a shorter-width colon between HH:MM to
  * maximize pixel usage.
  *
  *     H H : M M : S S
  *     2 2 1 2 2 1 1 1  (TextSize Multiplier)
  *
  * Width = 2  *  4  *  6   +   1  *  4  *  6     = 72
  *         TS    CNT   WIDTH   TS    CNT   WIDTH
  *
  * ACTUALLY NVM THIS. Turns out with small semicolons and full
  * size HH MM SS the width is exactly 84 which fits perfectly.
  */

  // Grab the data
  // int h = hour();
  int h = hourFormat12(); // 12hr format
  int m = minute();
  int s = second();

  if (!(current_control_mode == CONTROL_SET_CLOCK_TIME && s % 2))
  {
	// Set location of time
	int DISP_X = (h < 10) ? 6 : 0;
	const int DISP_Y = 24;

	// Begin GFX Setup
	display.setCursor(DISP_X, DISP_Y);
	display.setTextColor(BLACK);

	// Hours
	display.setTextSize(2);
	display.print(h);

	// Semicolon
	display.setTextSize(1);
	display.print(':');

	// Minutes
	display.setTextSize(2);
	printTwoDigits(m);

	// Semicolon
	display.setTextSize(1);
	display.print(':');

	// Seconds
	display.setTextSize(2);
	printTwoDigits(s);

	// AM/PM
	// here is shoddy code to right-align the symbol:
	display.setCursor(84 - 2 * 6 - DISP_X, DISP_Y + 2 * 8);
	display.setTextSize(1);
	display.print((isAM()) ? "AM" : "PM");
  }
	  
}
void printTwoDigits(int digits) {
  // Adapted from
  // http://www.pjrc.com/teensy/td_libs_TimeAlarms.html
  if (digits < 10) {
    display.print('0');
  }
  display.print(digits);
}
void setDisplayBacklight(int brightness) {
  // initial backlight setting to occur up in init command
  // from 0 to 1023
  Timer1.pwm(BACKLIGHT_PIN, 1023 - brightness);
  // if slowdown occurs, replace command with
  // setPwmDuty(..,..) because it's supposed to be
  // slightly faster
}

/*//////////////////////////////
CLOCK AND ALARM
//////////////////////////////*/

// vars declared in H file...is that okay? hah no I am learning
//AlarmID_t Alarm1_ID;
//time_t Alarm1_time;
int last_sec;
bool Alarm1_isOn = true;
int8_t Alarm1_hour = 8; //hours in 24-time
int8_t Alarm1_min = 0;
bool Alarm1_isAM = true; //nonessential
int Alarm1_duration = 30*60; //in seconds
int Alarm1_timeout = 120*60; //in seconds
bool Alarm1_running_last_cycle = false;

void initClockAndAlarm() {
	//and EEPROM
	
  //setTime(9, 55, 0, 1, 1, 2014); // 12:00:00 1/1/2014
  //setAlarm1(8, 0);            // 12:01:00 alarm
  setSyncProvider(RTC.get);
  last_sec = second();
  if (timeStatus() != timeSet) {
    showMessageOnDisplay("Unable to sync with the RTC", 5000);
    } else {
    showMessageOnDisplay("RTC OK! :)", 500);
  }
  
  if (EEPROM.read(EEPROM_ADDR_CHECK)==101) //101 = EEPROM is setup, change check val to reset settings
  {
	  setAlarm1(EEPROM.read(EEPROM_ADDR_ALARM_HOUR),EEPROM.read(EEPROM_ADDR_ALARM_MIN));
	  display_contrast = EEPROM.read(EEPROM_ADDR_LCD_CONTRAST);
	  display.setContrast(display_contrast);
	  showMessageOnDisplay("EEPROM OK! :)",500);
  }
  else {
	EEPROM.write(EEPROM_ADDR_CHECK,99); //to say it's setup
	EEPROM.write(EEPROM_ADDR_ALARM_HOUR,Alarm1_hour);
	EEPROM.write(EEPROM_ADDR_ALARM_MIN,Alarm1_min);
	EEPROM.write(EEPROM_ADDR_LCD_CONTRAST,display_contrast);
	showMessageOnDisplay("EEPROM was not set up so default config applied.", 5000);
		
  }
  
  
  // TODO: Can't just have logic to make sure it synced. Also need to check if
  // RTC is connected properly but just not set. This is extremely important for
  // debug (based on experience).
  
  //setAlarm1(8,0,true); //set alarm for 8:00am on reset
}
void setAlarm1(int h, int m) {
  //set in 24-hour-time
  Alarm1_hour = h;
  Alarm1_min = m;
  Alarm1_isAM = (h < 12);
}
void toggleAlarm1(){
  Alarm1_isOn = !Alarm1_isOn;
}
bool isAlarm1Running(){
  //true if alarm currently running (ie light is on)
  //or in timeout phase
  //false if alarm has been toggled off
  
  //NOTE: ROLLOVER CURRENTLY NOT HANDLED
  double start = (double)Alarm1_hour*60*60 + (double)Alarm1_min*60;
  double current = (double)hour()*60*60 + (double)minute()*60 + (double)second();
  //double end_duration = (double)start + (double)Alarm1_duration;
  double end_timeout = (double)start + (double)Alarm1_timeout;
  
  return (Alarm1_isOn && (current >= start && current <= end_timeout));
} 
double getAlarm1RunningTime(){
  //returns how long alarm has been running (in seconds)
  //does not error check. should make sure alarm is running
  //first by running isAlarm1Running();
  
  //HOW IT WORKS: Converts everything to minutes from midnight
  //NOTE: This will currently break for durations overlapping midnight
  
  double start = (double)Alarm1_hour*60*60 + (double)Alarm1_min*60;
  double current = (double)hour()*60*60 + (double)minute()*60 + (double)second();
  
  return (current - start);
}
