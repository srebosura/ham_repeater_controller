    //Repeater COR design for Motorola Radius or Maxtrac
    //Samuel Rebosura
    //rebosura_sd@yahoo.com.ph
    //09173145734
    
    void(* resetFunc) (void) = 0; //software reset vector
    const int aux_control = 8;
    const int rx_signal =  10;
    const int tx_control =  11;
    const int tone_out = 12;
    const int trigger_led =  13;
    int rx_active = 1;
    int last_rx_state = HIGH;
    int tone_signal = 0;
    int tone_ready = 0;
    long debounceDelay = 50;
    long lastRxDebounceTime = 0;
    long TimeLastCheck = millis();
    long trigger_timeout = 90; // maximum trigger time
    long TimeoutTime = 0;
    long time_var = 0;
    int time_out_state = 0;
    int trigger_loop = 0;
    int tone_enable = 1;
    int dtmf_detected = 0;
    int tx_enable = 1;
    int hangtime = 640; // time adjust for tx off delay before beep
    #include <DTMF.h>

  const int sensorPin = A0;
  const int dtmf_led = 9;
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
       Serial.begin(9600);
       pinMode(tx_control,OUTPUT);
       pinMode(aux_control,OUTPUT);
       pinMode(trigger_led, OUTPUT);
       pinMode(rx_signal, INPUT_PULLUP);
       pinMode(dtmf_led, OUTPUT); 
       digitalWrite(aux_control,LOW);
       digitalWrite(trigger_led,LOW);
       digitalWrite(tx_control,LOW);
       Serial.println("COR Ready!");
       delay(5000);       

}

void loop() {
  dtmf_decode();
  if (trigger_loop <= 5)  {
  dtmf_decode();  
  trigger_control_2();
  }
  if (trigger_loop <= 15 && trigger_loop >= 5)  {
  dtmf_decode();
  trigger_control();
  }
  if (trigger_loop <= 20 && trigger_loop >= 15)  {
  dtmf_decode();
  trigger_control_3();
  }
  if (trigger_loop == 21)  {
  trigger_loop = 0;
  }
}

void wait_rx() {
  int reading;
  int rx_state=1;              
  //rx signal             
  reading = digitalRead(rx_signal);
  if (reading != last_rx_state) {
   lastRxDebounceTime = millis();
     }                
      if ((millis() - lastRxDebounceTime) > debounceDelay) {
        rx_state = reading;
        lastRxDebounceTime=millis();
        }                               
        last_rx_state = reading;        
      if(rx_state==0){
        rx_active = 1;
        } 
 }

void trigger_control() {
  if (digitalRead(rx_signal) == 0 && tone_ready == 1)  {
    check_timeout();
    return;
  }
  if (digitalRead(rx_signal) == 0 && tx_enable == 1)  {
    digitalWrite(tx_control,HIGH);
    digitalWrite(trigger_led,HIGH);
    Serial.println("RX detected");
    //dtmf_decode();
    tone_ready = 1;
    set_timeout();       
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 1)  {
    delay(hangtime);
    if (tone_enable == 1) {
    send_tone();
    }
    if (tone_enable == 0) {
    tone_ready = 0;
    }
    if (tone_enable == 2) {
    tone4(); // for single beep via dtmf remote
    }
    if (tone_enable == 3) {
    tone7(); // for triple beep via dtmf remote
    }
    delay(300);
    if ( dtmf_detected == 1)  {
      tone8();
      tone8();
      tone8();
      tone8();
      tone8();
      dtmf_detected = 0;
      delay(300);
    }
    digitalWrite(tx_control,LOW);
    digitalWrite(trigger_led,LOW);
    readString="";
    trigger_loop = trigger_loop+1;
    tone_signal = tone_signal+1;
    if (tone_signal == 8) {
      tone_signal = 0;
    }
    Serial.println("Tone_signal:");
    Serial.println(tone_signal);
    Serial.println("Transmit Time = ");
    //time_var = trigger_timeout-(TimeoutTime/1000);
    //Serial.println(time_var);
    set_timeout();
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 0)  {
    return;        
  }
  
}

void trigger_control_2() {
  if (digitalRead(rx_signal) == 0 && tone_ready == 1)  {
    check_timeout();
    return;
  }
  if (digitalRead(rx_signal) == 0 && tx_enable == 1)  {
    digitalWrite(tx_control,HIGH);
    digitalWrite(trigger_led,HIGH);
    Serial.println("RX detected");
    //dtmf_decode();
    tone_ready = 1;
    set_timeout();       
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 1)  {
    delay(hangtime);
    if (tone_enable == 1) {
    tone4(); 
    }
    if (tone_enable == 0) {
    tone_ready = 0;
    }
    if (tone_enable == 2) {
    tone4(); // for single beep via dtmf remote
    }
    if (tone_enable == 3) {
    tone7(); // for triple beep via dtmf remote
    }   
    delay(300);
    if ( dtmf_detected == 1)  {
      tone8();
      tone8();
      tone8();
      tone8();
      tone8();
      dtmf_detected = 0;
      delay(300);
    }
    digitalWrite(tx_control,LOW);
    digitalWrite(trigger_led,LOW);
    readString="";
    trigger_loop = trigger_loop+1;
    //tone_signal = tone_signal+1;
    //if (tone_signal == 8) {
    //  tone_signal = 0;
    //}
    Serial.println("Tone_signal:");
    Serial.println(tone_signal);
    Serial.println("Transmit Time = ");
    //time_var = trigger_timeout-(TimeoutTime/1000);
    //Serial.println(time_var);
    set_timeout();
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 0)  {
    return;        
  } 
}
void trigger_control_3() {
  if (digitalRead(rx_signal) == 0 && tone_ready == 1)  {
    check_timeout();
    return;
  }
  if (digitalRead(rx_signal) == 0 && tx_enable == 1)  {
    digitalWrite(tx_control,HIGH);
    digitalWrite(trigger_led,HIGH);
    Serial.println("RX detected");
    //dtmf_decode();
    tone_ready = 1;
    set_timeout();       
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 1)  {
    delay(hangtime);
    if (tone_enable == 1) {
    tone7();
    }
    if (tone_enable == 0) {
    tone_ready = 0;
    }
    if (tone_enable == 2) {
    tone4(); // for single beep via dtmf remote
    }
    if (tone_enable == 3) {
    tone7(); // for triple beep via dtmf remote
    }   
    delay(300);
    if ( dtmf_detected == 1)  {
      tone8();
      tone8();
      tone8();
      tone8();
      tone8();
      dtmf_detected = 0;
      delay(300);
    }
    digitalWrite(tx_control,LOW);
    digitalWrite(trigger_led,LOW);
    readString="";
    trigger_loop = trigger_loop+1;
    //tone_signal = tone_signal+1;
    //if (tone_signal == 8) {
    //  tone_signal = 0;
    //}
    Serial.println("Tone_signal:");
    Serial.println(tone_signal);
    Serial.println("Transmit Time = ");
    //time_var = trigger_timeout-(TimeoutTime/1000);
    //Serial.println(time_var);
    set_timeout();
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
      //default:
       // tone_signal = 0;
       // break;
 }
}

void tone0() {
      tone(tone_out, 495, 250);
      delay(250);
      noTone(tone_out);
      delay(250);
      tone_ready = 0;
      Serial.println("Tone_0");    
}
void tone1() {
      tone(tone_out, 880, 250);
      delay(250);
      noTone(tone_out);
      delay(250);
      tone_ready = 0;
      Serial.println("Tone_1");
}
void tone2() {
      tone(tone_out, 660, 100);
      delay(75);
      tone(tone_out, 495, 150);
      delay(250);
      tone_ready = 0;
      Serial.println("Tone_2");
}
void tone3() {
     tone(tone_out, 1760, 100);
      delay(100);
      tone(tone_out, 880, 100);
      delay(100);
      tone(tone_out, 330, 150);
      delay(200);
      tone_ready = 0;
      Serial.println("Tone_3");
}
void tone4() {
      tone(tone_out, 330, 250);
      delay(250);
      noTone(tone_out);
      delay(250);
      tone_ready = 0;
      Serial.println("Tone_4");    
}
void tone5() {
      tone(tone_out, 495, 100);
      delay(100);
      tone(tone_out, 880, 200);
      delay(200);
      tone_ready = 0;
      Serial.println("Tone_5");
}
void tone6() {
      tone(tone_out, 330, 100);
      delay(100);
      tone(tone_out, 495, 150);
      delay(200);
      tone_ready = 0;
      Serial.println("Tone_6");
}
void tone7() {
     tone(tone_out, 495, 100);
      delay(100);
      tone(tone_out, 660, 100);
      delay(100);
      tone(tone_out, 880, 150);
      delay(200);
      tone_ready = 0;
      Serial.println("Tone_7");
}
void tone8() {
      tone(tone_out, 1760, 100);
      delay(100);
      tone(tone_out, 800, 100);
      delay(100);
      tone(tone_out, 540, 100);
      delay(100);
      tone_ready = 0;
      Serial.println("Tone_8");
}
 void set_timeout() {
   
    TimeoutTime = trigger_timeout*1000;
    TimeLastCheck = millis();
    Serial.println("Set_time_out");
             
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
      digitalWrite(tx_control,LOW);
      digitalWrite(trigger_led,LOW);
      Serial.println("Time_out");
      delay(30000); // Set to 30 seconds TX off time
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
  // To set the remote code modify below, example readString == "1234"  
  }
  if (readString.length() == 4) {
    if (readString == "#012") {
       tone_enable = 0;
       Serial.println("tone off");
       dtmf_detected = 1;
    }
    if (readString == "#045") {
       tone_enable = 1;
       trigger_loop = 0;
       Serial.println("tone on");
       dtmf_detected = 1;
    }
    if (readString == "#013") {
       tone_enable = 2;
       Serial.println("tone4 on");
       dtmf_detected = 1;
    }
    if (readString == "#014") {
       tone_enable = 3;
       Serial.println("tone7 on");
       dtmf_detected = 1;
    }
    if (readString == "*036") {
       tx_enable = 0;
       Serial.println("tx off");
       dtmf_detected = 1;
    }
    if (readString == "*098") {
       tx_enable = 1;
       Serial.println("tx on");
       dtmf_detected = 1;
    }
    if (readString == "1179") {
       tx_enable = 1;
       Serial.println("rebooting.....");
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
    if (readString == "1231") {
       Serial.println("Auxilliary control on");
       delay(640);
      tone8();
      tone8();
      tone8();
      tone8();
      tone8();
      delay(250);
      digitalWrite(aux_control,HIGH);
    }
     if (readString == "1230") {
       Serial.println("Auxilliary control off");
       delay(640);
      tone8();
      tone8();
      tone8();
      tone8();
      tone8();
      delay(250);
      digitalWrite(aux_control,LOW);
    }
    
  Serial.println(readString);

  readString="";
  }
  //Serial.println("DTMF listening ...."); 
    }
    
