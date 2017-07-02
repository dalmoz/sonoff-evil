/*
  xdrv_snfled.ino - sonoff led support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*********************************************************************************************\
 * Sonoff Led
\*********************************************************************************************/

uint8_t ledTable[] = {
    0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,
    4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,
    8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14,
   14, 15, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 22,
   22, 23, 23, 24, 25, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32,
   33, 33, 34, 35, 36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45,
   46, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
   61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78,
   80, 81, 82, 83, 85, 86, 87, 89, 90, 91, 93, 94, 95, 97, 98, 99,
  101,102,104,105,107,108,110,111,113,114,116,117,119,121,122,124,
  125,127,129,130,132,134,135,137,139,141,142,144,146,148,150,151,
  153,155,157,159,161,163,165,166,168,170,172,174,176,178,180,182,
  184,186,189,191,193,195,197,199,201,204,206,208,210,212,215,217,
  219,221,224,226,228,231,233,235,238,240,243,245,248,250,253,255 };

uint8_t sl_dcolor[2];
uint8_t sl_tcolor[2];
uint8_t sl_lcolor[2];

uint8_t sl_power;
uint8_t sl_any;
uint8_t sl_wakeupActive = 0;
uint8_t sl_wakeupDimmer = 0;
uint16_t sl_wakeupCntr = 0;

uint32_t Atoh(char *s)
{
  uint32_t value = 0;
  uint32_t digit;
  int8_t c;
  
  while((c = *s++)) {
    if ('0' <= c && c <= '9') {
      digit = c - '0';
    }
    else if ('A' <= c && c <= 'F') {
      digit = c - 'A' + 10;
    }
    else if ('a' <= c && c <= 'f') {
      digit = c - 'a' + 10;
    }
    else {
      break;
    }
    value = (value << 4) | digit;
  }
  return value;
}

void sl_setDim(uint8_t myDimmer)
{
  float newDim = 100 / (float)myDimmer;
  float fmyCld = (float)sysCfg.led_color[0] / newDim;
  float fmyWrm = (float)sysCfg.led_color[1] / newDim;
  sl_dcolor[0] = (uint8_t)fmyCld;
  sl_dcolor[1] = (uint8_t)fmyWrm;
}

/********************************************************************************************/

void sl_init(void)
{
  sysCfg.pwmvalue[0] = 0;  // We use led_color
  sysCfg.pwmvalue[1] = 0;  // We use led_color
  sl_power = 0;
  sl_any = 0;
  sl_wakeupActive = 0;
}

void sl_setPower(uint8_t power)
{
  sl_power = power &1;
  sl_wakeupActive = 0;
  sl_animate();
}

void sl_animate()
{
// {"Wakeup":"Done"}
  char svalue[32];  // was MESSZ
  uint8_t fadeValue;
  
  if (0 == sl_power) {  // Power Off
    sl_tcolor[0] = 0;
    sl_tcolor[1] = 0;
  }
  else {
    if (!sl_wakeupActive) {  // Power On
      sl_setDim(sysCfg.led_dimmer[0]);
      if (0 == sysCfg.led_fade) {
        sl_tcolor[0] = sl_dcolor[0];
        sl_tcolor[1] = sl_dcolor[1];
      } else {
        if ((sl_tcolor[0] != sl_dcolor[0]) || (sl_tcolor[1] != sl_dcolor[1])) {
          if (sl_tcolor[0] < sl_dcolor[0]) {
            sl_tcolor[0] += ((sl_dcolor[0] - sl_tcolor[0]) >> sysCfg.led_speed) +1;
          }
          if (sl_tcolor[1] < sl_dcolor[1]) {
            sl_tcolor[1] += ((sl_dcolor[1] - sl_tcolor[1]) >> sysCfg.led_speed) +1;
          }
          if (sl_tcolor[0] > sl_dcolor[0]) {
            sl_tcolor[0] -= ((sl_tcolor[0] - sl_dcolor[0]) >> sysCfg.led_speed) +1;
          }
          if (sl_tcolor[1] > sl_dcolor[1]) {
            sl_tcolor[1] -= ((sl_tcolor[1] - sl_dcolor[1]) >> sysCfg.led_speed) +1;
          }
        }
      }
    } else {   // Power On using wake up duration
      if (1 == sl_wakeupActive) {
        sl_wakeupActive = 2;
        sl_tcolor[0] = 0;
        sl_tcolor[1] = 0;
        sl_wakeupCntr = 0;
        sl_wakeupDimmer = 0;
      }
      sl_wakeupCntr++;
      if (sl_wakeupCntr > ((sysCfg.led_wakeup * STATES) / sysCfg.led_dimmer[0])) {
        sl_wakeupCntr = 0;
        sl_wakeupDimmer++;
        if (sl_wakeupDimmer <= sysCfg.led_dimmer[0]) {
          sl_setDim(sl_wakeupDimmer);
          sl_tcolor[0] = sl_dcolor[0];
          sl_tcolor[1] = sl_dcolor[1];
        } else {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"Wakeup\":\"Done\"}"));
          mqtt_publish_topic_P(2, PSTR("WAKEUP"), svalue);
          sl_wakeupActive = 0;
        }
      }
    }
  }
  if ((sl_lcolor[0] != sl_tcolor[0]) || (sl_lcolor[1] != sl_tcolor[1]) || sl_any) {
    sl_any = 0;
    sl_lcolor[0] = sl_tcolor[0];
    sl_lcolor[1] = sl_tcolor[1];
    for (byte i = 0; i < 2; i++) {
      if (pin[GPIO_PWM1 +i] < 99) {
        analogWrite(pin[GPIO_PWM1 +i], ((sysCfg.led_table) ? ledTable[sl_lcolor[i]] : sl_lcolor[i]) * (PWM_RANGE / 255));
      }
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean sl_command(char *type, uint16_t index, char *dataBufUc, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  boolean coldim = false;

  if (!strcmp_P(type,PSTR("COLOR"))) {
    uint8_t my_color[5];
    if (4 == data_len) {
      char ccold[3], cwarm[3];
      memcpy(ccold, dataBufUc, 2);
      ccold[2] = '\0';
      memcpy(cwarm, dataBufUc + 2, 2);
      cwarm[2] = '\0';
      my_color[0] = Atoh(ccold);
      my_color[1] = Atoh(cwarm);
      uint16_t temp = my_color[0];
      if (temp < my_color[1]) {
        temp = my_color[1];
      }
      float mDim = (float)temp / 2.55;
      sysCfg.led_dimmer[0] = (uint8_t)mDim;
      float newDim = 100 / mDim;
      float fmyCold = (float)my_color[0] * newDim;
      float fmyWarm = (float)my_color[1] * newDim;
      sysCfg.led_color[0] = (uint8_t)fmyCold;
      sysCfg.led_color[1] = (uint8_t)fmyWarm;
      coldim = true;
    } else {
      sl_setDim(sysCfg.led_dimmer[0]);
      uint16_t color = (uint16_t)sl_dcolor[0] << 8;
      color += (uint16_t)sl_dcolor[1];
      snprintf_P(svalue, ssvalue, PSTR("{\"Color\":\"%04X\"}"), color);
    }
  }
  else if (!strcmp_P(type,PSTR("DIMMER"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 100)) {
      sysCfg.led_dimmer[0] = payload;
      coldim = true;
    } else {
      snprintf_P(svalue, ssvalue, PSTR("{\"Dimmer\":%d}"), sysCfg.led_dimmer[0]);
    }
  }
  else if (!strcmp_P(type,PSTR("LEDTABLE"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
      switch (payload) {
      case 0: // Off
      case 1: // On
        sysCfg.led_table = payload;
        break;
      case 2: // Toggle
        sysCfg.led_table ^= 1;
        break;
      }
      sl_any = 1;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"LedTable\":\"%s\"}"), getStateText(sysCfg.led_table));
  }
  else if (!strcmp_P(type,PSTR("FADE"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
      switch (payload) {
      case 0: // Off
      case 1: // On
        sysCfg.led_fade = payload;
        break;
      case 2: // Toggle
        sysCfg.led_fade ^= 1;
        break;
      }
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Fade\":\"%s\"}"), getStateText(sysCfg.led_fade));
  }
  else if (!strcmp_P(type,PSTR("SPEED"))) {  // 1 - fast, 8 - slow
    if ((data_len > 0) && (payload > 0) && (payload <= 8)) {
      sysCfg.led_speed = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Speed\":%d}"), sysCfg.led_speed);
  }
  else if (!strcmp_P(type,PSTR("WAKEUPDURATION"))) {
    if ((data_len > 0) && (payload > 0) && (payload < 3601)) {
      sysCfg.led_wakeup = payload;
      sl_wakeupActive = 0;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"WakeUpDuration\":%d}"), sysCfg.led_wakeup);
  }
  else if (!strcmp_P(type,PSTR("WAKEUP"))) {
    do_cmnd_power(index, 1);
    sl_wakeupActive = 1;
    snprintf_P(svalue, ssvalue, PSTR("{\"Wakeup\":\"Started\"}"));
  }
  else {
    serviced = false;  // Unknown command
  }
  if (coldim) {
//    do_cmnd_power(index, (sysCfg.led_dimmer[0]>0));
    if (sysCfg.led_dimmer[0] && !(power&1)) {
      do_cmnd_power(1, 1);
    }
    else if (!sysCfg.led_dimmer[0] && (power&1)) {
      do_cmnd_power(1, 0);
    }
#ifdef USE_DOMOTICZ
    mqtt_publishDomoticzPowerState(1);
#endif  // USE_DOMOTICZ
    sl_setDim(sysCfg.led_dimmer[0]);
    uint16_t color = (uint16_t)sl_dcolor[0] << 8;
    color += (uint16_t)sl_dcolor[1];
    snprintf_P(svalue, ssvalue, PSTR("{\"POWER\":\"%s\", \"Dimmer\":%d, \"Color\":\"%04X\"}"),
      getStateText(power &1), sysCfg.led_dimmer[0], color);
  }
  return serviced;
}

