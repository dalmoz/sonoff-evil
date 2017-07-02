/*
  xdrv_ws2812.ino - ws2812 led string support for Sonoff-Tasmota

  Copyright (C) 2017  Heiko Krupp and Theo Arends

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

#ifdef USE_WS2812
/*********************************************************************************************\
 * WS2812 Leds using NeopixelBus library
\*********************************************************************************************/

#include <NeoPixelBus.h>

#ifdef USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip = NULL;
#else  // USE_WS2812_CTYPE
  NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> *strip = NULL;
#endif  // USE_WS2812_CTYPE
#else  // USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> *strip = NULL;
#else  // USE_WS2812_CTYPE
  NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> *strip = NULL;
#endif  // USE_WS2812_CTYPE
#endif  // USE_WS2812_DMA

struct wsColor {
  uint8_t red, green, blue;
};

struct ColorScheme {
  wsColor* colors;
  uint8_t count;
};

wsColor incandescent[2] = { 255, 140, 20, 0, 0, 0 };
wsColor rgb[3] = { 255, 0, 0, 0, 255, 0, 0, 0, 255 };
wsColor christmas[2] = { 255, 0, 0, 0, 255, 0 };
wsColor hanukkah[2] = { 0, 0, 255, 255, 255, 255 };
wsColor kwanzaa[3] = { 255, 0, 0, 0, 0, 0, 0, 255, 0 };
wsColor rainbow[7] = { 255, 0, 0, 255, 128, 0, 255, 255, 0, 0, 255, 0, 0, 0, 255, 128, 0, 255, 255, 0, 255 };
wsColor fire[3] = { 255, 0, 0, 255, 102, 0, 255, 192, 0 };
ColorScheme schemes[7] = {
  incandescent, 2,
  rgb, 3,
  christmas, 2,
  hanukkah, 2,
  kwanzaa, 3,
  rainbow, 7,
  fire, 3 };

uint8_t widthValues[5] = { 
    1,     // Small
    2,     // Medium
    4,     // Large
    8,     // Largest
  255 };   // All
uint8_t repeatValues[5] = { 
    8,     // Small
    6,     // Medium
    4,     // Large
    2,     // Largest
    1 };   // All
uint8_t speedValues[6] = { 
    0,     // None
   18,     // Slowest
   14,     // Slower
   10,     // Slow
    6,     // Fast
    2 };   // Fastest
/*
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
*/
uint8_t lany = 0;
RgbColor dcolor;
RgbColor tcolor;
RgbColor lcolor;

uint8_t wakeupDimmer = 0;
uint16_t wakeupCntr = 0;
unsigned long stripTimerCntr = 0;  // Bars and Gradient

void ws2812_setDim(uint8_t myDimmer)
{
  float newDim = 100 / (float)myDimmer;
  float fmyRed = (float)sysCfg.ws_red / newDim;
  float fmyGrn = (float)sysCfg.ws_green / newDim;
  float fmyBlu = (float)sysCfg.ws_blue / newDim;
  dcolor.R = (uint8_t)fmyRed;
  dcolor.G = (uint8_t)fmyGrn;
  dcolor.B = (uint8_t)fmyBlu;
}

void ws2812_setColor(uint16_t led, char* colstr)
{
  HtmlColor hcolor;
  char log[LOGSZ];
  char lcolstr[8]; 
  
  snprintf_P(lcolstr, sizeof(lcolstr), PSTR("#%s"), colstr);
  uint8_t result = hcolor.Parse<HtmlColorNames>((char *)lcolstr, 7);
  if (result) {
    if (led) {
      strip->SetPixelColor(led -1, RgbColor(hcolor)); // Led 1 is strip Led 0 -> substract offset 1
      strip->Show();
    } else {
      dcolor = RgbColor(hcolor);

//      snprintf_P(log, sizeof(log), PSTR("DBG: Red %02X, Green %02X, Blue %02X"), dcolor.R, dcolor.G, dcolor.B);
//      addLog(LOG_LEVEL_DEBUG, log);
  
      uint16_t temp = dcolor.R;
      if (temp < dcolor.G) {
        temp = dcolor.G;
      }
      if (temp < dcolor.B) {
        temp = dcolor.B;
      }
      float mDim = (float)temp / 2.55;
      sysCfg.ws_dimmer = (uint8_t)mDim;
  
      float newDim = 100 / mDim;
      float fmyRed = (float)dcolor.R * newDim;
      float fmyGrn = (float)dcolor.G * newDim;
      float fmyBlu = (float)dcolor.B * newDim;
      sysCfg.ws_red = (uint8_t)fmyRed;
      sysCfg.ws_green = (uint8_t)fmyGrn;
      sysCfg.ws_blue = (uint8_t)fmyBlu;

      lany = 1;
    }
  }
}

void ws2812_replaceHSB(String *response)
{
  ws2812_setDim(sysCfg.ws_dimmer);
  HsbColor hsb = HsbColor(dcolor);
  response->replace("{h}", String((uint16_t)(65535.0f * hsb.H)));
  response->replace("{s}", String((uint8_t)(254.0f * hsb.S)));
  response->replace("{b}", String((uint8_t)(254.0f * hsb.B)));
}

void ws2812_getHSB(float *hue, float *sat, float *bri)
{
  ws2812_setDim(sysCfg.ws_dimmer);
  HsbColor hsb = HsbColor(dcolor);
  *hue = hsb.H;
  *sat = hsb.S;
  *bri = hsb.B;
}

void ws2812_setHSB(float hue, float sat, float bri)
{
  char rgb[7];
  
  HsbColor hsb;
  hsb.H = hue;
  hsb.S = sat;
  hsb.B = bri;
  RgbColor tmp = RgbColor(hsb);
  sprintf(rgb,"%02X%02X%02X", tmp.R, tmp.G, tmp.B);
  ws2812_setColor(0,rgb);
}

void ws2812_getColor(uint16_t led, char* svalue, uint16_t ssvalue)
{
  RgbColor mcolor;
  char stemp[20];
  
  if (led) {
    mcolor = strip->GetPixelColor(led -1);
    snprintf_P(stemp, sizeof(stemp), PSTR("Led%d"), led);
  } else {
    ws2812_setDim(sysCfg.ws_dimmer);
    mcolor = dcolor;
    snprintf_P(stemp, sizeof(stemp), PSTR("Color"));
  }
  uint32_t color = (uint32_t)mcolor.R << 16;
  color += (uint32_t)mcolor.G << 8;
  color += (uint32_t)mcolor.B;
  snprintf_P(svalue, ssvalue, PSTR("{\"%s\":\"%06X\"}"), stemp, color);
}

void ws2812_stripShow()
{
  RgbColor c;  

  if (sysCfg.ws_ledtable) {
    for (uint16_t i = 0; i < sysCfg.ws_pixels; i++) {
      c = strip->GetPixelColor(i);
      strip->SetPixelColor(i, RgbColor(ledTable[c.R], ledTable[c.G], ledTable[c.B]));
    }
  }
  strip->Show();
}

void ws2812_resetWakupState()
{
  wakeupDimmer = 0;
  wakeupCntr = 0;
}

void ws2812_resetStripTimer()
{
  stripTimerCntr = 0;
}

int mod(int a, int b)
{
   int ret = a % b;
   if (ret < 0) {
    ret += b;
   }
   return ret;
}

void ws2812_clock()
{
  RgbColor c;
  
  strip->ClearTo(0);   // Reset strip
  float newDim = 100 / (float)sysCfg.ws_dimmer;
  float f1 = 255 / newDim;
  uint8_t i1 = (uint8_t)f1;
  float f2 = 127 / newDim;
  uint8_t i2 = (uint8_t)f2;
  float f3 = 63 / newDim;
  uint8_t i3 = (uint8_t)f3;

  int j = sysCfg.ws_pixels;
  int clksize = 600 / j;
  int i = (rtcTime.Second * 10) / clksize;

  c = strip->GetPixelColor(mod(i,    j)); c.B = i1; strip->SetPixelColor(mod(i,    j), c);
  i = (rtcTime.Minute * 10) / clksize;
  c = strip->GetPixelColor(mod(i -1, j)); c.G = i3; strip->SetPixelColor(mod(i -1, j), c);
  c = strip->GetPixelColor(mod(i,    j)); c.G = i1; strip->SetPixelColor(mod(i,    j), c);
  c = strip->GetPixelColor(mod(i +1, j)); c.G = i3; strip->SetPixelColor(mod(i +1, j), c);
  i = (rtcTime.Hour % 12) * (50 / clksize);
  c = strip->GetPixelColor(mod(i -2, j)); c.R = i3; strip->SetPixelColor(mod(i -2, j), c);
  c = strip->GetPixelColor(mod(i -1, j)); c.R = i2; strip->SetPixelColor(mod(i -1, j), c);
  c = strip->GetPixelColor(mod(i,    j)); c.R = i1; strip->SetPixelColor(mod(i,    j), c);
  c = strip->GetPixelColor(mod(i +1, j)); c.R = i2; strip->SetPixelColor(mod(i +1, j), c);
  c = strip->GetPixelColor(mod(i +2, j)); c.R = i3; strip->SetPixelColor(mod(i +2, j), c);
  ws2812_stripShow();
}

void ws2812_gradientColor(struct wsColor* mColor, uint8_t range, uint8_t gradRange, uint8_t i)
{
/*
 * Compute the color of a pixel at position i using a gradient of the color scheme.
 * This function is used internally by the gradient function.
 */
  ColorScheme scheme = schemes[sysCfg.ws_scheme -3];
  uint8_t curRange = i / range;
  uint8_t rangeIndex = i % range;
  uint8_t colorIndex = rangeIndex / gradRange;
  uint8_t start = colorIndex;
  uint8_t end = colorIndex +1;
  if (curRange % 2 != 0) {
    start = (scheme.count -1) - start;
    end = (scheme.count -1) - end;
  }
  float newDim = 100 / (float)sysCfg.ws_dimmer;
  float fmyRed = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].red, scheme.colors[end].red) / newDim;
  float fmyGrn = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].green, scheme.colors[end].green) / newDim;
  float fmyBlu = (float)map(rangeIndex % gradRange, 0, gradRange, scheme.colors[start].blue, scheme.colors[end].blue) / newDim;
  mColor->red = (uint8_t)fmyRed;
  mColor->green = (uint8_t)fmyGrn;
  mColor->blue = (uint8_t)fmyBlu;
}

void ws2812_gradient()
{
/* 
 * This routine courtesy Tony DiCola (Adafruit)
 * Display a gradient of colors for the current color scheme.
 *  Repeat is the number of repetitions of the gradient (pick a multiple of 2 for smooth looping of the gradient).
 */
  RgbColor c;

  ColorScheme scheme = schemes[sysCfg.ws_scheme -3];
  if (scheme.count < 2) {
    return;
  }

  uint8_t repeat = repeatValues[sysCfg.ws_width];  // number of scheme.count per ledcount
  uint8_t range = (uint8_t)ceil((float)sysCfg.ws_pixels / (float)repeat);
  uint8_t gradRange = (uint8_t)ceil((float)range / (float)(scheme.count - 1));
  uint8_t offset = speedValues[sysCfg.ws_speed] > 0 ? stripTimerCntr / speedValues[sysCfg.ws_speed] : 0;

  wsColor oldColor, currentColor;
  ws2812_gradientColor(&oldColor, range, gradRange, offset);
  currentColor = oldColor;
  for (uint16_t i = 0; i < sysCfg.ws_pixels; i++) {
    if (repeatValues[sysCfg.ws_width] > 1) {
      ws2812_gradientColor(&currentColor, range, gradRange, i +offset);
    }
    if (sysCfg.ws_speed > 0) {
      // Blend old and current color based on time for smooth movement.
      c.R = map(stripTimerCntr % speedValues[sysCfg.ws_speed], 0, speedValues[sysCfg.ws_speed], oldColor.red, currentColor.red);
      c.G = map(stripTimerCntr % speedValues[sysCfg.ws_speed], 0, speedValues[sysCfg.ws_speed], oldColor.green, currentColor.green);
      c.B = map(stripTimerCntr % speedValues[sysCfg.ws_speed], 0, speedValues[sysCfg.ws_speed], oldColor.blue, currentColor.blue);
    }
    else {
      // No animation, just use the current color.
      c.R = currentColor.red;
      c.G = currentColor.green;
      c.B = currentColor.blue;
    }
    strip->SetPixelColor(i, c);
    oldColor = currentColor;
  }
  ws2812_stripShow();
}

void ws2812_bars()
{
/* 
 * This routine courtesy Tony DiCola (Adafruit)
 * Display solid bars of color for the current color scheme.
 * Width is the width of each bar in pixels/lights.
 */
  RgbColor c;
  uint16_t i;

  ColorScheme scheme = schemes[sysCfg.ws_scheme -3];

  uint8_t maxSize = sysCfg.ws_pixels / scheme.count;
  if (widthValues[sysCfg.ws_width] > maxSize) {
    maxSize = 0;
  }

  uint8_t offset = speedValues[sysCfg.ws_speed] > 0 ? stripTimerCntr / speedValues[sysCfg.ws_speed] : 0;

  wsColor mcolor[scheme.count];
  memcpy(mcolor, scheme.colors, sizeof(mcolor));
  float newDim = 100 / (float)sysCfg.ws_dimmer;
  for (i = 0; i < scheme.count; i++) {
    float fmyRed = (float)mcolor[i].red / newDim;
    float fmyGrn = (float)mcolor[i].green / newDim;
    float fmyBlu = (float)mcolor[i].blue / newDim;
    mcolor[i].red = (uint8_t)fmyRed;
    mcolor[i].green = (uint8_t)fmyGrn;
    mcolor[i].blue = (uint8_t)fmyBlu;
  }
  uint8_t colorIndex = offset % scheme.count;
  for (i = 0; i < sysCfg.ws_pixels; i++) {
    if (maxSize) {
      colorIndex = ((i + offset) % (scheme.count * widthValues[sysCfg.ws_width])) / widthValues[sysCfg.ws_width];
    }
    c.R = mcolor[colorIndex].red;
    c.G = mcolor[colorIndex].green;
    c.B = mcolor[colorIndex].blue;
    strip->SetPixelColor(i, c);
  }
  ws2812_stripShow();
}

void ws2812_animate()
{
  char log[LOGSZ];
  uint8_t fadeValue;
  
  stripTimerCntr++;
  if (0 == power) {  // Power Off
    sleep = sysCfg.sleep;
    stripTimerCntr = 0;
    tcolor = 0;
  }
  else {
    sleep = 0;
    switch (sysCfg.ws_scheme) {
      case 0:  // Power On
        ws2812_setDim(sysCfg.ws_dimmer);
        if (0 == sysCfg.ws_fade) {
          tcolor = dcolor;
        } else {
          if (tcolor != dcolor) {
            if (tcolor.R < dcolor.R) {
              tcolor.R += ((dcolor.R - tcolor.R) >> sysCfg.ws_speed) +1;
            }
            if (tcolor.G < dcolor.G) {
              tcolor.G += ((dcolor.G - tcolor.G) >> sysCfg.ws_speed) +1;
            }
            if (tcolor.B < dcolor.B) {
              tcolor.B += ((dcolor.B - tcolor.B) >> sysCfg.ws_speed) +1;
            }
            if (tcolor.R > dcolor.R) {
              tcolor.R -= ((tcolor.R - dcolor.R) >> sysCfg.ws_speed) +1;
            }
            if (tcolor.G > dcolor.G) {
              tcolor.G -= ((tcolor.G - dcolor.G) >> sysCfg.ws_speed) +1;
            }
            if (tcolor.B > dcolor.B) {
              tcolor.B -= ((tcolor.B - dcolor.B) >> sysCfg.ws_speed) +1;
            }
          }
        }
        break;
      case 1:  // Wake up light
        wakeupCntr++;
        if (0 == wakeupDimmer) {
          tcolor = 0;
          wakeupDimmer++;
        }
        else {
          if (wakeupCntr > ((sysCfg.ws_wakeup * STATES) / sysCfg.ws_dimmer)) {
            wakeupCntr = 0;
            wakeupDimmer++;
            if (wakeupDimmer <= sysCfg.ws_dimmer) {
              ws2812_setDim(wakeupDimmer);
              tcolor = dcolor;
            } else
              sysCfg.ws_scheme = 0;
          }
        }
        break;
      case 2:  // Clock
        if (((STATES/10)*2 == state) || (lany != 2)) {
          ws2812_clock();
        }
        lany = 2;
        break;
      default:
        if (1 == sysCfg.ws_fade) {
          ws2812_gradient();
        } else {
          ws2812_bars();
        }
        lany = 1;
        break;
    }
  }

  if ((sysCfg.ws_scheme <= 1) || (!(power &1))) {
    if ((lcolor != tcolor) || lany) {
      lany = 0;
      lcolor = tcolor;

//    snprintf_P(log, sizeof(log), PSTR("DBG: StripPixels %d, CfgPixels %d, Red %02X, Green %02X, Blue %02X"), strip->PixelCount(), sysCfg.ws_pixels, lcolor.R, lcolor.G, lcolor.B);
//    addLog(LOG_LEVEL_DEBUG, log);

      if (sysCfg.ws_ledtable) {
        for (uint16_t i = 0; i < sysCfg.ws_pixels; i++) {
          strip->SetPixelColor(i, RgbColor(ledTable[lcolor.R],ledTable[lcolor.G],ledTable[lcolor.B]));      
        }
      } else {
        for (uint16_t i = 0; i < sysCfg.ws_pixels; i++) {
          strip->SetPixelColor(i, lcolor);
        }
      }
      strip->Show();
    }
  }
}

void ws2812_update()
{
  lany = 1;
}

void ws2812_pixels()
{
  strip->ClearTo(0);
  strip->Show();
  tcolor = 0;
  lany = 1;
}

void ws2812_init()
{
#ifdef USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  strip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(WS2812_MAX_LEDS);  // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
#else  // USE_WS2812_CTYPE
  strip = new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(WS2812_MAX_LEDS);  // For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
#endif  // USE_WS2812_CTYPE
#else  // USE_WS2812_DMA
#if (USE_WS2812_CTYPE == 1)
  strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod>(WS2812_MAX_LEDS, pin[GPIO_WS2812]);
#else  // USE_WS2812_CTYPE
  strip = new NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod>(WS2812_MAX_LEDS, pin[GPIO_WS2812]);
#endif  // USE_WS2812_CTYPE
#endif  // USE_WS2812_DMA
  strip->Begin();
  ws2812_pixels();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean ws2812_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;

  if (!strcmp_P(type,PSTR("PIXELS"))) {
    if ((data_len > 0) && (payload > 0) && (payload <= WS2812_MAX_LEDS)) {
      sysCfg.ws_pixels = payload;
      ws2812_pixels();
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Pixels\":%d}"), sysCfg.ws_pixels);
  }
  else if (!strcmp_P(type,PSTR("LED")) && (index > 0) && (index <= sysCfg.ws_pixels)) {
    if (6 == data_len) {
//       ws2812_setColor(index, dataBufUc);
      ws2812_setColor(index, dataBuf);
    }
    ws2812_getColor(index, svalue, ssvalue);
  }
  else if (!strcmp_P(type,PSTR("COLOR"))) {
    if (6 == data_len) {
//        ws2812_setColor(0, dataBufUc);
      ws2812_setColor(0, dataBuf);
      power = 1;
    }
    ws2812_getColor(0, svalue, ssvalue);
  }
  else if (!strcmp_P(type,PSTR("DIMMER"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 100)) {
      sysCfg.ws_dimmer = payload;
      power = 1;
#ifdef USE_DOMOTICZ
      mqtt_publishDomoticzPowerState(index);
#endif  // USE_DOMOTICZ
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Dimmer\":%d}"), sysCfg.ws_dimmer);
  }
  else if (!strcmp_P(type,PSTR("LEDTABLE"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
      switch (payload) {
      case 0: // Off
      case 1: // On
        sysCfg.ws_ledtable = payload;
        break;
      case 2: // Toggle
        sysCfg.ws_ledtable ^= 1;
        break;
      }
      ws2812_update();
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"LedTable\":\"%s\"}"), getStateText(sysCfg.ws_ledtable));
  }
  else if (!strcmp_P(type,PSTR("FADE"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 2)) {
      switch (payload) {
      case 0: // Off
      case 1: // On
        sysCfg.ws_fade = payload;
        break;
      case 2: // Toggle
        sysCfg.ws_fade ^= 1;
        break;
      }
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Fade\":\"%s\"}"), getStateText(sysCfg.ws_fade));
  }
  else if (!strcmp_P(type,PSTR("SPEED"))) {  // 1 - fast, 5 - slow
    if ((data_len > 0) && (payload > 0) && (payload <= 5)) {
      sysCfg.ws_speed = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Speed\":%d}"), sysCfg.ws_speed);
  }
  else if (!strcmp_P(type,PSTR("WIDTH"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 4)) {
      sysCfg.ws_width = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Width\":%d}"), sysCfg.ws_width);
  }
  else if (!strcmp_P(type,PSTR("WAKEUP"))) {
    if ((data_len > 0) && (payload > 0) && (payload < 3601)) {
      sysCfg.ws_wakeup = payload;
      if (1 == sysCfg.ws_scheme) {
        sysCfg.ws_scheme = 0;
      }
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"WakeUp\":%d}"), sysCfg.ws_wakeup);
  }
  else if (!strcmp_P(type,PSTR("SCHEME"))) {
    if ((data_len > 0) && (payload >= 0) && (payload <= 9)) {
      sysCfg.ws_scheme = payload;
      if (1 == sysCfg.ws_scheme) {
        ws2812_resetWakupState();
      }
      power = 1;
      ws2812_resetStripTimer();
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"Scheme\":%d}"), sysCfg.ws_scheme);
  }
  else {
    serviced = false;  // Unknown command
  }
  return serviced;
}
#endif  // USE_WS2812
