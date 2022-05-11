
#include "robert_link.h"
#include "line_detection.h"

enum Inputs
{
  ADJUST_MAX = 'm',
  ENTER_PID = 'a'
};

int serialHelpMessage()
{
  Serial.println("Welcome to Robert uwu....");
  Serial.println("adjust max speed: m <float from 0 to 1>");
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

void acceptSerialInput()
{
  if (Serial.available() == 0)
    return;
  String read_response = Serial.readString();
  Serial.println(read_response);

  // To split string look at https://forum.arduino.cc/t/how-to-split-a-string-with-space-and-store-the-items-in-array/888813/8
  // Use str.indexOf or strtok
  char selectedInput = read_response.charAt(0);
  read_response = read_response.substring(1);
  switch (selectedInput)
  {
  case ADJUST_MAX:
    float adjust_num = read_response.toFloat();
    int num = int(MAX_MOTOR_SPEED * adjust_num);
    set_motor_speed(num);

    String toPrint = String("Changed motor speed to ") + num;
    Serial.println(toPrint);
    // set_PID_constants(0.1, 0.1, 0.1);

    break;
  case ENTER_PID:
    Serial.println("done changing constants");
    break;

  default:
    Serial.println("Unknown input" + selectedInput);
    break;
  }
}