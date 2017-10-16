//#######################################################################################################
//################################## Plugin 208: NOKIA lcd2 ###############################################
//#######################################################################################################

#define PLUGIN_208
#define PLUGIN_ID_208         208
#define PLUGIN_NAME_208       "Display - LCD PCD8544 [TESTING]"
#define PLUGIN_VALUENAME1_208 "Backlight"
#define PLUGIN_VALUENAME2_208 "Contrast"
#define PLUGIN_VALUENAME3_208 "Rotation"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 *lcd2;
byte lines = 6;

boolean Plugin_208(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_208 ;
        Device[deviceCount].Type = DEVICE_TYPE_I2C;
        Device[deviceCount].VType = SENSOR_TYPE_SWITCH;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 3;
        Device[deviceCount].SendDataOption = false;
        Device[deviceCount].TimerOption = true;
        break;
      }


    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_208);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_208));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_208));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_208));
        break;
      }


    case PLUGIN_WEBFORM_LOAD:
      {
        byte choice1 = Settings.TaskDevicePluginConfig[event->TaskIndex][3];
        byte choice2 = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        byte choice3 = Settings.TaskDevicePluginConfig[event->TaskIndex][2];
        byte choice4 = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
        char deviceTemplate[lines][32];
        LoadCustomTaskSettings(event->TaskIndex, (byte*)&deviceTemplate, sizeof(deviceTemplate));
        char tmpString[128];
        for (byte varNr = 0; varNr < lines; varNr++)
        {
          string += F("<TR><TD>Line ");
          string += varNr + 1;
          string += F(":<TD><input type='text' size='80' maxlength='80' name='Plugin_208_template");
          string += varNr + 1;
          string += F("' value='");
          string += deviceTemplate[varNr];
          string += F("'>");
        }

        sprintf_P(tmpString, PSTR("<TR><TD>Display Contrast(50-100):<TD><input type='text' name='plugin_208_contrast' value='%u'>"), choice2);
        string += tmpString;
        string += F("<TR><TD>Backlight pin:<TD>");
        addPinSelect(false, string, "taskdevicepin3", Settings.TaskDevicePin3[event->TaskIndex]);
        int optionValues3[4];
        optionValues3[0] = 0;
        optionValues3[1] = 1;
        optionValues3[2] = 2;
        optionValues3[3] = 3;
        String options3[4];
        options3[0] = F("0");
        options3[1] = F("90");
        options3[2] = F("180");
        options3[3] = F("270");
        string += F("<TR><TD>Display Rotation:<TD><select name='plugin_208_rotation'>");
        for (byte x = 0; x < 4; x++)
        {
          string += F("<option value='");
          string += optionValues3[x];
          string += "'";
          if (choice1 == optionValues3[x])
            string += F(" selected");
          string += ">";
          string += options3[x];
          string += F("</option>");
        }
        string += F("</select>");
        int optionValues4[2];
        optionValues4[0] = 0;
        optionValues4[1] = 1;
        String options4[2];
        options4[0] = F("ON");
        options4[1] = F("OFF");
        string += F("<TR><TD>Backlight:<TD><select name='plugin_208_backlight'>");
        for (byte x = 0; x < 2; x++)
        {
          string += F("<option value='");
          string += optionValues4[x];
          string += "'";
          if (choice4 == optionValues4[x])
            string += F(" selected");
          string += ">";
          string += options4[x];
          string += F("</option>");
        }
        string += F("</select>");

        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        int plugin1 = WebServer.arg("plugin_208_rotation").toInt();
        int plugin2 = WebServer.arg("plugin_208_contrast").toInt();
        int plugin3 = Settings.TaskDevicePin3[event->TaskIndex];
        int plugin4 = WebServer.arg("plugin_208_backlight").toInt();
        String options1[4];
        options1[0] = F("0");
        options1[1] = F("90");
        options1[2] = F("180");
        options1[3] = F("270");
        UserVar[event->BaseVarIndex+2]=options1[plugin1].toInt();
        UserVar[event->BaseVarIndex+1]=plugin2;
        UserVar[event->BaseVarIndex]=!plugin4;
        String log = F("PCD8544: ");
        if (plugin2>100) plugin2=100;
        if (plugin2<50) plugin2=50;
        if (plugin1!=Settings.TaskDevicePluginConfig[event->TaskIndex][3]){
          log += String(F("rotation changed to: ")) + plugin1;
          addLog(LOG_LEVEL_INFO, log);
          Settings.TaskDevicePluginConfig[event->TaskIndex][3] = plugin1;
          }
        if (plugin4!=Settings.TaskDevicePluginConfig[event->TaskIndex][3]){
          log += String(F("backlight is now ")) + plugin4?"off":"on";
          addLog(LOG_LEVEL_INFO, log);
          Settings.TaskDevicePluginConfig[event->TaskIndex][0] = plugin4;
          }
         if (plugin2!=Settings.TaskDevicePluginConfig[event->TaskIndex][3]){
          log += String(F("contrast is changed to: ")) + plugin2;
          addLog(LOG_LEVEL_INFO, log);
          Settings.TaskDevicePluginConfig[event->TaskIndex][1] = plugin2;
          }
        char deviceTemplate[lines][32];
        for (byte varNr = 0; varNr < lines; varNr++)
        {
          char argc[25];
          String arg = F("Plugin_208_template");
          arg += varNr + 1;
          arg.toCharArray(argc, 25);
          String tmpString = WebServer.arg(argc);
          strncpy(deviceTemplate[varNr], tmpString.c_str(), sizeof(deviceTemplate[varNr]));
        }

     //   Settings.TaskDeviceID[event->TaskIndex] = 1; // temp fix, needs a dummy value

        SaveCustomTaskSettings(event->TaskIndex, (byte*)&deviceTemplate, sizeof(deviceTemplate));
        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        if (!lcd2)
        {
          // pin 14 - Serial clock out (SCLK)
          // pin 13 - Serial data out (DIN)
          // pin 12 - Data/Command select (D/C)
          // pin 5 - LCD chip select (CS)
          // pin 4 - LCD reset (RST)
          //Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC,
          //int8_t CS, int8_t RST)
          lcd2 = new Adafruit_PCD8544(0, 16, 4, 14, -1);
		      //lcd2 = new Adafruit_PCD8544(14, 4, 16); // CS(CEN) GPIO15 , D/C (need to be connected) GPIO2 , RESET GPIO16
        }
        // Setup lcd2 display
        byte plugin1 = Settings.TaskDevicePluginConfig[event->TaskIndex][3];
        byte plugin2 = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        byte plugin4 = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
        UserVar[event->BaseVarIndex+2]=plugin1;
        UserVar[event->BaseVarIndex+1]=plugin2;
        UserVar[event->BaseVarIndex]=!plugin4;
        if (plugin1==0 || plugin1==2)
        {
          lines=6;
        }
        else{
          lines=10;
        }
        lcd2->begin();                     // initialize the lcd2
        lcd2->setContrast(plugin2);
        lcd2->setRotation(plugin1);
        lcd2->clearDisplay();
        char deviceTemplate[lines][32];
        LoadCustomTaskSettings(event->TaskIndex, (byte*)&deviceTemplate, sizeof(deviceTemplate));
        displayText(deviceTemplate);
        lcd2->display();
        if (Settings.TaskDevicePin3[event->TaskIndex] != -1){
          pinMode(Settings.TaskDevicePin3[event->TaskIndex], OUTPUT);
          digitalWrite(Settings.TaskDevicePin3[event->TaskIndex],Settings.TaskDevicePluginConfig[event->TaskIndex][0]);
          setPinState(1, Settings.TaskDevicePin3[event->TaskIndex], PIN_MODE_OUTPUT, Settings.TaskDevicePluginConfig[event->TaskIndex][3]);}
        success = true;
        break;
      }

    case PLUGIN_READ:
      {
        char deviceTemplate[lines][32];
        LoadCustomTaskSettings(event->TaskIndex, (byte*)&deviceTemplate, sizeof(deviceTemplate));
        displayText(deviceTemplate);
        success = false;
        break;
      }

    case PLUGIN_WRITE:
      {
        String tmpString  = string;
        int argIndex = tmpString.indexOf(',');
        if (argIndex)
          tmpString = tmpString.substring(0, argIndex);
        if (tmpString.equalsIgnoreCase(F("PCD8544")))
        {
          success = true;
          argIndex = string.lastIndexOf(',');
          tmpString = string.substring(argIndex + 1);
          lcd2->setCursor(event->Par2 - 1, event->Par1 - 1);
          lcd2->println(tmpString.c_str());
        }
        if (tmpString.equalsIgnoreCase(F("PCD8544CMD")))
        {
          success = true;
          argIndex = string.lastIndexOf(',');
          tmpString = string.substring(argIndex + 1);
          if (tmpString.equalsIgnoreCase(F("Clear")))
            lcd2->clearDisplay();
        }
        break;
      }

  }
  return success;
}

boolean displayText(char deviceTemplate[][32])
{
        String log = F("PCD8544: ");
        String string = F("");
        lcd2->clearDisplay();
        lcd2->setTextSize(1);
        lcd2->setTextColor(BLACK);
        lcd2->setCursor(0,0);
        for (byte x = 0; x < lines; x++)
        {
          String tmpString = deviceTemplate[x];
          if (tmpString.length())
          {
            String newString = parseTemplate(tmpString, 32);
            lcd2->setCursor(0,x*8);
            lcd2->print(newString);
            string+=newString+"\\";
          }
        }
        log += String(F("displayed text: \"")) + String(string) + String(F("\""));
        addLog(LOG_LEVEL_INFO, log);
        lcd2->display();
        return true;
    }
