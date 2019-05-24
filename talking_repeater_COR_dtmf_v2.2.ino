//Talking Repeater COR design for Motorola Radius or Maxtrac
//Samuel Rebosura
  void(* resetFunc) (void) = 0; //software reset vector
  #include <SoftwareSerial.h>
  #include <DS3231.h>
  #include <DTMF.h>
// Init the DS3231 using the hardware interface
  DS3231 rtc(SDA, SCL);
  #define rxPin   6  // Serial input (connects to Emic 2's SOUT pin)
  #define txPin   7  // Serial output (connects to Emic 2's SIN pin)
// set up a new serial port
  SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);
    const int rx_signal =  10;
    const int tx_control =  11;
    const int tone_out = 12;
    const int trigger_led =  13;
    int rx_active = 1;
    int tone_signal = 0;
    int tone_ready = 0;
    long TimeLastCheck = millis();
    long trigger_timeout = 90;
    long TimeoutTime = 0;
    long time_var = 0;
    int trigger_loop = 0;
    int tone_enable = 4;
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
  delay(5000);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT); 
  // set the data rate for the SoftwareSerial port
  emicSerial.begin(9600);
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(TUESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(7, 18, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(20, 3, 2018);   // Set the date to January 1st, 2014 
  //emicSerial.print("N2");
  //emicSerial.print('\n');     // Send a CR in case the system is already up
  emicSerial.print("V16");
  emicSerial.print('\n'); 
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(50);                          // Short delay
  emicSerial.flush();                 // Flush the receive buffer 
  Serial.begin(9600);
  pinMode(tx_control,OUTPUT);
  pinMode(trigger_led, OUTPUT);
  pinMode(rx_signal, INPUT_PULLUP);
  pinMode(dtmf_led, OUTPUT); 
  digitalWrite(tx_control,LOW);
  digitalWrite(trigger_led,LOW);
  Serial.println("Ready!");
  delay(6000);
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
    digitalWrite(tx_control,HIGH);
    digitalWrite(trigger_led,HIGH);
    //Serial.println("RX detected");
    //dtmf_decode();
    tone_ready = 1;
    set_timeout();       
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 1)  {
    if (trigger_loop == 25)  {
    delay(250);
    voice_msg("ARCCO Incorporated.");
    }
    if (trigger_loop == 49 && voice_enable == 1)  {
    delay(150);
    time_check();
    }
    if (trigger_loop == 74 && voice_enable == 1)  {
    delay(250);
    voice_msg("Anteqera Radio and Cellphone Communication Incorporated.");  
    }
    delay(640);
    if (tone_enable == 1) {
    send_tone();
    }
    if (tone_enable == 0) {
    delay(640);
    tone_ready = 0;
    }
    if (tone_enable == 2) {
    tone4();
    }
    if (tone_enable == 3) {
    tone6();
    }
    if (tone_enable == 4) {
    tone7();
    }
    delay(100);
      if ( dtmf_detected == 1)  {
      digitalWrite(tx_control,HIGH);
      digitalWrite(trigger_led,HIGH);
      tone8();
      tone8();
      tone8();
      dtmf_detected = 0;
      voice_msg("Access granted.");
      if (tone_enable == 0) {
      voice_msg("Beep off.");
      }
      if (tone_enable == 1) {
      voice_msg("Beep on.");
      }
      if (tone_enable == 2) {
      voice_msg("Beep option 1.");
      }
      if (tone_enable == 3) {
      voice_msg("Beep option 2.");
      }
      if (tone_enable == 4) {
      voice_msg("Beep option 3.");
      }
      if (tx_enable == 0) {
      voice_msg("Repeater off.");
      }
      if (tx_enable == 1) {
      voice_msg("Repeater is active.");
      }
      delay(250);
     }
    digitalWrite(tx_control,LOW);
    digitalWrite(trigger_led,LOW);
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
  //if (digitalRead(rx_signal) == 0 && tx_enable == 0)  {
   // Serial.println("RX detected");
   // Serial.println("TX disabled!");
    //readString="";
    //dtmf_decode();
    //return;        
  //}
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
      tone(tone_out, 494, 250);
      delay(250);
      noTone(tone_out);
      delay(250);
      tone_ready = 0;
      //Serial.println("Tone_0");    
}
void tone1() {
      tone(tone_out, 880, 250);
      delay(250);
      noTone(tone_out);
      delay(250);
      tone_ready = 0;
      //Serial.println("Tone_1");
}
void tone2() {
      tone(tone_out, 784, 175);
      delay(100);
      tone(tone_out, 1568, 200);
      delay(200);
      tone_ready = 0;
      //Serial.println("Tone_2");
}
void tone3() {
     tone(tone_out, 440, 150);
      delay(100);
      tone(tone_out, 880, 250);
      delay(200);
      //tone(tone_out, 330, 150);
      //delay(200);
      tone_ready = 0;
      //Serial.println("Tone_3");
}
void tone4() {
      tone(tone_out, 330, 250);
      delay(250);
      noTone(tone_out);
      delay(250);
      tone_ready = 0;
      //Serial.println("Tone_4");    
}
void tone5() {
      tone(tone_out, 392, 175);
      delay(150);
      tone(tone_out, 523, 200);
      delay(150);
      tone_ready = 0;
      //Serial.println("Tone_5");
}
void tone6() {
      tone(tone_out, 600, 175);
      delay(100);
      tone(tone_out, 880, 200);
      delay(100);
      tone_ready = 0;
      //Serial.println("Tone_6");
}
void tone7() {
      //tone(tone_out, 960, 250);
      //delay(250);
      //noTone(tone_out);
      //delay(250);
      tone(tone_out, 1000, 25);
      delay(10);
      tone(tone_out, 1100, 25);
      delay(10);
      tone(tone_out, 1200, 25);
      delay(10);
      tone(tone_out, 1100, 25);
      delay(10);
      tone(tone_out, 1000, 25);
      delay(10);
      tone(tone_out, 1000, 25);
      delay(10);
      tone(tone_out, 1100, 25);
      delay(10);
      tone(tone_out, 1200, 25);
      delay(10);
      tone(tone_out, 1100, 25);
      delay(10);
      tone(tone_out, 1000, 25);
      delay(10);
      tone(tone_out, 1100, 25);
      delay(10);
      tone(tone_out, 1200, 25);
      delay(10);
      tone(tone_out, 1100, 25);
      delay(10);
      tone(tone_out, 1000, 25);
      delay(100);
      tone_ready = 0;
      //Serial.println("Tone_7");
}
void tone8() {
      tone(tone_out, 1760, 100);
      delay(100);
      tone(tone_out, 800, 100);
      delay(100);
      tone(tone_out, 540, 100);
      delay(100);
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
      tone8();
      tone8();
      tone8();
      tone8();
      tone8();
      delay(250);
      voice_msg("Timeout");
      digitalWrite(tx_control,LOW);
      digitalWrite(trigger_led,LOW);
      delay(15000);
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
    if (readString == "#6470") {
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
       //Serial.println("tone7 on");
       dtmf_detected = 1;
    }
    if (readString == "#6474") {
       tone_enable = 4;
       //Serial.println("tone1 on");
       dtmf_detected = 1;
    }
    if (readString == "#7530") {
       tx_enable = 0;
       //Serial.println("tx off");
       dtmf_detected = 1;
    }
    if (readString == "#7531") {
       tx_enable = 1;
       //Serial.println("tx on");
       dtmf_detected = 1;
    }
    if (readString == "#1234") {
       //Serial.println("TimeCheck");
       say_time();
    }
    if (readString == "#5678") {
       //Serial.println("Date");
       say_date();   
    }
    if (readString == "#4321") {
       //Serial.println("ARCCO ID");
       delay (1000);
       voice_out("Welcome to the Anteqera Radio and Cellphone Communication Incorporated");  
    }
    if (readString == "#5930") {
       delay (1000);
       emicSerial.print("N2");
       emicSerial.print('\n');
       delay(150);      
       voice_msg("Gender change.");
       resetFunc(); 
    }
    if (readString == "#5931") {
       delay (1000);
       emicSerial.print("N0");
       emicSerial.print('\n');
       delay(150);
       voice_msg("Gender change.");
       resetFunc();
    }
    if (readString == "#5932") {
       delay (1000);
       emicSerial.print("N4");
       emicSerial.print('\n');
       delay(150);
       voice_msg("Gender change.");
       resetFunc();
    }
    if (readString == "#5933") {
       delay (1000);
       emicSerial.print("W175");
       emicSerial.print('\n');
       delay(150);
       voice_msg("Speech slow.");
       resetFunc();
    }
    if (readString == "#5934") {
       delay (1000);
       emicSerial.print("W200");
       emicSerial.print('\n');
       delay(150);
       voice_msg("Speech normal.");
       resetFunc();
    }
    if (readString == "#1590") {
       delay (1000);
       voice_enable = 0;
       voice_msg("Voice off.");
       digitalWrite(tx_control,LOW);
       digitalWrite(trigger_led,LOW);  
    }  
    if (readString == "#1591") {
       delay (1000);
       voice_enable = 1;
       voice_msg("Voice on.");
       digitalWrite(tx_control,LOW);
       digitalWrite(trigger_led,LOW);  
    }  
    if (readString == "#1179") {
       tx_enable = 1;
       //Serial.println("reboot");
       delay(640);
      tone8();
      tone8();
      tone8();
      tone8();
      tone8();
      delay(250);
      digitalWrite(tx_control,LOW);
      digitalWrite(trigger_led,LOW);
      resetFunc();
    }
    
  Serial.println(readString);

  readString="";
  }
  //Serial.println("DTMF listening ...."); 
    }

 void voice_out(String msg) 
{
    digitalWrite(tx_control,HIGH);
    digitalWrite(trigger_led,HIGH);
    delay(150);
   // Speak some text
  //emicSerial.print("N2");
  //emicSerial.print('\n');
  emicSerial.print('S');
  emicSerial.print(msg);  
  emicSerial.print('\n');  
  while (emicSerial.read() != ':');   
  tone4();
  digitalWrite(tx_control,LOW);
  digitalWrite(trigger_led,LOW);
}
void say_date() 
{
  digitalWrite(tx_control,HIGH);
  digitalWrite(trigger_led,HIGH);
  //emicSerial.print("N2");
  //emicSerial.print('\n');
  emicSerial.print('S');
  emicSerial.print("Today is");
  delay (1000);
  emicSerial.print(rtc.getDOWStr());
  delay (1000);
  String moString = rtc.getDateStr();
  //Serial.println(moString);
  moString.remove(0,3);
  //Serial.print(moString);
  moString.remove(2);
  String mString = "";
    if (moString == "01") {
      mString = "January";   
    }
    if (moString == "02") {
      mString = "February";   
    }  
    if (moString == "03") {
      mString = "March";   
    }
    if (moString == "04") {
      mString = "April";   
    }
    if (moString == "05") {
      mString = "May";   
    }
    if (moString == "06") {
      mString = "June";   
    }
    if (moString == "07") {
      mString = "July";   
    }
    if (moString == "08") {
      mString = "August";   
    }
    if (moString == "09") {
      mString = "September";   
    }
    if (moString == "10") {
      mString = "October";   
    }
    if (moString == "11") {
      mString = "November";   
    }
    if (moString == "12") {
      mString = "December";   
    }    
  emicSerial.print(mString);  
  String dtString = rtc.getDateStr();
  dtString.remove(2);
  emicSerial.print(dtString);  
  emicSerial.print('\n'); 
  while (emicSerial.read() != ':');
  tone4();
  digitalWrite(tx_control,LOW);
  digitalWrite(trigger_led,LOW);
}

void say_time()
{
  digitalWrite(tx_control,HIGH);
  digitalWrite(trigger_led,HIGH);
  //emicSerial.print("N2");
  //emicSerial.print('\n');
  emicSerial.print('S');
  emicSerial.print("Time is");
  delay (1000);
  String hrString = rtc.getTimeStr();
  hrString.remove(5);
  hrString.replace(':', ' ');
  emicSerial.print(hrString);
  emicSerial.print('\n');
  while (emicSerial.read() != ':');
  tone4();
  digitalWrite(tx_control,LOW);
  digitalWrite(trigger_led,LOW); 
}
void time_check()
{
  tone4();
  //emicSerial.print("N2");
  //emicSerial.print('\n');
  emicSerial.print('S');
  emicSerial.print("Time is");
  delay (1000);
  String hrString = rtc.getTimeStr();
  hrString.remove(5);
  hrString.replace(':', ' ');
  emicSerial.print(hrString);
  emicSerial.print('\n');
  while (emicSerial.read() != ':');
}
void check_time ()
{
 String hrString = rtc.getTimeStr();
 if (hrString == "06:00:00") { 
  voice_out("Good Morning. Time is 6 o'clock.");
 }
 if (hrString == "09:00:00") { 
  voice_out("It's nine A,M.");
 }
 if (hrString == "12:00:00") { 
  voice_out("Good noon. Time is 12 PM");
 }
 if (hrString == "15:00:00") { 
  voice_out("It's 3 PM.");
 }
 if (hrString == "18:00:00") { 
  voice_out("Time is 6 PM.");
 }
 if (hrString == "23:00:00") { 
  voice_out("It's 11 PM. Good Night and 73's to all.");
 }
}  

void voice_msg(String msg) 
{   
   // Speak some text
  tone4();
  //emicSerial.print("N2");
  //emicSerial.print('\n');
  emicSerial.print('S');
  emicSerial.print(msg); 
  emicSerial.print('\n');  
  while (emicSerial.read() != ':');   
  //Serial.println("Voice msg.");
}
