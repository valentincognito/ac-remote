#include <IRremote.h>
#include <Wire.h>

IRsend irsend;

const int AC_TYPE   = 0;
int AC_HEAT         = 0;
int AC_POWER_ON     = 0;
int AC_AIR_ACLEAN   = 0;
int AC_TEMPERATURE  = 27;
int AC_FLOW         = 1;

const int AC_FLOW_TOWER[3] = {0, 4, 6};
const int AC_FLOW_WALL[4]  = {0, 2, 4, 5};

unsigned long AC_CODE_TO_SEND;

int r = LOW;
int o_r = LOW;

byte a, b = 0;

void ac_send_code(unsigned long code)
{
  Serial.println(code);
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

void setup() {
  Serial.begin(9600);
}
void loop() {
 while (Serial.available() > 0) {
    int command = Serial.parseInt();

    if (Serial.read() == '\n') {
      r = !r ;
      if ( r != o_r) {
        switch (command) {
        case 0: // off
          ac_power_down();
          break;
        case 1: // on
          ac_activate(AC_TEMPERATURE, AC_FLOW);
          break;
        case 2:
          if ( b == 0 | b == 1 ) {
            ac_change_air_swing(b);
          }
          break;
        case 3: // 1  : clean on, power on
          if ( b == 0 | b == 1 ) {
            ac_air_clean(b);
          }
          break;
        case 4:
          if ( 0 <= b && b <= 2  ) {
            ac_activate(AC_TEMPERATURE, b);
          }
          break;
        case 5:
          if (18 <= b && b <= 30  ) {
            ac_activate(b, AC_FLOW);
          }
          break;
        case 6:
          if ( 18 <= AC_TEMPERATURE && AC_TEMPERATURE <= 29 ) {
            ac_activate((AC_TEMPERATURE + 1), AC_FLOW);
          }
          break;
        case 7:
          if ( 19 <= AC_TEMPERATURE && AC_TEMPERATURE <= 30 ) {
            ac_activate((AC_TEMPERATURE - 1), AC_FLOW);
          }
          break;
        case 8:
          if ( AC_POWER_ON == 1 ) {
            ac_power_down();
            delay(100);
            ac_air_clean(1);
          } else {
            if ( AC_AIR_ACLEAN == 1) {
              ac_air_clean(0);
              delay(100);
            }
            ac_activate(AC_TEMPERATURE, AC_FLOW);
          }
          break;
        default:
          if ( 18 <= a && a <= 30 ) {
            if ( 0 <= b && b <= 2 ) {
              ac_activate(a, b);
            }
          }
        }
      }
      o_r = r ;
    }
  }
  delay(100);
}
