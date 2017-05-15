#include <EEPROM.h>

//EEPROM配置
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

int VOICE_MIN = 400;
int DIRECTION = 1;
int MOTOR_SPEED_MAX = 150;
int TIME_RUN = 5;

struct config_type
{
  int EEPROMmin;
  int EEPROMdirection;
  int EEPROMspeed;
  int EEPROMruntime;
};

void read_eeprom() {
  config_type config_readback;
  EEPROM_read(0, config_readback);
  VOICE_MIN = config_readback.EEPROMmin;
  DIRECTION = config_readback.EEPROMdirection;
  MOTOR_SPEED_MAX = config_readback.EEPROMspeed;
  TIME_RUN = config_readback.EEPROMruntime;
}

void write_eeprom() {
  config_type config;                  // 定义结构变量config，并定义config的内容
  config.EEPROMmin = VOICE_MIN;
  config.EEPROMdirection = DIRECTION;
  config.EEPROMspeed = MOTOR_SPEED_MAX;
  config.EEPROMruntime = TIME_RUN;
  EEPROM_write(0, config);         // 变量config存储到EEPROM，地址0写入
}

