#include <IRremote.h>
#include <Wire.h>

IRsend irsend;

const int AC_TYPE  = 0;
// 0 : TOWER
// 1 : WALL

int AC_HEAT = 0;
// 0 : cooling
// 1 : heating

int AC_POWER_ON    = 0;
// 0 : off
// 1 : on

int AC_AIR_ACLEAN  = 0;
// 0 : off
// 1 : on --> power on

int AC_TEMPERATURE = 27;
// temperature : 18 ~ 30

int AC_FLOW        = 1;
// 0 : low
// 1 : mid
// 2 : high
// if AC_TYPE =1, 3 : change

const int AC_FLOW_TOWER[3] = {0, 4, 6};
const int AC_FLOW_WALL[4]  = {0, 2, 4, 5};

unsigned long AC_CODE_TO_SEND;

int r = LOW;
int o_r = LOW;

byte a, b = 0;

void ac_send_code(unsigned long code)
{
  Serial.print("code to send : ");
  Serial.print(code, BIN);
  Serial.print(" : ");
  Serial.println(code, HEX);

  irsend.sendLG(code, 28);
}

void ac_activate(int temperature, int air_flow)
{
  int AC_MSBITS1 = 8;
  int AC_MSBITS2 = 8;
  int AC_MSBITS3 = 0;
  int AC_MSBITS4 ;
  if ( AC_HEAT == 1 ) {
    // heating
    AC_MSBITS4 = 4;
  } else {
    // cooling
    AC_MSBITS4 = 0;
  }
  int AC_MSBITS5 = temperature - 15;
  int AC_MSBITS6 ;

  if ( AC_TYPE == 0) {
    AC_MSBITS6 = AC_FLOW_TOWER[air_flow];
  } else {
    AC_MSBITS6 = AC_FLOW_WALL[air_flow];
  }

  int AC_MSBITS7 = (AC_MSBITS3 + AC_MSBITS4 + AC_MSBITS5 + AC_MSBITS6) & B00001111;

  AC_CODE_TO_SEND =  AC_MSBITS1 << 4 ;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS2) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS3) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS4) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS5) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS6) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS7);

  ac_send_code(AC_CODE_TO_SEND);

  AC_POWER_ON = 1;
  AC_TEMPERATURE = temperature;
  AC_FLOW = air_flow;
}

void ac_change_air_swing(int air_swing)
{
  if ( AC_TYPE == 0) {
    if ( air_swing == 1) {
      AC_CODE_TO_SEND = 0x881316B;
    } else {
      AC_CODE_TO_SEND = 0x881317C;
    }
  } else {
    if ( air_swing == 1) {
      AC_CODE_TO_SEND = 0x8813149;
    } else {
      AC_CODE_TO_SEND = 0x881315A;
    }
  }

  ac_send_code(AC_CODE_TO_SEND);
}

void ac_power_down()
{
  AC_CODE_TO_SEND = 0x88C0051;

  ac_send_code(AC_CODE_TO_SEND);

  AC_POWER_ON = 0;
}

void ac_air_clean(int air_clean)
{
  if ( air_clean == 1) {
    AC_CODE_TO_SEND = 0x88C000C;
  } else {
    AC_CODE_TO_SEND = 0x88C0084;
  }

  ac_send_code(AC_CODE_TO_SEND);

  AC_AIR_ACLEAN = air_clean;
}

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;

void setup()
{
  Serial.begin(38400);
  delay(1000);

  ac_activate(27, 2);
}

void loop()
{
  recvWithEndMarker();
  parsInput();
  showNewData();
        
  if ( r != o_r) {
    /*
    # a : mode or temp    b : air_flow, temp, swing, clean, cooling/heating
    # 18 ~ 30 : temp      0 ~ 2 : flow // on
    # 0 : off             0
    # 1 : on              0
    # 2 : air_swing       0 or 1
    # 3 : air_clean       0 or 1
    # 4 : air_flow        0 ~ 2 : flow
    # 5 : temp            18 ~ 30
    # + : temp + 1
    # - : temp - 1
    # m : change cooling to air clean, air clean to cooling
    */
//    if(incomingByte == 0){
//       Serial.write(incomingByte);
//       //ac_activate(27, 2);
//    }else if(incomingByte == 1){
//      ac_power_down();
//    }
    
//    switch (incomingByte) {
//      case 0: // off
//        ac_power_down();
//        break;
//      case 1: // on
//        ac_activate(AC_TEMPERATURE, AC_FLOW);
//        break;
//      case 2:
//        if ( b == 0 | b == 1 ) {
//          ac_change_air_swing(b);
//        }
//        break;
//      case 3: // 1  : clean on, power on
//        if ( b == 0 | b == 1 ) {
//          ac_air_clean(b);
//        }
//        break;
//      case 4:
//        if ( 0 <= b && b <= 2  ) {
//          ac_activate(AC_TEMPERATURE, b);
//        }
//        break;
//      case 5:
//        if (18 <= b && b <= 30  ) {
//          ac_activate(b, AC_FLOW);
//        }
//        break;
//      case '+':
//        if ( 18 <= AC_TEMPERATURE && AC_TEMPERATURE <= 29 ) {
//          ac_activate((AC_TEMPERATURE + 1), AC_FLOW);
//        }
//        break;
//      case '-':
//        if ( 19 <= AC_TEMPERATURE && AC_TEMPERATURE <= 30 ) {
//          ac_activate((AC_TEMPERATURE - 1), AC_FLOW);
//        }
//        break;
//      case 'm':
//        /*
//          if ac is on,  1) turn off, 2) turn on ac_air_clean(1)
//          if ac is off, 1) turn on,  2) turn off ac_air_clean(0)
//        */
//        if ( AC_POWER_ON == 1 ) {
//          ac_power_down();
//          delay(100);
//          ac_air_clean(1);
//        } else {
//          if ( AC_AIR_ACLEAN == 1) {
//            ac_air_clean(0);
//            delay(100);
//          }
//          ac_activate(AC_TEMPERATURE, AC_FLOW);
//        }
//        break;
//      default:
//        if ( 18 <= a && a <= 30 ) {
//          if ( 0 <= b && b <= 2 ) {
//            ac_activate(a, b);
//          }
//        }
//    }

    o_r = r ;
  }
  delay(100);
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';  //\n for newline and \r for carriage return
  char rc;
  
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    //Serial.print(rc,HEX);
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
      //Serial.println(0);
    }
    else {
      Serial.println(0);
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
void parsInput(){
  if (newData == true) {
    String myString = receivedChars;
    int commaIndex = myString.indexOf("=");
    //  Search for the next comma just after the first
    int secondCommaIndex = myString.indexOf(';', commaIndex + 1);
  
    String firstValue = myString.substring(0, commaIndex);
    String secondValue = myString.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = myString.substring(secondCommaIndex + 1); // To the end of the string
    
    Serial.println(firstValue);
    Serial.println(secondValue);
    Serial.println(thirdValue);
    
    inputCommands(firstValue, secondValue);
  }
}

void showNewData() {
  if (newData == true) {
    //Serial.print("This just in ... ");
    //Serial.println(receivedChars);
    newData = false;
  }
}

void inputCommands(String par, String val) {
    if (par == "led"){
      Serial.println(val);
    } else if (par == "ech"){
      Serial.println(val);
    } else {
      Serial.println("<Par not found!>");
    } 
}

