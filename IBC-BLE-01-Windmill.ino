#include <Microduino_Key.h>
#include"userDef.h"
#include"colorLed.h"
#include"motor.h"
#include "eeprom.h"
#include "windmillJson.h"

Key keyMic(PIN_MIC, INPUT_PULLUP);

void setup() {
  Serial.begin(57600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  MotorLeft.Fix(1);
  MotorRight.Fix(1);
  read_eeprom();

  if (VOICE_MIN < 100 || VOICE_MIN > 800 || (abs(DIRECTION) != 1) || MOTOR_SPEED_MAX < 80 || MOTOR_SPEED_MAX > 255 || TIME_RUN < 1 || TIME_RUN > 15)
  {
    VOICE_MIN = 400;
    DIRECTION = 1;
    MOTOR_SPEED_MAX = 150;
    TIME_RUN = 5;
  }

  motorRun(MOTOR_SPEED_MAX * DIRECTION, MOTOR_SPEED_MAX * DIRECTION);
  setAllLed(COLOR_RED);
  delay(400);
  setAllLed(COLOR_GREEN);
  delay(400);
  setAllLed(COLOR_BLUE);
  delay(400);
  setAllLed(COLOR_NONE);

#if DEBUG
  Serial.println("**************START************");
  Serial.print("MIC DATA:");
  Serial.println(VOICE_MIN);
  Serial.print("SPEED DATA:");
  Serial.println(MOTOR_SPEED_MAX);
  Serial.print("DIRECTION DATA:");
  Serial.println(DIRECTION);
  Serial.print("RUNTIME DATA:");
  Serial.println(TIME_RUN);
  Serial.println("---------------------------");
#endif
}

void loop() {

  //  Serial.print("MIC Val:");
  //  Serial.println(analogRead(PIN_MIC));

  windmilljson_data();

  if (keyMic.read(VOICE_MIN, VOICE_MAX) == SHORT_PRESS)  //如果声音传感器检测到了声音，风车和LED灯将会启动。
  {
    uint32_t motorTimer = millis();
    while (millis() - motorTimer < TIME_RUN * 1000)          //在检测到声音的10秒钟以内
    {
      windmilljson_data();
      motorRun(MOTOR_SPEED_MAX * DIRECTION, MOTOR_SPEED_MAX * DIRECTION);   //启动电机。
      ledRainbow(10);                                   //LED开始变换彩虹颜色。
    }
  }
  else                                   //检测到声音10秒钟以后
  {
    motorFree();//关掉电机。
    setAllLed(COLOR_NONE);              //关掉LED灯。
    //ledBreath(1, 15);
  }
}
