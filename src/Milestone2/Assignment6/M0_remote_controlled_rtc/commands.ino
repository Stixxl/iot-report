void leaveRoom(uint8_t param){
    Serial.print("Leave ");Serial.println(param);
    while (param!=0){
      digitalWrite(triggerIn,HIGH);
      mydelay(50);
      digitalWrite(triggerIn,LOW);
      mydelay(50);
      digitalWrite(triggerOut,HIGH);
      mydelay(50);
      digitalWrite(triggerOut,LOW);
      mydelay(500);
      param--;
    }
}

void enterRoom(uint8_t param){
    Serial.print("Enter ");Serial.println(param);
    
    while (param!=0){
      digitalWrite(triggerOut,HIGH);
      mydelay(50);
      digitalWrite(triggerOut,LOW);
      mydelay(50);
      digitalWrite(triggerIn,HIGH);
      mydelay(50);
      digitalWrite(triggerIn,LOW);
      mydelay(500);
      param--;
    }
}

void peekIntoRoom(uint8_t param){
    Serial.print("Peek Into ");Serial.println(param);
    
    while (param!=0){
      digitalWrite(triggerOut,HIGH);
      mydelay(50);
      digitalWrite(triggerOut,LOW);
      mydelay(50);
      param--;
    }
}


void mydelay(int ms) {
  long var;
  volatile float temp;
  temp = 1000000.0;

  for (var = 1; var < ms * 300; var++) temp = temp / 2;
}

void showRoomState() {

  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font


  display.clearDisplay();
  display.setCursor(0, 0);     // Start at top-left corner
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.write("G10  ");
  uint8_t day,month,year,hours,minutes,seconds;
  getTimeInCalender(day,month,year,hours,minutes,seconds);
  printTimeOnDisplay(getTimeInSeconds(),false);
  display.write("\n");
 
  display.setCursor(0, 30);     // Start at top-left corner
  display.setTextSize(4);      // Normal 1:1 pixel scale
  
  if (count<10) display.write(" ");
  display.print(count);
  display.print(" ");


  if (prediction<10) display.write(" ");
  display.print(prediction);
  display.display();

}

void resetCount(){
  count=0;
}

void setShowRoomPeriod(uint8_t param){
  showPeriod=param;
}

void setMsgPeriod(uint8_t param){
  msgPeriod=param;
}

void setTimewarpFactor(uint8_t param){
  setTime(getTimeInSeconds());
  timewarpFactor=param;
}

void setPrediction(uint8_t param){
  prediction=param;
}

void runTaskControlled(uint8_t cmd, uint8_t param, boolean repeat){
  //Serial.println(cmd);
  if (cmd==1) enterRoom(param);
  if (cmd==2) leaveRoom(param);
  if (cmd==3) peekIntoRoom(param);

  if (cmd==11) {
    showRoomState();
    if(repeat) insertTask(getTimeInSeconds()+showPeriod, cmd, 0);
  }
  if (cmd==12) {
    do_send(&sendjob);
    if(repeat) insertTask(getTimeInSeconds()+msgPeriod,cmd,0);
  }
  if (cmd==13) {
    resetCount();
    if(repeat) insertTask(getTimeInSeconds()+resetPeriod, cmd, 0);
  }
  if (cmd==14) setPrediction(param);

  
  if (cmd==21) setShowRoomPeriod(param);
  if (cmd==22) setMsgPeriod(param);
  if (cmd==23) setTimewarpFactor(param);

  if (cmd==31) printTasklist();
  if (cmd==32) printCurrentTime();

}

void runTaskOnce(uint8_t cmd, uint8_t param){
  runTaskControlled(cmd,param,0);
}

void runTaskRepeated(uint8_t cmd, uint8_t param){
  runTaskControlled(cmd,param,1);
}
