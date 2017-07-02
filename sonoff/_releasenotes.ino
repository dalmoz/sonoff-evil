/* 5.1.0 20170513
 * Fix Offline/Removal of retained topic when FullTopic is changed
 * Add FullTopic to MQTT Configuration and Information web page
 * Add license model GPLv3 (#188)
 *
 * 5.0.7 20170511
 * Fix possible exception 28 on empty command
 * Add command SetOption0 as replacement for SaveState
 * Add command SetOption1 as replacement for ButtonRestrict
 * Add command SetOption2 as replacement for Units
 * Add command SetOption4 as replacement for MqttResponse
 * Add command SetOption8 as replacement for TempUnit
 * Add command SetOption10 On|Off to select between Offline or Removing previous retained topic (#417, #436)
 *
 * 5.0.6 20170510
 * Remove hyphen in case of a single DHT sensor connected (#427)
 * Add command MqttRetry <seconds> to change default MQTT reconnect retry timer from minimal 10 seconds (#429)
 *
 * 5.0.5 20170508
 * Add command FullTopic with tokens %topic% (replaced by command Topic value) and 
 *  %prefix% (replaced by command Prefix<x> values) for more flexible topic definitions (#244)
 *  See wiki > MQTT Features https://github.com/arendst/Sonoff-Tasmota/wiki/MQTT-Features for more information
 *
 * 5.0.4 20170505
 * Add Sonoff Pow Energy Total up to 40 MWh
 * Add command EnergyReset 1|2|3 to reset Energy counters (#406)
 * Fix Domoticz Energy logging (#411)
 * Add command PowerOnState 4 to keep relay always on and disabling all power control (#418)
 *
 * 5.0.3 20170504
 * Add command SensorRetain on|off to enable retaining of mqtt message tele/sonoff/SENSOR (#74)
 * Change WifiConfig timeout from 60 seconds to 180 seconds (#212)
 * Change Sonoff Touch command Ledstate functionality by turning led on if power is off (#214)
 * Add 4 seconds delay after power on before enabling button to workaround Wemos D1 mini RTS circuit (#380)
 *
 * 5.0.2 20170503
 * Reset SaveData, SaveState and MqttResponse to default values due to rearranging settings
 * Moved some settings to flag area
 * Add command TempUnit Celsius|Fahrenheit for selecting Celsius or Fahrenheit (#347)
 * Add command TempRes 0..3 for selecting Temperature Resolution (#347)
 * Add command HumRes 0..3 for selecting Humidity Resolution (#347)
 * Add command PressRes 0..3 for selecting Pressure Resolution (#347)
 * Add command EnergyRes 0..5 for selecting Energy Resolution (#347)
 * Add "TempUnit":"C|F" to sensor JSON output (#347)
 * Add support for up to three DHT type sensors each using a different GPIO (#339, #404)
 *
 * 5.0.1 20170429
 * Adjust Sonoff SC messages to prepare for display feature
 * Move static data from RAM to Flash
 * Fix PowerOnState for some devices not reporting "Power on" state (#284, #380, #383)
 *
 * 5.0.0 20170425
 * Memory status message update
 * Fix setting migration to better preserve settings during move (#382)
 * Best practice is first doing a Backup Configuration before installing version 5.0.0
 * Reset save count after setting move
 * Start using new linker script without SPIFFS
 *
 * 4.2.0 20170424
 * Prepare for SPIFFS removal by moving settings to EEPROM area
 * Fix compilation error when webserver is disabled (#378)
 *
 * 4.1.3 20170410
 * Add user configuarble GPIO to module S20 Socket and Slampher
 * Add support for Sonoff SC (#112)
 * Set PWM frequency from 1000Hz to 910Hz as used on iTead Sonoff Led firmware (#122)
 * Set Sonoff Led unconfigured floating outputs to 0 to reduce exceptions due to power supply instabilities (#122)
 * Add Access Point Mac Address to Status 11 and Telemetry (#329)
 * Fix DS18B20 negative temperature readings (#334)
 *
 * 4.1.2 20170403
 * Rename Unrecognised command to Unknown command
 * Remove all command lists
 * Remove command SmartConfig (superseded by WifiConfig)
 * Fix boot loop when selecting module Sonoff 4CH or Sonoff Touch on non ESP8285 hardware
 * Add optional support for Toshiba and Mitsubishi HVAC IR control (needs updated IRremote8266 library) (#83, #257)
 * Add all configured switches to Domoticz Configuration web page (#305)
 * Fix compile error when selecting WS2812 DMA (#313)
 *
 * 4.1.1 20170329
 * Fix default Telemetry for command Prefix3
 * Fix webserver Module parameters for disabled select
 * Fix sensor status for enabled switches
 * Remove Light as alternative for Power (save code space)
 * Remove migration option from pre V3 (code cleanup)
 * Remove unofficial SPIFFS support (code cleanup)
 * Remove command list when unknown command is entered (save code space)
 * Rename Status11 json from StatusPWR to unique StatusSTS
 * Rename command Gateway to IPAddres2, Subnetmask to IPAddress3 and DnsServer to IPAddress4 (save code space)
 * Add Command MqttResponse to select either command or RESULT topic as response (#258)
 * Add command StateText1 to StateText3 to assign MQTT_STATUS_OFF, MQTT_STATUS_ON and MQTT_CMND_TOGGLE respectively (#286)
 * Remove restart after IPAddress changes (#292)
 * Add support for MAX31850 in xsns_ds18x20.ino (#295)
 * Fix possible uptime update misses (#302)
 * 
 * 4.1.0 20170325
 * Change static IP addresses in user_config.h from list (using commas) to string (using dots)
 * Unify display result of commands Modules, Module and Gpios
 * Rewrite Module selection web page to bring size down from 18651 to 4319 bytes (!) (#234, #240)
 * Add basic support for (Lixada) H801 RGBWW controller (#252)
 * Add command Prefix1 to Prefix3 to assign SUB_PREFIX, PUB_PREFIX and PUB_PREFIX2 respectively (#255)
 * Add static ip addresses to flash (#262)
 * Add commands IpAddress, Gateway, Subnetmask and DnsServer to select static ip addresses (#273)
 *
 * 4.0.8 20170321
 * Fix entering non-numeric webpassword
 * Force selection between TLS or Webserver due to memory restraint (#240)
 * Allow entering empty string using "0" for selected commands (#242)
 * Fix exception when posting commands to web console containing % (#250)
 *
 * 4.0.7 20170319
 * Increased Sonoff Led PWM frequency from 432 to 1000
 * Fix possible watch dog reboot after changing module type on web page
 * Fix reporting of GPIO usage from web page
 * Fix Sonoff Led blank during firmware upgrade
 * Fix Sonoff Led flicker and possible flash corruption by using latest Arduino-esp8266 versions
 *   of pwm core files included in sonoff library (#211)
 * Add PWM output control with commands PWM1 to PWM5 using user selectable GPIOs (#211)
 * Fix exceptions due to low values of commands HlwPCal (10000), HlwUCal (1000) and HlwICal (2500) (#223)
 * Add Switch state to sensor status (#227, #233)
 * Add user configuarble GPIO to module Sonoff Touch (#228)
 * Add define WEB_PORT to user_config.h to change default web server port from 80 (#232)
 * Fix failed Ota Firmware upgrade started from Web page (#235)
 *
 * 4.0.6 20170316
 * Fix to better find device by Wifi hostname
 * Fix compile error when some I2C devices are disabled
 * Add (experimental) support for SHT1X emulating I2C (#97)
 * Add ADC to ElectroDragon (#203)
 * Add support for Sonoff Dev (#206)
 *
 * 4.0.5 20170314
 * Add command Status 11 to show power status with Vcc if define USE_ADC_VCC is enabled (default)
 * Add ADC input to Sonoff SV and Wemos D1 mini - Needs recompile with define USE_ADC_VCC disabled (#137)
 * Add MQTT host:port to timeout message (#199)
 *
 * 4.0.4 20170312
 * Add pulse timers for up to 4 relays (#106)
 * Fix Sonoff Led power state when dimmer or color is 0 (#176)
 * Add command NtpServer<x> to configure up to three NTP servers (#177)
 * Delete module User Test as module Wemos D1 mini has same/more user configurable GPIO (#178)
 * Add more user configurable GPIO to module ElectroDragon (#183)
 *
 * 4.0.3 20170309
 * Renamed Module NodeMCU to WeMos D1 mini
 * Add GPIO1 as user option to some modules
 * Add Buttons, Relays and Leds to user configurable options (#159)
 * Add description on Module parameters web page to some well known GPIOs (#107, #171)
 *
 * 4.0.2 20170308
 * Restore correct seriallog level after Serial logging was disabled
 * Add simple dimmer slider to Sonoff Led web page
 * Reduced root webpage size by 31%
 * Expand Status 2 with Build date/time and core version
 * Fix webserver redirection when not in WifiManager mode (#156)
 * Add command ButtonRestrict On/Off to restrict access to button hold and button multi press options above 2 (#161)
 * Fix DS18S20 negative temperature readings (#165)
 * Fix crlf compilation error due to bad syntax (#144, #167)
 *
 * 4.0.1 20170305
 * Fix char default sizes and set MESSZ to 360 (#143)
 * Fix SerialLog setting status
 * Disable syslog when emulation is active
 * Add DS18B20 web page display refresh
 *
 * 4.0.0 20170303
 * Add define to remove config migration code for versions below 3.0 (See Wiki-Upgrade-Migration path)
 * Free memory by switching from String to char[]
 * Raised Sonoff Led PWM frequency from 200Hz to 432Hz in search of stability (hardware watchdog timeouts) (#122)
 * Increase message size and suggested minimum MQTT_MAX_PACKET_SIZE to 512 (#114, #124)
 * Remove runtime warning message regarding MQTT_MAX_PACKET_SIZE too small as it is now moved to compile time (#124)
 * Fix possible panics with web console and http commands while UDP syslog is active (#127)
 * Add optional static IP address (#129)
 * Add define ENERGY_RESOLUTION in user_config.h to allow user control over precision (#136)
 *
 * 3.9.22 20170228
 * Update web console
 * Fix Status 4 JSON message
 * Add Exception info during restart if available
 * Add osWatch service to detect loop hangs that might happen during (OTA) upgrades
 * Add WiOn support for relay and switch only (#82, #102)
 * Allow for user specified relay count up to four in sonoff_template.h (#109)
 * Add support for HTU21 compatible I2C sensors SI7013, SI7020 and SI7021 (#118)
 * Add NodeMCU or Wemos configuration option (#119)
 *
 * 3.9.21 20170224
 * Add ajax to web root page and web console (#79)
 * Add commands SwitchMode1..4 and enable user switches 2, 3 and 4 (#84, #88)
 * Fix MQTT upgrade when webserver is active
 *
 * 3.9.20 20170221
 * Add minimal basic authentication to Web Admin mode (#87)
 * Fix Hue and add HSB support (#89)
 * 
 * 3.9.19 20170219
 * Sonoff Led: Made GPIO04, 05 and 15 available for user
 * Sonoff Led: Add commands Fade, Speed, WakupDuration, Wakeup and LedTable
 *
 * 3.9.18 20170218
 * Fix ledstate 0 to turn off led
 * Fix Sonoff Led dimmer range (#16)
 * Change Sonoff Led command Dimmer to act on both cold and warm color
 * Add Sonoff Led command Color CCWW where CCWW are hexadecimal values fro 00 - FF
 * Reduce Sonoff Led flickering by disabling interrupts during flash save and disabling 
 *   Led during OTA upgrade and Web upload (#16)
 *
 * 3.9.17 20170217
 * Fix possible ArduinoJSON related memory fragmentation
 * Changed console logging using less memory
 * Add GPIO04 as user selectable for Sonoff Dual (#75)
 *
 * 3.9.16 20170214
 * Update latching relay handler
 * Add support for IR led using IRremoteESP8266 library (#59)
 * Add Hue argument passing using ArduinoJSON library (#59)
 *
 * 3.9.15 20170213
 * Change JSON float values from string to number according to http://json.org (#56)
 * Add support for exs latched relay module https://ex-store.de/ESP8266-WiFi-Relay-V31 (#58)
 * Add support for inverted relays
 * Changed MAX_LOG_LINES from 70 to 60 to preserve memory
 *
 * 3.9.14 20170211
 * Add False and True as alternatives for 0/Off and 1/On (#49)
 * Fix Status10 JSON format (#52)
 * Fix DS18x20 using OneWire library (#53)
 *
 * 3.9.13 20170210
 * Add FlashChipMode to Status 4
 * Removed redundant DHT2 option and code
 * Add Sonoff SV GPIO pin 05 configuration (#40)
 * Add configuration file backup and restore via web page
 * Fix latency due to light_sleep mode even if sleep was set to zero (#50)
 *
 * 3.9.12 20170208
 * Fix compile error when webserver is disabled (#30)
 * Fix possible ESP8285 flash problem by updating Flash Chip Mode to DOUT during OTA upload
 * Fix hostname issues by not allowing user entry of string formatting and removing from user_config.h (#36)
 *
 * 3.9.11 20170204
 * Fix command I2Cscan
 * Fix not allowed spaces in Topic, ButtonTopic and SwitchTopic
 * Make all TELEMETRY, STATUS and COMMAND message topics unique (#4)
 * Advertise command topic to be used by iobroker (#299)
 * Fix butten (non)detection if no GPIO_KEY1 is defined (#13)
 * Change WeMo serialnumber from 7 decimal chars to 8 hexadecimal chars (#18)
 * Update web page with Build Date/Time, Emulation and mDNS Discovery and Advertise information (#21)
 *
 * 3.9.10 20170130
 * Add WS2812 Color Type selection (RGB or GRB) to user_config.h (#7)
 * Hue api changes to support HUE App(s) (#8)
 *
 * 3.9.9 20170130
 * Add command status 10 showing sensor data
 * Fix hlw status messages if hlw is disabled
 *
 * 3.9.8 20170130
 * Remove GPIO07 and GPIO08 from user selectable (#5)
 *
 * 3.9.7 20170129
 * Fix possible WS2812 exceptions when using emulation
 * Add command Emulation to dynamic configure Belkin WeMo and Hue Bridge for Alexa
 *
 * 3.9.6 20170129
 * Add dynamic sleep for WS2812 animation (#1)
 *
 * 3.9.5 20170128
 * Fix error message in case of wrong Domoticz command
 *
 * 3.9.4 20170127
 * Fix Sonoff Dual Relay switching (#287)
 *
 * 3.9.3 20170127
 * Add confirmation before Restart via webpage
 * Expand Domoticz Configuration webpage with Key, Switch and Sensor Index and 
 *   add commands DomoticzSwitchIdx and DomoticzSensorIdx (#86) (#174) (#219)
 * Fix default DHT11 sensor driver selection
 * Fix LedPower status after button press (#279)
 * Add command Sleep 0 - 250 mSec for optional light sleep mode to lower energy consumption (#272)
 *   (Expect overall button/key/switch misses and wrong values on Sonoff Pow)
 * Add Hue brightness extension (#281)
 * Fix Hue brightness and change to call by reference (#283)
 *
 * 3.9.2 20170124
 * Add confirmation before Reset Configuration via webpage (#244)
 * Add WS2812 features (see Wiki commands)
 *
 * 3.9.1 20170124
 * Change PowerOnState function to only trigger when Power On (and not just restart) (#238)
 * Move HLW interrupts back to RAM and make WS2812_DMA optional as it generates Exception on Pow (#264)
 * Add charset=utf-8 to webpages (#266)
 * Update Hue emulation (#268)
 * Fix status module number
 * Add support for domoticz Dimmer on Sonoff_Led and WS2812
 * Fix possible ESP8285 flash problem by updating Flash Chip Mode to DOUT during web upload
 *
 * 3.2.6a 20170120
 * Fix Sonoff Pow compile error (#255)
 * Move HLW interrupts back to ROM (Needed for WS2812 DMA interrupts)
 * Removed all IO config from user_config.h as this will be done by commands or webpage
 * Removed MessageFormat and supports JSON only except POWER/LIGHT status
 * Add command LedPower to control main led (#247)
 * Add more FriendlyNames for Hue (#254)
 * Add DMA support for WS2812 when using pin 3 while other pins work just as well in my case...
 * Add HUE emulation for Alexa (#229)
 * Add basic WS2812 support (#229)
 * Fix Wemo when MQTT is disabled (#245)
 * Revert ButtonTopic and change SwitchTopic1 - 4 to one SwitchTopic
 * Rename MqttUnits to Units
 * Add Mqtt command to enable/disable MQTT
 *
 * 3.2.2a 20170115
 * Add dynamic (Sonoff) Module, user GPIO and sensor selection (one size fits (almost) all)
 * Add support for Sonoff LED
 * Add Seriallog disable after 600 seconds for Sonoff Dual and 4 Channel
 * Add ButtonTopic2 - 4, SwitchTopic1 - 4 and SwitchRetain
 *
 * 3.2.2 20170113
 * Fix PowerOnState 2 functionality after re-applying power (#230)
 *
 * 3.2.1 20170113
 * Fix some failed command decoding (#228)
 * Removed passwords from status messages (#216)
 *
 * 3.2.0 20170111
 * Add I2C BH1750 sensor (#222)
 * Sensor rewrite preparing for online selection
 *
 * 3.1.16 20170109
 * Fix Domoticz possible error condition
 * Remove Wifi password from connection message (#216)
 * Add Configure Other menu item to web page (#209)
 * Add command FriendlyName, field Friendly Name and define FRIENDLY_NAME to be used by Alexa
 *   eliminating current use of MQTT_CLIENT_ID (#209)
 * Add friendlyname to webpage replacing former hostname
 *
 * 3.1.15 20170108
 * Fix Domoticz send key regression with Toggle command  
 *
 * 3.1.14 20170107
 * Add support for command TOGGLE (define MQTT_CMND_TOGGLE) when ButtonTopic is in use and not equal to Topic (#207)
 *
 * 3.1.13 20170107
 * Fix web console command input when SUB_PREFIX contains '/' (#152)
 * Add command response to web command (#200)
 * Add option to disable MQTT as define USE_MQTT in user_config.h (#200)
 *
 * 3.1.12 20170106
 * Add OTA retry to solve possible HTTP transient errors (#204)
 * Fix MQTT host discovery
 *
 * 3.1.11 20170105
 * Add mDNS to advertise webserver as <hostname>.local/
 *
 * 3.1.10 20170105
 * Fix ButtonTopic when SUB_PREFIX = PUB_PREFIX
 * Add workaround for possible MQTT queueing when SUB_PREFIX = PUB_PREFIX
 * Add optional MQTT host discovery using define USE_DISCOVERY in user_config.h (#115)
 *
 * 3.1.9 20170104
 * Fix Power Blink start position (toggled)
 * Change PulseTime increments: 1 .. 111 in 0.1 sec (max 11 seconds) and 112 .. 64900 in seconds (= 12 seconds until 18 hours) (#188)
 * Add support for SUB_PREFIX = PUB_PREFIX (#190)
 *
 * 3.1.8 20170103
 * Add retain flag to LWT offline and only send "tele/sonoff/LWT Offline" (#179)
 * Change retained LWT Online message to only send "tele/sonoff/LWT Online"
 *
 * 3.1.7 20161231
 * Add retained message LWT Online when sonoff makes MQTT connection (#179)
 *
 * 3.1.6 20161230
 * Add blinking using commands BlinkTime, BlinkCount and Power Blink|3|BlinkOff|4 (#165)
 *
 * 3.1.5 20161228
 * Fix serial space command exception (28)
 *
 * 3.1.4 20161227
 * Fix MQTT subscribe regression exception (3) (#162)
 * Fix serial empty command exception (28)
 *
 * 3.1.3 20161225
 * Extent Domoticz configuration webpage with optional indices (#153)
 * Fix multi relay legacy tele message from tele/sonoff/2/POWER to tele/sonoff/POWER2
 * Add support for iTead Motor Clockwise/Anticlockwise
 *
 * 3.1.2 20161224
 * Extent command PowerOnState with toggle at power on (option 2 is now option 3!) (#156)
 *
 * 3.1.1 20161223
 * Add support for Sonoff Touch and Sonoff 4CH (#40)
 * Update DomoticzIdx and DomoticzKeyIdx with relay/key index (DomoticzIdx1/DomoticzKeyIdx1)
 * Add command PowerOnState to control relay(s) at power on (#154)
 *
 * 3.1.0 20161221
 * Add Sonoff Pow measurement smoothing
 * Fix serial command topic preamble error (#151)
 * Fix 2.x to 3.x migration inconsistencies (#146)
 *
 * 3.0.9 20161218
 * Add Sonoff Pow voltage reading when relay is on but no load present (#123)
 *
 * 3.0.8 20161218
 * Add temperature conversion to Fahrenheit as option in user_config.h (TEMP_CONVERSION) (#145)
 *
 * 3.0.7 20161217
 * Add user_config_override.h to be used by user to override some defaults in user_config.h (#58)
 * Fix Sonoff Pow low power (down to 4W) intermittent measurements (#123)
 *
 * 3.0.6 20161217
 * Fix MQTT_CLIENT_ID starting with % sign as in "%06X" (#142)
 * Add auto power off after PulseTime * 0.1 Sec to relay 1 (#134)
 *
 * 3.0.5 20161215
 * Add more control over LED with command LedState options (#136, #143)
 *   LED_OFF (0), LED_POWER (1), LED_MQTTSUB (2), LED_POWER_MQTTSUB (3), LED_MQTTPUB (4), LED_POWER_MQTTPUB (5), LED_MQTT (6), LED_POWER_MQTT (7)
 * Add option WIFI_RETRY (4) to command WifiConfig to allow connection retry to other AP without restart (#73)
 *
 * 3.0.4 20161211
 * Fix intermittent Domoticz update misses (#133)
 *
 * 3.0.3 20161210
 * Fix compiler warnings (#132)
 * Remove redundant code
 * Fix Domoticz pushbutton support
 *
 * 3.0.2 20161209
 * Add pushbutton to SwitchMode (#130)
 *
 * 3.0.1 20161209
 * Fix initial config
 *
 * 3.0.0 20161208
 * Migrate and clean-up flash layout
 *   Settings from version 2.x are saved but settings from version 3.x can not be used with version 2.x
 * Change SEND_TELEMETRY_RSSI to SEND_TELEMETRY_WIFI and add AP and SSID to telemetry
 * Split long JSON messages
 * Fix inconsistent status messages
 * Fix all status messages to return JSON if enabled
 * Remove relay index in cmnd/sonoff/<relay>/POWER now changed
 *   to cmnd/sonoff/POWER for single relay units
 *   and cmnd/sonoff/POWER<relay> for multi relay units like Sonoff dual
 * Add retain option to Power/Light status controlled by command PowerRetain On|Off (#126)
 *
 * 2.1.2 20161204
 * Add support for second wifi AP (#73)
 * Update command WifiConfig
 * Fix possible WifiManager hang
 *
 * 2.1.1a 20161203
 * Fix scan for wifi networks if WeMo is enabled
 * Fix syslog setting using web page
 *
 * 2.1.1 20161202
 * Add support for ElectroDragon second relay and button (only toggle with optional ButtonTopic) (#110)
 *
 * 2.1.0 20161202
 * Add optional EXPERIMENTAL TLS to MQTT (#49)
 * Fix MQTT payload handling (#111)
 * Optimzed WeMo code
 *
 * 2.0.21a 20161201
 * Fix WeMo PowerPlug emulation
 *
 * 2.0.21 20161130
 * Add Belkin WeMo PowerPlug emulation enabled with USE_WEMO_EMULATION in user_config.h (Heiko Krupp) (#105, #109)
 *
 * 2.0.20 20161130
 * Relax MQTTClient naming but only allows hexadecimal uppercase numbers (#107)
 * Add I2C support with command I2CScan
 * Add I2C sensor driver for HTU21 as alternate sensor using TH10/16 connectors (Heiko Krupp) (#105)
 * Add I2C sensor driver for BMP085/BMP180/BMP280/BME280 as alternate sensor using TH10/16 connectors
 *
 * 2.0.19a 20161127
 * Add support for ButtonTopic and ButtonRetain to wall switch function
 * Add pullup to SWITCH_PIN and command SwitchMode to syntax
 *
 * 2.0.18 20161126
 * Add SUB_PREFIX multi level support allowing 'cmnd' or 'cmnd/level2/level3'
 * Add wall switch function to GPIO14 and command SwitchMode (Alex Scott) (#103)
 *
 * 2.0.17 20161123
 * Calibrate HLWPCAL from 12345 to 12530
 * Add alternative sensor driver DHT2 using Adafruit DHT library
 * Add define MESSAGE_FORMAT to user_config.h
 * Throttle console messages
 * Shorten JSON messages
 * Fix possible Panic
 * Fix User mode webserver security
 *
 * 2.0.16 20161118
 * Add alternative sensor driver DS18x20 using OneWire library (#95)
 * Change sensor MQTT message from tele/sonoff/TEMPERATURE to tele/sonoff/DHT/TEMPERATURE or
 *   tele/sonoff/DS18B20/TEMPERATURE or tele/sonoff/DS18x20/1/TEMPERATURE
 * Add sensors to root webpage and auto refresh every 4 seconds (#92)
 * Add optional JSON messageformat to all telemetry data
 * Enforce minimum TelePeriod to be 10 seconds
 * Fix Energy Yesterday reset after restart
 * Add Energy Today restore after controlled restart
 *
 * 2.0.15 20161116
 * Change TODAY_POWER and PERIOD_POWER to TODAY_ENERGY and PERIOD_ENERGY
 * Fix serial regression
 * Fix syslog hangs when loghost is unavailable
 *
 * 2.0.14 20161115
 * Add HLW threshold delay
 * Fix HLW intermittent current deviation
 * Fix button functionality during wificonfig
 * Add CRC check to DS18B20 sensor (#88)
 *
 * 2.0.13 20161113
 * Add additional upload error code descriptions
 * Add PlatformIO support (#80)
 *
 * 2.0.12 20161113
 * Fix Serial and Web response regression when no MQTT connection available
 * Fix Sonoff Dual power telemetric data for second relay
 * Removed MQTT password from Information web page
 * Hide MQTT password from Configure MQTT web page
 *
 * 2.0.11 20161111
 * Rewrite button and web toggle code
 * Fix NTP sync
 * Add HLW calibration commands HLWPCAL, HLWUCAL and HLWICAL (need define USE_POWERCALIBRATION)
 * Fix power threshold tests
 *
 * 2.0.10 20161109
 * Add additional Domoticz define (#63)
 * Add defines MQTT_STATUS_ON and MQTT_STATUS_OFF in user_config.h to select status On/Off string
 * Fix status response differences (#65)
 * Fix divide by zero exception (#70)
 * Fix syslog loop exception
 *
 * 2.0.9 20161108
 * clarify MODULE in user_config.h
 * Fix hlw false values
 *
 * 2.0.8 20161108
 * Add initial status after power on
 * Seperate driver files
 * Fix hlw code and calibrate Pow
 * Move user config defines to user_config.h (#61)
 *
 * 2.0.7 20161030
 * Make Ticker mandatory
 * Add Domoticz support (Increase MQTT_MAX_PACKET_SIZE to 400) (#54)
 * Add command MessageFormat 0|1 to select either legacy or JSON output
 *
 * 2.0.6 20161024
 * Add Sonoff Pow power factor
 * Initial support for up to four relays using iTEAD PSB (4Channel)
 *   - Currently only supports one button (All buttons behave the same)
 *   - Use command MODEL 4 to select four relay option
 *     (After first power on it will support 2 relays like Sonoff Dual)
 * Fix ledstate
 * Add command Status 9 to display Sonoff Pow thresholds
 * Add commands PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow and CurrentHigh for use
 *   with Sonoff Pow thresholds
 *
 * 2.0.5 20161018
 * Add updates to user_config.h - moved SEND_TELEMETRY_DS18B20 and SEND_TELEMETRY_DHT to module area.
 *   As Sonoff TH10/16 does not have the logic installed for GPIO04 You'll have to select ONE of both
 * Add Sonoff Pow support (experimental until Pow tested)
 * Add command Status 8 to display Sonoff Pow energy values
 * Add command MqttUnits On|Off to add units to values
 * Change web main page header character size
 * Change On/Off to ON/OFF status messages to satisfy openHAB
 * Change TEMP to TEMPERATURE and HUM to HUMIDITY
 *
 * 2.0.4 20161009
 * Add MQTT_BUTTON_RETAIN, SAVE_DATA and SAVE_STATE defines to user_config.h (#35)
 * Update ButtonRetain to remove retained message(s) from broker when turned off
 * Add Retain for second relay on Sonoff Dual
 * Provide power status messages with device topic index if requested
 *
 * 2.0.3 20161008
 * Update wifi initialization
 * Add command BUTTONRETAIN for optional MQTT retain on button press (#35)
 * Add command SAVESTATE to disable power state save. May be used with MQTT retain
 *
 * 2.0.2 20161006
 * Fix wifi issue 2186
 *
 * 2.0.1 20161002
 * Fix button press
 *
 * 2.0.0 20161002
 * Update Sonoff TH10/16 sensor pins (My TH10 only has GPIO14 connected)
 * Add full support for Sonoff dual
 *
 * 1.0.35 20160929
 * Add more lines to console
 * Add timeout and disable MQTT on web upload
 * Add command SAVEDATA to control parameter save (for flash wear afficionados) (#30)
 *
 * 1.0.34 20160926
 * Fix button press six and seven
 * Add more information to webserver
 *
 * 1.0.33 20160915
 * Better WPS error message
 * Separate webserver code from support.ino into webserver.ino
 * Fix webserver User by removing unwanted restart option
 *
 * 1.0.32 20160913
 * Add Wifi Protected Setup (WPS) as third option for initial config
 * Add command WIFICONFIG replacing deprecated command SMARTCONFIG
 * Add option WIFICONFIG 3 to start WPSconfig
 * Add option WIFICONFIG 0 to start saved Wifi config tool (WPSconfig, Smartconfig or Wifimanager)
 * Change button behaviour - See Wiki
 *
 * 1.0.31 20160907
 * Fix DS18B20 misread if teleperiod = 2
 * Tuned sensor code
 * Updated prefered ElectroDragon connection to Relay 1 and Button 1
 * Moved SONOFF and ELECTRO_DRAGON port config to user_config.h
 *
 * 1.0.30 20160902
 * Fix command TELEPERIOD 0
 * Add ESP- tag to UDP log message for easy rsyslogd filtering
 * Add ElectroDragon (Relay 2 only) functionality. Select with #define MODULE ELECTRO_DRAGON
 * Add ? as null message alternative
 * Add DHT temperature and humidity telemetry support. Enable with #define SEND_TELEMETRY_DHT
 * Add DS18B20 temperature telemetry support. Enable with #define SEND_TELEMETRY_DS18B20
 * Restrict HOSTNAME, MQTTCLIENT, TOPIC and BUTTONTOPIC in topic mode only
 *
 * 1.0.29 20160831
 * Allow UPGRADE, OTAURL, RESTART, RESET, MQTTHOST, MQTTPORT, MQTTUSER, MQTTPASSWORD and WEBSERVER also in group mode
 *
 * 1.0.28 20160831
 * Add webserver state to status 5
 * Add optional PUB_PREFIX2 (tele) for telemetry usage
 * Add command TELEPERIOD
 * Fix syntax message
 * Change memory status display
 *
 * 1.0.27 20160831
 * Add sketch flash size
 * Add console to webserver
 * Add command weblog
 * Change WifiManager web pages to minimal
 * Change display default hostname and MQTT client id in webserver
 * Change HTTP command interface to http://sonoff-1234/cm?cmnd=light 2
 * Change HEARTBEAT to UPTIME
 *
 * 1.0.26 20160829
 * Add define USE_WEBSERVER to disable web server code in source
 * Add file upload as alternative for ota upload to webserver
 * Add information to webserver
 * Add command hostname
 * Add command logport
 * Change HTTP command interface to http://sonoff-1234/cmd?cmnd=light 2
 * Change button behaviour with regards to Smartconfig and OTA upload. See README.md
 * Enforce default hostname to either "%s-%04d" or user defined without any %
 * Enforce default mqtt client id to either "DVES_%06X" or user defined without any %
 *
 * 1.0.25 20160822
 * Remove config system halts to keep ota available
 *
 * 1.0.24 20160821
 * Add test for MQTT_SUBTOPIC
 * Change log range to LOG_LEVEL_ALL
 * Change MQTT introduction messages
 * Moved MQTT_MAX_PACKET_SIZE warning message to introduction messages
 *
 * 1.0.23 20160821
 * Add option USE_SPIFFS to move config from flash to spiffs
 * Add webserver with options 0 (off), 1 (user) and 2 (admin)
 * Add HTTP command interface (http://sonoff-1234/c?cmnd=light 2)
 * Add wifimanager countdown counter
 * Add command line webpage
 * Add relay control to wifimanager
 * Add restart option 99 to force restart
 * Fix wifi hostname
 * Fix NETBIOS hostname problem by reducing default hostname length
 * Fix possible exception if WIFI_HOSTNAME is changed
 * Fix upgrade messages
 * Reduce memory use by redesigning config routines
 * Split syntax message
 * Rename define SERIAL_IO to USE_SERIAL
 *
 * 1.0.22 20160814
 * Add all MQTT parameters for configuration
 * Add wifimanager to configure Wifi and MQTT via web server
 * Change NTP time handling
 * Fix Smartconfig parameter buffer overflow
 * Fix PlatformIO warnings
 *
 * 1.0.21 20160808
 * Remove semaphore as subscription flooding (more than 15 subscriptions per second) is managed by SDK (LmacRxBlk:1)
 * Add optional RTC interrupt (define USE_TICKER) to keep RTC synced during subscription flooding
 * Remove heartbeatflag
 *
 * 1.0.20 20160805
 * Add semaphore to handle out of memory when too many subscriptions requested
 * Use Daylight Saving (DST) parameters from user_config.h when timezone = 99
 * Add status 7 option displaying RTC information
 * Add ledstate to status 0
 *
 * 1.0.19 20160803
 * Fix possible MQTT_CLIENT_ID induced Exception(28)
 *
 * 1.0.18 20160803
 * Moved Cfg_Default
 * Fix negative data handling
 * Remove MQTT information from status 1 and add labels to status 1
 * Add mac address to status 5
 * Add MQTT ClientId, UserId and Password to status 6
 *
 * 1.0.17 20160731
 * Better variable range checking
 * Change ambiguous connection messages
 * Add timestamp to serial message
 *
 * 1.0.16 20160729
 * Moved wifi, rtc, syslog and config to support.ino
 * Fixed button action when buttontopic is used. Introduced with 1.0.15
 * Better buffer overflow checks (strlcpy)
 *
 * 1.0.15 20160728
 * Removed pubsubclient config changes from sonoff.ino as it doesn't work
 *   reapply MQTT_MAX_PACKET_SIZE 256 and MQTT_KEEPALIVE 120 to PubSubClient.h
 * Add status 0 option displaying all status messages
 * Change MQTT_MAX_PACKET_SIZE from 1024 to 256
 * Add buffer overflow checks (snprintf and strncpy)
 * Implemented common string sizes
 *
 * 1.0.14 20160722
 * Seperate user config from sonoff.ino to user_config.h (pucebaboon)
 * Change defaults from sidnas2 to domus1
 * Add MQTT status message as status 6 (pucebaboon)
 * Add status type to message (pucebaboon)
 * Add pubsubclient config changes to sonoff.ino (pucebaboon)
 *
 * 1.0.13 20160702
 * Add Ledstate 1 option to show power state on led
 *
 * 1.0.12 20160529
 * Allow disable of button topic using "0"
 *
 * 1.0.11 20160524
 * Provide button response if MQTT connection lost
 *
 * 1.0.10 20160520
 * Add optional button topic to assist external MQTT clients
 * Change version notation
 * Reset default values
 *
 * 1.0.9  20160503
 * Add more blinks
 * Add reset 2 option erasing flash
 * Add status 5 option displaying network info
 * Add syslog check for Wifi connection
 * Resize mqtt_publish log array
 * Change Wifi smartconfig active from 100 to 60 seconds
 * Update Wifi initialization
 *
 * 1.0.8  20160430
 * Remove use of Wifi config data from SDK
 * Add status 3 (syslog info) and status 4 (flash info)
 * Add restart option to button (5 quick presses)
 *
 * 1.0.7  20160420
 * Add UDP syslog support
 * Change HOST command to MQTTHOST command
 * Add commands SYSLOG, SERIALLOG and LOGHOST
 * Change hostname to lower case to distinguise between open-sdk version
 * Add support for ESP-12F used in my modified wkaku power socket switch
 * Fix timezone command
 * Add RTC month names for future use
 * Modify button code
 * Remove initialization errors by better use of MQTT loop
 *
 * 1.0.6  20160406
 * Removed Wifi AP mode (#1)
 * Add test for Arduino IDE version >= 1.6.8
 * Fix RTC time sync code
 *
 * 1.0.5  20160310
 * Initial public release
 * Show debug info by selecting option from IDE Tools Debug port: Serial
 */
