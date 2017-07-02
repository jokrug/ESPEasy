/*

  This plug in is written by Joachim (https://github.com/jokrug)
  Plugin is based upon SenseAir plugin by Daniel Tedenljung info__AT__tedenljungconsulting.com

  This plugin reads the power and energy values from a EasyMeter powermeter.
  The EasyMeter sends the OBIS- protokol. http://www.easymeter.com/produkt/Q3D
  DevicePin1 - is RX for ESP
*/

#ifdef PLUGIN_BUILD_TESTING

#define PLUGIN_157
#define PLUGIN_ID_157         157
#define PLUGIN_NAME_157       "EasyMeter power meter [TESTING]"
#define PLUGIN_VALUENAME1_157 "Energy in" //
#define PLUGIN_VALUENAME2_157 "Energy out" //
#define PLUGIN_VALUENAME3_157 "Power L1" //
#define PLUGIN_VALUENAME4_157 "Power L2" //
#define PLUGIN_VALUENAME5_157 "Power L3" //
#define PLUGIN_VALUENAME6_157 "Power total"
#define EASYMETER_READ_TIMEOUT   2000

boolean Plugin_157_init = false;

#include <SoftwareSerial.h>
SoftwareSerial *Plugin_157_SoftSerial;

boolean Plugin_157(byte function, struct EventStruct *event, String& string)
{
  bool success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_157;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].VType = SENSOR_TYPE_HEXA;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = true;
        Device[deviceCount].ValueCount = 6;
        Device[deviceCount].SendDataOption = true;
        Device[deviceCount].TimerOption = true;
        Device[deviceCount].GlobalSyncOption = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_157);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_157));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_157));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_157));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[3], PSTR(PLUGIN_VALUENAME4_157));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[4], PSTR(PLUGIN_VALUENAME5_157));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[5], PSTR(PLUGIN_VALUENAME6_157));
        break;
      }

    case PLUGIN_INIT:
      {
        //SoftwareSerial(int receivePin, int transmitPin, bool inverse_logic = false, unsigned int buffSize = 64);
        Plugin_157_SoftSerial = new SoftwareSerial(Settings.TaskDevicePin1[event->TaskIndex],
                                                  0, false, 230);
        Plugin_157_SoftSerial->begin(9600, 7);
        addLog(LOG_LEVEL_INFO, F("SmartMeter: Init OK "));

        //delay first read, because hardware needs to initialize on cold boot
        //otherwise we get a weird value or read error
        timerSensor[event->TaskIndex] = millis() + 15000;

        Plugin_157_init = true;
        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {
      }

    case PLUGIN_READ:
      {
        if (!Plugin_157_init)
          break;

        // get response
        char recv_lineBuf[50];
        recv_lineBuf[0] = 0;
        long start = millis();
        int counter = 0;
        while (((millis() - start) < EASYMETER_READ_TIMEOUT) &&
               (counter < sizeof(recv_lineBuf)))
        {
          if (Plugin_157_SoftSerial->available() > 0)
          {
            byte b = Plugin_157_SoftSerial->read();
            if((b == '\n') || (b == '\r')) {
              recv_lineBuf[counter] = 0;
              if(counter > 10) //valid string is at least 10 bytes long.
                if(parseEasyMeterString(event, recv_lineBuf))
                {
                  success = true;
                  break;
                }
              counter = 0;
            } else
              recv_lineBuf[counter++] = b;
          } else {
            delay(10);
          }
        }
        if ((millis() - start) >= EASYMETER_READ_TIMEOUT){
            addLog(LOG_LEVEL_INFO, F("EasyMeter: Error, timeout while trying to read"));
        }
        break;
      }
  }
  return success;
}

/********************************************************************************************\
  Parse a command string to event struct
  \*********************************************************************************************/
bool parseEasyMeterString(struct EventStruct *event, char* byteStr)
{
/* example:
1-0:0.0.0*255(0272160535233)
1-0:1.8.0*255(00000080.6357332*kWh)
1-0:2.8.0*255(00000801.4717444*kWh)
1-0:21.7.0*255(000070.09*W)
1-0:41.7.0*255(000075.39*W)
1-0:61.7.0*255(000435.91*W)
1-0:1.7.0*255(000581.39*W)
1-0:96.5.5*255(80)
*/
  String string = String(byteStr);
  addLog(LOG_LEVEL_DEBUG, string);

  if(string.startsWith("1-0:1.8.0")) //energy in
    UserVar[event->BaseVarIndex] = getValueFromString(string);
  else if(string.startsWith("1-0:2.8.0")) //energy out
    UserVar[event->BaseVarIndex+1] = getValueFromString(string);
  else if(string.startsWith("1-0:21.7.0")) //power L1
    UserVar[event->BaseVarIndex+2] = getValueFromString(string); //L1
  else if(string.startsWith("1-0:41.7.0")) //power L2
    UserVar[event->BaseVarIndex+3] = getValueFromString(string); //L2
  else if(string.startsWith("1-0:61.7.0")) //powr L3
    UserVar[event->BaseVarIndex+4] = getValueFromString(string); //L3
  else if(string.startsWith("1-0:1.7.0")) //power total
    UserVar[event->BaseVarIndex+5] = getValueFromString(string); //power total
  else if(string.startsWith("1-0:96.5.5")) //status
    return true; //Final string of a data block.

  return false;
}

double getValueFromString( String &string)
{// 1-0:1.7.0*255(000581.39*W): extract and convert number between (... and *
  int start = string.indexOf('(');
  int end   = string.indexOf('*', start);
  if(string.charAt(start+1) == '-') //toFloat doesn't convert negative numbers
    return string.substring(start+2, end).toFloat()*-1;
  else
    return string.substring(start+1, end).toFloat();
}

#endif
