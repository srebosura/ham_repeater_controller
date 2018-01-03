    //Repeater COR design for Motorola Radius or Maxtrac
    //Samuel Rebosura
    
    void(* resetFunc) (void) = 0; //software reset vector
    const int rx_signal =  10;
    const int tx_control =  11;
    const int tone_out = 12;
    const int trigger_led =  13;
    const int tone_enable =  2;
    int rx_active = 1;
    int last_rx_state = HIGH;
    int tone_signal = 0;
    int tone_ready = 0;
    long debounceDelay = 50;
    long lastRxDebounceTime = 0;
    long TimeLastCheck = millis();
    long trigger_timeout = 45;
    long TimeoutTime = 0;
    long time_var = 0;
    int time_out_state = 0;
    int trigger_loop = 0;
    

void setup() {
       Serial.begin(9600);
       pinMode(tx_control,OUTPUT);
       pinMode(trigger_led, OUTPUT);
       pinMode(rx_signal, INPUT_PULLUP);
       pinMode(tone_enable, INPUT_PULLUP);
       digitalWrite(tx_control,LOW);
       digitalWrite(trigger_led,LOW);
       Serial.println("COR Ready!");
       delay(10000);       

}

void loop() { 
  trigger_control();  
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
  if (digitalRead(rx_signal) == 0)  {
    digitalWrite(tx_control,HIGH);
    digitalWrite(trigger_led,HIGH);
    Serial.println("RX detected");
    tone_ready = 1;
    set_timeout();       
   }
  if (digitalRead(rx_signal) == 1 && tone_ready == 1)  {
    delay(640);
    if (tone_enable == 1) {
    send_tone();
    }
    delay(300);
    digitalWrite(tx_control,LOW);
    digitalWrite(trigger_led,LOW);
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
      tone(tone_out, 330, 100);
      delay(150);
      tone(tone_out, 660, 150);
      delay(200);
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
      delay(150);
      tone(tone_out, 800, 100);
      delay(150);
      tone(tone_out, 540, 100);
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
      resetFunc();
      }
    }
    
