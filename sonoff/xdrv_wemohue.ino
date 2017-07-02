/*
  xdrv_wemohue.ino - wemo and hue support for Sonoff-Tasmota

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

#ifdef USE_EMULATION

#define UDP_BUFFER_SIZE 200         // Max UDP buffer size needed for M-SEARCH message

boolean udpConnected = false;

char packetBuffer[UDP_BUFFER_SIZE]; // buffer to hold incoming UDP packet
IPAddress ipMulticast(239, 255, 255, 250); // Simple Service Discovery Protocol (SSDP)
uint32_t portMulticast = 1900;      // Multicast address and port

/*********************************************************************************************\
 * WeMo UPNP support routines
\*********************************************************************************************/
const char WEMO_MSEARCH[] PROGMEM =
  "HTTP/1.1 200 OK\r\n"
  "CACHE-CONTROL: max-age=86400\r\n"
  "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
  "EXT:\r\n"
  "LOCATION: http://{r1}:80/setup.xml\r\n"
  "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
  "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
  "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
  "ST: urn:Belkin:device:**\r\n"
  "USN: uuid:{r2}::urn:Belkin:device:**\r\n"
  "X-User-Agent: redsonic\r\n"
  "\r\n";

String wemo_serial()
{
  char serial[16];
  
  snprintf_P(serial, sizeof(serial), PSTR("201612K%08X"), ESP.getChipId());
  return String(serial);
}

String wemo_UUID()
{
  char uuid[27];
  
  snprintf_P(uuid, sizeof(uuid), PSTR("Socket-1_0-%s"), wemo_serial().c_str());
  return String(uuid);
}

void wemo_respondToMSearch()
{
  char message[TOPSZ];
  char log[LOGSZ];

  if (portUDP.beginPacket(portUDP.remoteIP(), portUDP.remotePort())) {
    String response = FPSTR(WEMO_MSEARCH);
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", wemo_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
    snprintf_P(message, sizeof(message), PSTR("Response sent"));
  } else {
    snprintf_P(message, sizeof(message), PSTR("Failed to send response"));
  }
  snprintf_P(log, sizeof(log), PSTR("UPnP: Wemo %s to %s:%d"),
    message, portUDP.remoteIP().toString().c_str(), portUDP.remotePort());
  addLog(LOG_LEVEL_DEBUG, log);
}

/*********************************************************************************************\
 * Hue Bridge UPNP support routines
 * Need to send 3 response packets with varying ST and USN
\*********************************************************************************************/
const char HUE_RESPONSE[] PROGMEM =
  "HTTP/1.0 200 OK\r\n"
  "HOST: 239.255.255.250:1900\r\n"
  "CACHE-CONTROL: max-age=100\r\n"
  "EXT:\r\n"
  "LOCATION: http://{r1}:80/description.xml\r\n"
  "SERVER: FreeRTOS/7.4.2 UPnP/1.0 IpBridge/1.15.0\r\n"
  "hue-bridgeid: {r2}\r\n";
const char HUE_ST1[] PROGMEM = 
  "ST: upnp:rootdevice\r\n";
const char HUE_USN1[] PROGMEM =
  "USN: uuid:{r3}::upnp:rootdevice\r\n"
  "\r\n";

const char HUE_ST2[] PROGMEM =
  "ST: uuid:{r3}\r\n";
const char HUE_USN2[] PROGMEM =
  "USN: uuid:{r3}\r\n"
  "\r\n";

const char HUE_ST3[] PROGMEM =
  "ST: urn:schemas-upnp-org:device:basic:1\r\n";
  
String hue_bridgeid()
{
  char bridgeid[16];
  
  snprintf_P(bridgeid, sizeof(bridgeid), PSTR("5CCF7FFFFE%03X"), ESP.getChipId());
  return String(bridgeid);
}

String hue_UUID()
{
  char serial[36];
  
  snprintf_P(serial, sizeof(serial), PSTR("f6543a06-da50-11ba-8d8f-5ccf7f%03x"), ESP.getChipId());
  return String(serial);
}

void hue_respondToMSearch()
{
  char message[TOPSZ];
  char log[LOGSZ];

  if (portUDP.beginPacket(portUDP.remoteIP(), portUDP.remotePort())) {
    String response = FPSTR(HUE_RESPONSE);
    String response_st = FPSTR(HUE_ST1);
    String response_usn = FPSTR(HUE_USN1);
    response += response_st + response_usn;
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", hue_bridgeid());
    response.replace("{r3}", hue_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
//    addLog(LOG_LEVEL_DEBUG_MORE, response.c_str());
    
    response = FPSTR(HUE_RESPONSE);
    response_st = FPSTR(HUE_ST2);
    response_usn = FPSTR(HUE_USN2);
    response += response_st + response_usn;
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", hue_bridgeid());
    response.replace("{r3}", hue_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
//    addLog(LOG_LEVEL_DEBUG_MORE, response.c_str());
    
    response = FPSTR(HUE_RESPONSE);
    response_st = FPSTR(HUE_ST3);
    response += response_st + response_usn;
    response.replace("{r1}", WiFi.localIP().toString());
    response.replace("{r2}", hue_bridgeid());
    response.replace("{r3}", hue_UUID());
    portUDP.write(response.c_str());
    portUDP.endPacket();
    
    snprintf_P(message, sizeof(message), PSTR("3 response packets sent"));
//    addLog(LOG_LEVEL_DEBUG_MORE, response.c_str());

  } else {
    snprintf_P(message, sizeof(message), PSTR("Failed to send response"));
  }
  snprintf_P(log, sizeof(log), PSTR("UPnP: HUE %s to %s:%d"),
    message, portUDP.remoteIP().toString().c_str(), portUDP.remotePort());
  addLog(LOG_LEVEL_DEBUG, log);
}

/********************************************************************************************/

boolean UDP_Disconnect()
{
  if (udpConnected) {
    WiFiUDP::stopAll();
    addLog_P(LOG_LEVEL_DEBUG, PSTR("UPnP: Multicast disabled"));
    udpConnected = false;
  }
  return udpConnected;
}

boolean UDP_Connect()
{
  if (!udpConnected) {
    if (portUDP.beginMulticast(WiFi.localIP(), ipMulticast, portMulticast)) {
      addLog_P(LOG_LEVEL_INFO, PSTR("UPnP: Multicast (re)joined"));
      udpConnected = true;
    } else {
      addLog_P(LOG_LEVEL_INFO, PSTR("UPnP: Multicast join failed"));
      udpConnected = false;
    }
  }
  return udpConnected;
}

void pollUDP()
{
  if (udpConnected) {
    if (portUDP.parsePacket()) {
      int len = portUDP.read(packetBuffer, UDP_BUFFER_SIZE -1);
      if (len > 0) {
        packetBuffer[len] = 0;
      }
      String request = packetBuffer;

//      addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("UDP: Packet received"));
//      addLog_P(LOG_LEVEL_DEBUG_MORE, packetBuffer);

      if (request.indexOf("M-SEARCH") >= 0) {
        if ((EMUL_WEMO == sysCfg.flag.emulation) &&(request.indexOf("urn:Belkin:device:**") > 0)) {
          wemo_respondToMSearch();
        }
        else if ((EMUL_HUE == sysCfg.flag.emulation) && ((request.indexOf("ST: urn:schemas-upnp-org:device:basic:1") > 0) || (request.indexOf("ST: upnp:rootdevice") > 0) || (request.indexOf("ST: ssdp:all") > 0))) {
          hue_respondToMSearch();
        }
      }
    }
  }
}

#ifdef USE_WEBSERVER
/*********************************************************************************************\
 * Web server additions
\*********************************************************************************************/
const char WEMO_EVENTSERVICE_XML[] PROGMEM =
  "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
  "<actionList>"
    "<action>"
      "<name>SetBinaryState</name>"
      "<argumentList>"
        "<argument>"
          "<retval/>"
          "<name>BinaryState</name>"
          "<relatedStateVariable>BinaryState</relatedStateVariable>"
          "<direction>in</direction>"
        "</argument>"
      "</argumentList>"
      "<serviceStateTable>"
        "<stateVariable sendEvents=\"yes\">"
          "<name>BinaryState</name>"
          "<dataType>Boolean</dataType>"
          "<defaultValue>0</defaultValue>"
        "</stateVariable>"
        "<stateVariable sendEvents=\"yes\">"
          "<name>level</name>"
          "<dataType>string</dataType>"
          "<defaultValue>0</defaultValue>"
        "</stateVariable>"
      "</serviceStateTable>"
    "</action>"
  "</scpd>\r\n"
  "\r\n";
const char WEMO_SETUP_XML[] PROGMEM =
  "<?xml version=\"1.0\"?>"
  "<root>"
    "<device>"
      "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
      "<friendlyName>{x1}</friendlyName>"
      "<manufacturer>Belkin International Inc.</manufacturer>"
      "<modelName>Sonoff Socket</modelName>"
      "<modelNumber>3.1415</modelNumber>"
      "<UDN>uuid:{x2}</UDN>"
      "<serialNumber>{x3}</serialNumber>"
      "<binaryState>0</binaryState>"
      "<serviceList>"
        "<service>"
          "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
          "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
          "<controlURL>/upnp/control/basicevent1</controlURL>"
          "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
          "<SCPDURL>/eventservice.xml</SCPDURL>"
        "</service>"
      "</serviceList>"
    "</device>"
  "</root>\r\n"
  "\r\n";
const char HUE_DESCRIPTION_XML[] PROGMEM =
  "<?xml version=\"1.0\"?>"
  "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
  "<specVersion>"
      "<major>1</major>"
      "<minor>0</minor>"
  "</specVersion>"
  "<URLBase>http://{x1}/</URLBase>"
  "<device>"
      "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>"
      "<friendlyName>Amazon-Echo-HA-Bridge ({x1})</friendlyName>"
      "<manufacturer>Royal Philips Electronics</manufacturer>"
      "<modelName>Philips hue bridge 2012</modelName>"
      "<modelNumber>929000226503</modelNumber>"
      "<UDN>uuid:{x2}</UDN>"
  "</device>"
  "</root>\r\n"
  "\r\n";
const char HUE_LIGHT_STATUS_JSON[] PROGMEM =
  "{\"state\":"
      "{\"on\":{state},"
      "\"bri\":{b},"
      "\"hue\":{h},"
      "\"sat\":{s},"
      "\"effect\":\"none\","
      "\"ct\":0,"
      "\"alert\":\"none\","
      "\"colormode\":\"hs\","
      "\"reachable\":true"
  "},"
  "\"type\":\"Dimmable light\","
  "\"name\":\"{j1}\","
  "\"modelid\":\"LWB004\","
  "\"manufacturername\":\"Philips\","
  "\"uniqueid\":\"{j2}\","
  "\"swversion\":\"66012040\""
  "}";
const char HUE_LIGHT_RESPONSE_JSON[] PROGMEM =
  "{\"success\":{\"{api}/{id}/{cmd}\":{res}}}";
const char HUE_CONFIG_RESPONSE_JSON[] PROGMEM =
  "{\"name\":\"Philips hue\","
   "\"mac\":\"{mac}\","
   "\"dhcp\":true,"
   "\"ipaddress\":\"{ip}\","
   "\"netmask\":\"{mask}\","
   "\"gateway\":\"{gw}\","
   "\"proxyaddress\":\"\","
   "\"proxyport\":0,"
   "\"UTC\":\"{dt}\","
   "\"whitelist\":{\"{id}\":{"
     "\"last use date\":\"{dt}\","
     "\"create date\":\"{dt}\","
     "\"name\":\"Remote\"}},"
   "\"swversion\":\"01036659\","
   "\"apiversion\":\"1.16.0\","
   "\"swupdate\":{\"updatestate\":0,\"url\":\"\",\"text\":\"\",\"notify\": false},"
   "\"linkbutton\":false,"
   "\"portalservices\":false"
  "}";
const char HUE_ERROR_JSON[] PROGMEM =
  "[{\"error\":{\"type\":901,\"address\":\"/\",\"description\":\"Internal Error\"}}]";

void handleUPnPevent()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle WeMo basic event"));

  String request = webServer->arg(0);
  if (request.indexOf("State>1</Binary") > 0) {
    do_cmnd_power(1, 1);
  }
  if (request.indexOf("State>0</Binary") > 0) {
    do_cmnd_power(1, 0);
  }
  webServer->send(200, F("text/plain"), "");
}

void handleUPnPservice()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle WeMo event service"));
  webServer->send_P(200, PSTR("text/plain"), WEMO_EVENTSERVICE_XML);
}

void handleUPnPsetupWemo()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle WeMo setup"));

  String setup_xml = FPSTR(WEMO_SETUP_XML);
  setup_xml.replace("{x1}", sysCfg.friendlyname[0]);
  setup_xml.replace("{x2}", wemo_UUID());
  setup_xml.replace("{x3}", wemo_serial());
  webServer->send(200, F("text/xml"), setup_xml);
}

/********************************************************************************************/

String hue_deviceId(uint8_t id)
{
  char deviceid[16];
  
  snprintf_P(deviceid, sizeof(deviceid), PSTR("5CCF7F%03X-%0d"), ESP.getChipId(), id);
  return String(deviceid);
}

void handleUPnPsetupHue()
{
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Hue Bridge setup"));

  String description_xml = FPSTR(HUE_DESCRIPTION_XML);
  description_xml.replace("{x1}", WiFi.localIP().toString());
  description_xml.replace("{x2}", hue_UUID());
  webServer->send(200, F("text/xml"), description_xml);
}

void hue_todo(String *path)
{
  char log[LOGSZ];
  
  snprintf_P(log, sizeof(log), PSTR("HTTP: HUE API not implemented (%s)"),path->c_str());
  addLog(LOG_LEVEL_DEBUG_MORE, log);
}

void hue_config_response(String *response)
{
  *response += FPSTR(HUE_CONFIG_RESPONSE_JSON);
  response->replace("{mac}", WiFi.macAddress());
  response->replace("{ip}", WiFi.localIP().toString());
  response->replace("{mask}", WiFi.subnetMask().toString());
  response->replace("{gw}", WiFi.gatewayIP().toString());
  response->replace("{dt}", getDateTime());
}

void hue_global_cfg(String *path)
{
  String response;

  path->remove(0,1);               // cut leading / to get <id>
  response = "{\"lights\":{\"";
  for (uint8_t i = 1; i <= Maxdevice; i++) {
    response += i;
    response += "\":";
    response += FPSTR(HUE_LIGHT_STATUS_JSON);
    if (i < Maxdevice) {
      response += ",\"";
    }
    response.replace("{state}", (power & (0x01 << (i-1))) ? "true" : "false");
    response.replace("{j1}", sysCfg.friendlyname[i-1]);
    response.replace("{j2}", hue_deviceId(i));  
    if (pin[GPIO_WS2812] < 99) {
#ifdef USE_WS2812
      ws2812_replaceHSB(&response);
#endif // USE_WS2812
    } else {
      response.replace("{h}", "0");
      response.replace("{s}", "0");
      response.replace("{b}", "0");
    }
  }
  response += F("},\"groups\":{},\"schedules\":{},\"config\":");

  hue_config_response(&response);
  response.replace("{id}", *path);
  response += "}";
  webServer->send(200, F("application/json"), response);
}

void hue_auth(String *path)
{
  char response[38];
  
  snprintf_P(response, sizeof(response), PSTR("[{\"success\":{\"username\":\"%03x\"}}]"), ESP.getChipId());
  webServer->send(200, F("application/json"), response);
}

void hue_config(String *path)
{
  String response = "";

  path->remove(0,1);               // cut leading / to get <id>
  hue_config_response(&response);
  response.replace("{id}", *path);
  webServer->send(200, F("application/json"), response);
}

void hue_lights(String *path)
{
/*
 * http://sonoff/api/username/lights/1/state?1={"on":true,"hue":56100,"sat":254,"bri":254,"alert":"none","transitiontime":40}
 */
  String  response;
  uint8_t device = 1;
  uint16_t tmp=0;
  int16_t pos = 0;
  float   bri = 0;
  float   hue = 0;
  float   sat = 0;
  bool on = false;
  bool change = false;
  char id[4];

  path->remove(0,path->indexOf("/lights"));                // Remove until /lights
  if (path->endsWith("/lights")) {                         // Got /lights
    response = "{\"";
    for (uint8_t i = 1; i <= Maxdevice; i++) {
      response += i;
      response += "\":";
      response += FPSTR(HUE_LIGHT_STATUS_JSON);
      if (i < Maxdevice) {
        response += ",\"";
      }
      response.replace("{state}", (power & (0x01 << (i-1))) ? "true" : "false");
      response.replace("{j1}", sysCfg.friendlyname[i-1]);
      response.replace("{j2}", hue_deviceId(i));  
      if (pin[GPIO_WS2812] < 99) {
#ifdef USE_WS2812
        ws2812_replaceHSB(&response);
#endif // USE_WS2812
      } else {
        response.replace("{h}", "0");
        response.replace("{s}", "0");
        response.replace("{b}", "0");
      }
    }
    response += "}";
    webServer->send(200, F("application/json"), response);
  }
  else if (path->endsWith("/state")) {                       // Got ID/state
    path->remove(0,8);                                       // Remove /lights/
    path->remove(path->indexOf("/state"));                   // Remove /state
    device = atoi(path->c_str());
    if ((device < 1) || (device > Maxdevice)) {
      device = 1;
    }
    response = "[";
    response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
    response.replace("{api}", "/lights");
    response.replace("{id}", String(device));
    response.replace("{cmd}", "state/on");
    if (1 == webServer->args()) {
      StaticJsonBuffer<400> jsonBuffer;
      JsonObject &hue_json = jsonBuffer.parseObject(webServer->arg(0));
      if (hue_json.containsKey("on")) {
        on = hue_json["on"];
        switch(on)
        {
          case false : do_cmnd_power(device, 0);
                       response.replace("{res}", "false");
                       break;
          case true  : do_cmnd_power(device, 1);
                       response.replace("{res}", "true");
                       break;
          default    : response.replace("{res}", (power & (0x01 << (device-1))) ? "true" : "false");
                       break;
        }
      }
#ifdef USE_WS2812
      ws2812_getHSB(&hue,&sat,&bri);
      if (hue_json.containsKey("bri")) {
        tmp = hue_json["bri"];
        bri = (float)tmp/254.0f;
        response += ",";
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{api}", "/lights");
        response.replace("{id}", String(device));
        response.replace("{cmd}", "state/bri");
        response.replace("{res}", String(tmp));
        change = true;
      }
      if (hue_json.containsKey("hue")) {
        tmp = hue_json["hue"];
        hue = (float)tmp/65535.0f;
        response += ",";
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{api}", "/lights");
        response.replace("{id}", String(device));
        response.replace("{cmd}", "state/hue");
        response.replace("{res}", String(tmp));
        change = true;
      }
      if (hue_json.containsKey("sat")) {
        tmp = hue_json["sat"];
        sat = (float)tmp/254.0f;
        response += ",";
        response += FPSTR(HUE_LIGHT_RESPONSE_JSON);
        response.replace("{api}", "/lights");
        response.replace("{id}", String(device));
        response.replace("{cmd}", "state/sat");
        response.replace("{res}", String(tmp));
        change = true;
      }
      if (change && (pin[GPIO_WS2812] < 99)) {
        ws2812_setHSB(hue, sat, bri);
        change = false;
      }
#endif // USE_WS2812
      response += "]";
    }   
    else {
      response=FPSTR(HUE_ERROR_JSON);
    }
    webServer->send(200, F("application/json"), response);
  }
  else if(path->indexOf("/lights/") >= 0) {            // Got /lights/ID
    path->remove(0,8);                                 // Remove /lights/
    device = atoi(path->c_str());
    if ((device < 1) || (device > Maxdevice)) {
      device = 1;
    }
    response = FPSTR(HUE_LIGHT_STATUS_JSON);
    response.replace("{state}", (power & (0x01 << (device -1))) ? "true" : "false");
    response.replace("{j1}", sysCfg.friendlyname[device -1]);
    response.replace("{j2}", hue_deviceId(device));
    if (pin[GPIO_WS2812] < 99) {
#ifdef USE_WS2812
      ws2812_replaceHSB(&response);
#endif // USE_WS2812
    } else {
      response.replace("{h}", "0");
      response.replace("{s}", "0");
      response.replace("{b}", "0");
    }
    webServer->send(200, F("application/json"), response);
  }
  else webServer->send(406, F("application/json"), "{}");
}

void handle_hue_api(String *path)
{
  /* HUE API uses /api/<userid>/<command> syntax. The userid is created by the echo device and
   * on original HUE the pressed button allows for creation of this user. We simply ignore the
   * user part and allow every caller as with Web or WeMo. 
   *
   * (c) Heiko Krupp, 2017
   */
   
  char log[LOGSZ];
  uint8_t args = 0;

  path->remove(0, 4);                                // remove /api      
  snprintf_P(log, sizeof(log), PSTR("HTTP: Handle Hue API (%s)"), path->c_str());
  addLog(LOG_LEVEL_DEBUG_MORE, log);
  for (args = 0; args < webServer->args(); args++) {
    String json = webServer->arg(args);
    snprintf_P(log, sizeof(log), PSTR("HTTP: Hue POST args (%s)"), json.c_str());
    addLog(LOG_LEVEL_DEBUG_MORE, log);
  }
  
  if (path->endsWith("/invalid/")) {}                 // Just ignore
  else if (path->endsWith("/")) hue_auth(path);       // New HUE App setup
  else if (path->endsWith("/config")) hue_config(path);
  else if (path->indexOf("/lights") >= 0) hue_lights(path);
  else if (path->endsWith("/groups")) hue_todo(path);
  else if (path->endsWith("/schedules")) hue_todo(path); 
  else if (path->endsWith("/sensors")) hue_todo(path);
  else if (path->endsWith("/scenes")) hue_todo(path);
  else if (path->endsWith("/rules")) hue_todo(path);
  else hue_global_cfg(path);
}
#endif  // USE_WEBSERVER
#endif  // USE_EMULATION

