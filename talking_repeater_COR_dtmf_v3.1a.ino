//Talking Repeater COR design for Motorola Radius or Maxtrac
//Samuel Rebosura
  void(* resetFunc) (void) = 0; //software reset vector
  #include <DFRobotDFPlayerMini.h>
  #include "Arduino.h"
  #include <SoftwareSerial.h>
  #include <DS3231.h>
  #include <DTMF.h>
// Init the DS3231 using the hardware interface
  DS3231 rtc(SDA, SCL); // RTC connected to Analog I/O A4,A5
  #define rxPin   6  // Serial input DF player
  #define txPin   7  // Serial output DF player
 
// set up a new serial port
    SoftwareSerial mySoftwareSerial(6, 7); // RX, TX
    DFRobotDFPlayerMini myDFPlayer; //set Serial for DFPlayer-mini mp3 module 
    const int rx_signal =  10;
    const int tx_control =  11;
    const int tone_out = 12;
    const int trigger_led =  13;
    int rx_active = 1;
    int tone_signal = 0;
    int tone_ready = 0;
    int tx_hangtime = 500; //TX hangtime in milli secs
    long TimeLastCheck = millis();
    long trigger_timeout = 90;//repeater time out variable in secs
    long TimeoutTime = 0;
    long time_var = 0;
    int trigger_loop = 0;
    int tone_enable = 1;
    int dtmf_detected = 0;
    int tx_enable = 1;
    int voice_enable = 1;
    
  int sensorPin = A0;
  int dtmf_led = 9;
  char thischar;
  float n=128.0;
  // sampling rate in Hz
  float sampling_rate=8926.0;

// Instantiate the dtmf library with the number of samples to be taken
// and the sampling rate.
DTMF dtmf = DTMF(n,sampling_rate);
float d_mags[8];
String readString;
    
void setup() {
  // define pin modes emic
  delay(3000);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT); 
  // set the data rate for the SoftwareSerial port
  mySoftwareSerial.begin (9600);
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(TUESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(23, 55, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(16, 7, 2019);   // Set the date to January 1st, 2014 
 
  Serial.begin(9600);
  pinMode(tx_control,OUTPUT);
  pinMode(trigger_led, OUTPUT);
  pinMode(rx_signal, INPUT_PULLUP);
  pinMode(dtmf_led, OUTPUT); 
  digitalWrite(tx_control,LOW);
  digitalWrite(trigger_led,LOW);
  Serial.println();
  Serial.println(F("DFPlayer"));
  Serial.println(F("Initializing"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Re-check the connection!"));
    Serial.println(F("2.Insert the SD card!"));
    //while(true);
    int tone_enable = 0;
    int voice_enable = 0;
  }
  Serial.println(F("DFPlayer ready"));
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30
  Serial.println("COR Ready!");
  Serial.println(rtc.getDateStr());
  Serial.println(rtc.getTimeStr());
  Serial.println(rtc.getDOWStr());
  delay(3000);
  tx_on();
  myDFPlayer.play(96);
  delay(5000);
  myDFPlayer.play(77); 
  delay(1000);
  tx_off();
}

void loop() {
  
  if (trigger_loop <=74)  {
  dtmf_decode();
  trigger_control();
 
  }
  if (trigger_loop == 75)  {
  trigger_loop = 0;
  }
  check_time();
}

void trigger_control() {
  if (digitalRead(rx_signal) == 0 && tone_ready == 1)  {
    check_timeout();
    return;
  }
  if (digitalRead(rx_signal) == 0 && tx_enable == 1)  {
    tx_on();
    Serial.println("RX detected");
    //dtmf_decode();
    tone_ready = 1;
    set_timeout();       
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 1)  {
    if (trigger_loop == 25)  {
    delay(250);
    myDFPlayer.play(62);
    delay(2000);
    }
    if (trigger_loop == 49 && voice_enable == 1)  {
    delay(150);
    time_check();
    }
    if (trigger_loop == 74 && voice_enable == 1)  {
    delay(150);
    myDFPlayer.play(96);
    delay(4000);
    }
    delay(500);
    if (tone_enable == 1) {
    send_tone();
    }
    if (tone_enable == 0) {
    delay(600);
    tone_ready = 0;
    }
    if (tone_enable == 2) {
    tone4();
    }
    if (tone_enable == 3) {
    tone2();
    }
    if (tone_enable == 4) {
    tone1();
    }
    if (tone_enable == 5) {
    tone7();
    }
    delay(100);
      if ( dtmf_detected == 1)  {
      delay(250);
      tx_on();
      dtmf_detected = 0;
      myDFPlayer.play(95);
      delay(2500);
      if (tone_enable == 0) {
      myDFPlayer.play(98);
      delay(1500);
      }
      if (tone_enable == 1) {
      myDFPlayer.play(97);
      delay(1500);
      }
      if (tone_enable == 2) {
      myDFPlayer.play(94);
      delay(1500);
      myDFPlayer.play(99);
      delay(1500);
      myDFPlayer.play(1);
      delay(750);
      }
      if (tone_enable == 3) {
      myDFPlayer.play(94);
      delay(1500);
      myDFPlayer.play(99);
      delay(1500);
      myDFPlayer.play(2);
      delay(750);
      }
      if (tone_enable == 4) {
      myDFPlayer.play(94);
      delay(1500);
      myDFPlayer.play(99);
      delay(1500);
      myDFPlayer.play(3);
      delay(750);
      }
      
      }
      
     if ( dtmf_detected == 2)  {
      delay(250);
      tx_on();
      dtmf_detected = 0;
      myDFPlayer.play(95);
      delay(2500);
      if (tx_enable == 0) {
       myDFPlayer.play(56);
      delay(2000);
      }
      if (tx_enable == 1) {
       myDFPlayer.play(55);
      delay(2000);
     }
     delay(250);
     }
    tx_off();
    readString="";
    trigger_loop = trigger_loop+1;
    //Serial.println(trigger_loop);
    tone_signal = tone_signal+1;
    if (tone_signal == 8) {
      tone_signal = 0;
    }
   
    set_timeout();
   } 
  //clear DTMF String on RX stanby / TX is off 
  if (digitalRead(rx_signal) == 0 && tx_enable == 0)  {
    Serial.println("RX detected");
    Serial.println("TX disabled!");
     if ( dtmf_detected == 2)  {
      delay(250);
      if (tx_enable == 1) {
        tx_on();
        myDFPlayer.play(95);
        delay(2500);
        myDFPlayer.play(55);
        delay(2500);
        tx_off();
     }
     dtmf_detected == 0;
     tone_ready == 0;
     }     
  }
  if (digitalRead(rx_signal) == 1 && tone_ready == 0)  {
    return;        
  } 
}


void send_tone() {
 switch (tone_signal) {
      case 0:
        tone0();
        break;
      case 1:
        tone1();
        break;
      case 2:
        tone2();
        break;
      case 3:
        tone3();
        break;
      case 4:
        tone4();
        break;
      case 5:
        tone5();
        break;
      case 6:
        tone6();
        break;
      case 7:
        tone7();
        break;
 }
}

void tone0() {
      myDFPlayer.play(76);
      delay(750);
      tone_ready = 0;
      //Serial.println("Tone_0");    
}
void tone1() {
     myDFPlayer.play(67);
      delay(750);
      tone_ready = 0;
      //Serial.println("Tone_1");
}
void tone2() {
      myDFPlayer.play(68);
      delay(750);
      tone_ready = 0;
      //Serial.println("Tone_2");
}
void tone3() {
     myDFPlayer.play(70);
     delay(750);
     tone_ready = 0;
      //Serial.println("Tone_3");
}
void tone4() {
      myDFPlayer.play(72);
      delay(650);
      tone_ready = 0;
      //Serial.println("Tone_4");    
}
void tone5() {
      myDFPlayer.play(85);
      delay(750);
      tone_ready = 0;
      //Serial.println("Tone_5");
}
void tone6() {
      myDFPlayer.play(71);
      delay(650);
      tone_ready = 0;
      //Serial.println("Tone_6");
}
void tone7() {
      myDFPlayer.play(64);
      delay(750);
      tone_ready = 0;
      //Serial.println("Tone_7");
}
void tone8() {
      myDFPlayer.play(84);
      delay(750);
      tone_ready = 0;
      
}
 void set_timeout() {
   
    TimeoutTime = trigger_timeout*1000;
    TimeLastCheck = millis();
    //Serial.println("Set_time_out");
             
  }

  void check_timeout() {
    TimeoutTime -= ((millis() - TimeLastCheck));
    TimeLastCheck = millis();
    //Serial.println(TimeoutTime);
    if (TimeoutTime <= 0) {
      delay(640);
      
      myDFPlayer.play(84);//warning beep
      delay(750);
      myDFPlayer.play(56);//say repeater timeout
      delay(2500);
      tx_off();
      delay(20000);
      }
    }

  void dtmf_decode()  {

      /* while(1) */dtmf.sample(sensorPin);
 
  dtmf.detect(d_mags,512);

  thischar = dtmf.button(d_mags,1800.);
  if(thischar) {
    Serial.println(thischar);
    
    char C = thischar;
    digitalWrite(dtmf_led,HIGH);
    delay(150);
    digitalWrite(dtmf_led,LOW);
    readString += C;
    if (readString == "#") {
      digitalWrite(tx_control,LOW);
      digitalWrite(trigger_led,LOW);
    }
  }
  if (readString.length() == 5) {
    if (readString == "#6470") {//Beep Select 0-4
       tone_enable = 0;
       //Serial.println("tone off");
       dtmf_detected = 1;
    }
    if (readString == "#6471") {
       tone_enable = 1;
       trigger_loop = 0;
       //Serial.println("tone on");
       dtmf_detected = 1;
    }
    if (readString == "#6472") {
       tone_enable = 2;
       //Serial.println("tone4 on");
       dtmf_detected = 1;
    }
    if (readString == "#6473") {
       tone_enable = 3;
       //Serial.println("tone2 on");
       dtmf_detected = 1;
    }
    if (readString == "#6474") {
       tone_enable = 4;
       //Serial.println("tone1 on");
       dtmf_detected = 1;
    }
    if (readString == "#6475") {
       tone_enable = 5;
       //Serial.println("tone7 on");
       dtmf_detected = 1;
    }
    if (readString == "#7530") {
       tx_enable = 0;
       //Serial.println("tx off");
       dtmf_detected = 2;
    }
    if (readString == "#7531") {
       tx_enable = 1;
       //Serial.println("tx on");
       dtmf_detected = 2;
    }
    if (readString == "#1234") {
       //Serial.println("TimeCheck");
       say_time();
    }
    if (readString == "#5678") {
       //Serial.println("Date");
       say_date();   
    }
    if (readString == "#4321") {//Welcome Club ID 1
      tx_on();
       delay (1000);
       myDFPlayer.play(93);
       delay(4500); 
       tx_off();
    }
    if (readString == "#5930") {//Club ID2
      tx_on();
       delay (1000);
       myDFPlayer.play(60);
       delay(6000);
       tx_off();
    }
    if (readString == "#5931") {//Club ID3
      tx_on();
       delay (1000);
       myDFPlayer.play(61);
      delay(5000);
      tx_off();
    }
    if (readString == "#5932") {//Club ID3
      tx_on();
       delay (1000);
       myDFPlayer.play(62);
      delay(4000);
      tx_off();
    }
    if (readString == "#5933") {//Club ID3
       tx_on();
       delay (1000);
       myDFPlayer.play(92);
      delay(5000);
      tx_off();
    }
    if (readString == "#5934") {//ID
       tx_on();
       delay (1000);
       myDFPlayer.play(96);
      delay(5000);
      tx_off();
    }
    if (readString == "#1590") {//auto ID disable
       tx_on();
       delay (1000);
       voice_enable = 0;
       myDFPlayer.play(79);
       delay(2000);
       tx_off();
    }  
    if (readString == "#1591") {//auto ID enabled by default
       tx_on();
       delay (1000);
       voice_enable = 1;
       myDFPlayer.play(79);
       delay(2000);
       tx_off();
    }  
    if (readString == "#1179") {
      tx_on();
      tx_enable = 1;
       //Serial.println("reboot");
       delay(640);
      myDFPlayer.play(79);
      delay(2000);
      tx_off();
      resetFunc();
    }
    
  Serial.println(readString);

  readString="";
  }
  //Serial.println("DTMF listening ...."); 
    }

 void voice_out(int msg) 
{
    tx_on();
    delay(150);
    myDFPlayer.play(msg);
    delay(4000);
    tx_off();
}
void say_date() 
{
  tx_on();
  delay(500);
  myDFPlayer.play(85);
  delay (650);
  myDFPlayer.play(53);
  delay (1100);
  String today = rtc.getDOWStr();
  delay (200);
  if (today == "Monday") {
    myDFPlayer.play(41);
    delay(1000);       
    }
  else if (today == "Tuesday") {
    myDFPlayer.play(42);
    delay(1000);       
    }
  else if (today == "Wednesday") {
    myDFPlayer.play(43);
    delay(1000);       
    }
   else if (today == "Thursday") {
    myDFPlayer.play(44);
    delay(1000);       
    }
   else if (today == "Friday") {
    myDFPlayer.play(45);
    delay(1000);       
    }
   else if (today == "Saturday") {
    myDFPlayer.play(46);
    delay(1000);       
    }
   else if (today == "Sunday") {
    myDFPlayer.play(47);
    delay(1000);       
   }
   
     
  String moString = rtc.getDateStr();
  //Serial.println(moString);
  moString.remove(0,3);
  //Serial.print(moString);
  moString.remove(2);
    if (moString == "01") {
      myDFPlayer.play(29);
      delay(1000);  
    }
    if (moString == "02") {
      myDFPlayer.play(30);
      delay(1000);  
    }  
    if (moString == "03") {
      myDFPlayer.play(31);
      delay(1000);   
    }
    if (moString == "04") {
      myDFPlayer.play(32);
      delay(1000);   
    }
    if (moString == "05") {
      myDFPlayer.play(33);
      delay(1000);    
    }
    if (moString == "06") {
      myDFPlayer.play(34);
      delay(1000);    
    }
    if (moString == "07") {
      myDFPlayer.play(35);
      delay(1000);  
    }
    if (moString == "08") {
      myDFPlayer.play(36);
      delay(1000);   
    }
    if (moString == "09") {
      myDFPlayer.play(37);
      delay(1000);   
    }
    if (moString == "10") {
      myDFPlayer.play(38);
      delay(1000); 
    }
    if (moString == "11") {
      myDFPlayer.play(39);
      delay(1000);    
    }
    if (moString == "12") {
      myDFPlayer.play(40);
      delay(1000);  
    }    
  
  String dtString = rtc.getDateStr();
  dtString.remove(2);
  int dates = dtString.toInt();
  if (dates < 21){myDFPlayer.play(dates); delay(750);}
  else if (dates == 20){myDFPlayer.play(20); delay(650);}
  else if (dates > 20 && dates <30){myDFPlayer.play(20); delay(600);myDFPlayer.play(dates-20); delay(650);}
  else if (dates == 30){myDFPlayer.play(30); delay(650);}
  else if (dates > 30 && dates <40){myDFPlayer.play(21); delay(600);myDFPlayer.play(dates-30); delay(650);}
  delay (500);
  myDFPlayer.play(77);
  delay (650);
  tx_off();
}

void say_time()
{
 tx_on();
 time_check();
 tx_off();
}

void time_check()
{
  delay(750);
  myDFPlayer.play(85);
  delay(1000);
  myDFPlayer.play(87);
  delay(1000);
  String hrString = rtc.getTimeStr();
  hrString.remove(3);
  String minString = rtc.getTimeStr();
  minString.remove(6);
  minString.remove(0,3);
  int mins = minString.toInt();
  int hrs = hrString.toInt();
  if (hrs == 0){myDFPlayer.play(12);delay(750);}
  else if (hrs < 12){myDFPlayer.play(hrs);delay(850);}
  else if (hrs == 12){myDFPlayer.play(12);delay(850);}
  else if (hrs > 12){myDFPlayer.play(hrs-12);delay(850);}
  if (mins == 0){delay(650);}
  else if (mins < 10){myDFPlayer.play(91); delay(850); myDFPlayer.play(mins); delay(850);}
  else if (mins == 10){myDFPlayer.play(10); delay(750);}
  else if (mins > 10 && mins < 20){myDFPlayer.play(mins); delay(850);}
  else if (mins == 20){myDFPlayer.play(20); delay(750);}
  else if (mins > 20 && mins <30){myDFPlayer.play(20); delay(750);myDFPlayer.play(mins-20); delay(750);}
  else if (mins == 30){myDFPlayer.play(30); delay(750);}
  else if (mins > 30 && mins <40){myDFPlayer.play(21); delay(750);myDFPlayer.play(mins-30); delay(750);}
  else if (mins == 40){myDFPlayer.play(40); delay(750);}
  else if (mins > 40 && mins <50){myDFPlayer.play(22); delay(750);myDFPlayer.play(mins-40); delay(750);}
  else if (mins == 50){myDFPlayer.play(50); delay(750);}
  else if (mins > 50 && mins <60){myDFPlayer.play(23); delay(750);myDFPlayer.play(mins-50); delay(750);}
  if (hrs < 12){myDFPlayer.play(89);delay(750);}//say AM
  else if (hrs == 12){myDFPlayer.play(90);delay(750);}//say PM
  else if (hrs > 12){myDFPlayer.play(90);delay(750);}//say PM
  myDFPlayer.play(77);//play bleep
  delay (650);
}
void check_time ()
{
 String hrString = rtc.getTimeStr();
 if (hrString == "06:00:00") { 
  delay(500);
  tx_on();
  myDFPlayer.play(85); //play bleep
  delay(1000);
  myDFPlayer.play(48); //say good morning
  delay(1000);
  myDFPlayer.play(87); //say time is
  delay(1000);
  myDFPlayer.play(6); // say 6
  delay(1100);
   myDFPlayer.play(89); // say AM
  delay(1000);
  tx_off();
 }
 if (hrString == "09:00:00") { 
  delay(500);
  tx_on();
  myDFPlayer.play(85);
  delay(1000);
  myDFPlayer.play(54);
  delay(2500);
  myDFPlayer.play(9);
  delay(1100);
   myDFPlayer.play(89);
  delay(1000);
  tx_off();
 }
 if (hrString == "12:00:00") { 
  delay(500);
  tx_on();
  myDFPlayer.play(85);
  delay(1000);
  myDFPlayer.play(88);
  delay(1000);
  myDFPlayer.play(87);
  delay(1000);
  myDFPlayer.play(12);
  delay(750);
   myDFPlayer.play(52);
  delay(1000);
  tx_off();
 }
 if (hrString == "15:00:00") { 
  delay(500);
  tx_on();
  myDFPlayer.play(85);
  delay(1000);
  myDFPlayer.play(49);
  delay(1000);
  myDFPlayer.play(87);
  delay(1000);
  myDFPlayer.play(3);
  delay(750);
  myDFPlayer.play(90);
  delay(1000);
  tx_off();
 }
 if (hrString == "18:00:00") { 
  delay(500);
  tx_on();
  myDFPlayer.play(85);
  delay(1000);
  myDFPlayer.play(50);
  delay(1000);
  myDFPlayer.play(87);
  delay(1000);
  myDFPlayer.play(6);
  delay(750);
   myDFPlayer.play(90);
  delay(1000);
  tx_off();
 }
 if (hrString == "23:00:00") {
  delay(500); 
  tx_on();
  myDFPlayer.play(85);
  delay(1000);
  myDFPlayer.play(51);
  delay(3000);
  myDFPlayer.play(87);
  delay(1000);
  myDFPlayer.play(11);
  delay(1000);
   myDFPlayer.play(90);
  delay(1000);
  tx_off();
 }
}  

void voice_msg(int msg) //not in use for futute update custom message
{   
  myDFPlayer.play(85);
  delay(1500);
  myDFPlayer.play(msg);
  delay(5000);
}

void tx_on(){//trigger control
  digitalWrite(tx_control,HIGH);
  digitalWrite(trigger_led,HIGH); 
}
void tx_off(){
  delay(tx_hangtime);//hang time 
  digitalWrite(tx_control,LOW);
  digitalWrite(trigger_led,LOW);
}
