#include <EEPROM.h>
#include "robert_link.h"
#include "line_detection.h"

enum Inputs
{
  ADJUST_MAX = 'm',
  ENTER_PID = 'a',
  HELP = 'h'
};

int SerialIntroMessage()
{
  Serial.println("Welcome to Robert uwu....");
  // Serial.println("adjust max speed: m <float from 0 to 1>");
  SerialHelpMessage();
}

int SerialHelpMessage()
{
  
  Serial.println("slowCounter: s <int>");
  Serial.println("stopCounter: n <int>");
  Serial.println("stop timer: l <int (ms)>");

  print_motor_speed();
  print_PID();
  Serial.println(String("slow marker") + (int)slowMarker + String(", stop marker:") + (int)stopMarker);
  Serial.println(String("stop timer") + (int)max_stop_counter);
}

char *binString(unsigned short n)
{
  static char bin[17];
  int x;

  for (x = 0; x < 16; x++)
  {
    bin[x] = n & 0x8000 ? '1' : '0';
    n <<= 1;
  }
  bin[16] = '\0';

  return (bin);
}

void change_motor()
{
  float input = Serial.parseFloat();
  int num = int(MAX_MOTOR_SPEED * input);
  int old_motor = set_motor_speed(num);
  String toPrintM = String("Changed motor speed to ") + num + String(", Old motor speed = ") + old_motor;
  Serial.println(toPrintM);
}

void change_pid()
{
  Serial.println("in pid");
  float float1 = Serial.parseFloat();
  // char interchars[16];
  // Serial.readBytesUntil(',', interchars, 0);
  float float2 = Serial.parseFloat();
  float float3 = Serial.parseFloat();
  // char toPrint[50];
  // snprintf(toPrint, sizeof(toPrint), "Floats: %4.1f", float1);
  set_PID_constants(float1, float2, float3);
  print_PID();
  // String toPrint = String("Changed motor speed to ") + float1 + String(",") + float2;
  // Serial.println(toPrint);
}

void change_slowMarker()
{
  Serial.println("Changing slow marker...");
  int input = Serial.parseInt();
  slowMarker = input;
  EEPROM.put(0, input);
  String toPrint = String("Changed slow marker to ") + input;
  Serial.println(toPrint);
}

void change_stopMarker()
{
  Serial.println("Changing stop marker...");
  int input = Serial.parseInt();
  stopMarker = input;
  EEPROM.put(1, input);
  String toPrint = String("Changed stop marker to ") + input;
  Serial.println(toPrint);
}

void change_stop_counter()
{
  Serial.println("Changing stop counter...");
  int input = Serial.parseInt();
  max_stop_counter = input;
  EEPROM.put(2, input);
  String toPrint = String("Changed stop marker to ") + input;
}

void get_eeprom_vars()
{
  EEPROM.get(0, slowMarker);
  EEPROM.get(1, stopMarker);
  EEPROM.get(2, max_stop_counter);
}

int acceptSerialInput()
{
  if (Serial.available() == 0)
    return 0;

  char saved = Serial.read();
  // Serial.println(saved);
  switch (saved)
  {
  case 'h':
    Serial.println("in help");
    SerialHelpMessage();
    break;
  case 'm':
    change_motor();
    break;
  case 'n':
    change_stopMarker();
    break;
  case 'l':
    change_stop_counter();
    break;

  case 'p':
    change_pid();
    break;
  case 's':
    change_slowMarker();
    break;
  case 'e':
    return 1;

  case '\n':
    break;
  default:
    Serial.println("In default");
    break;
  }

  return 0;

  // String read_response = Serial.readString();
  // Serial.println(read_response);

  // https://wokwi.com/projects/330808386186642002

  // EEPROM https://wokwi.com/projects/333980928556663378

  // // To split string look at https://forum.arduino.cc/t/how-to-split-a-string-with-space-and-store-the-items-in-array/888813/8
  // // Use str.indexOf or strtok
  // char selectedInput = read_response.charAt(0);
  // read_response = read_response.substring(1);
  // switch (selectedInput)
  // {
  // case ADJUST_MAX:
  //   float adjust_num = read_response.toFloat();
  //   int num = int(MAX_MOTOR_SPEED * adjust_num);
  //   set_motor_speed(num);

  //   String toPrint = String("Changed motor speed to ") + num;
  //   Serial.println(toPrint);
  //   // set_PID_constants(0.1, 0.1, 0.1);

  //   break;
  // case ENTER_PID:
  //   Serial.println("done changing constants");
  //   break;

  // default:
  //   Serial.println("Unknown input" + selectedInput);
  //   break;
  // }
}