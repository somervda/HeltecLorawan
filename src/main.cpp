#include <TTN_esp32.h>
#include "heltec.h"

#define MYLED 25
#define MYBTN 0

#define BAND 915E6 //you can set band here directly,e.g. 868E6,915E6

/***************************************************************************
 *  Go to your TTN console register a device then the copy fields
 *  and replace the CHANGE_ME strings below
 ****************************************************************************/
const char *devEui = "0000000000000001";                 // Change to TTN Device EUI
const char *appEui = "70B3D57ED0030CD0";                 // Change to TTN Application EUI
const char *appKey = "2FBD3BAEFF80F0A8B0E9589B985B905C"; // Chaneg to TTN Application Key

TTN_esp32 ttn;

struct sensor_data_struct
{
  uint32_t count;
  uint32_t hall;
  uint16_t data1;
  uint8_t data2;
  uint8_t data3;
  uint32_t data4;
};

float join_count;
int tx_delay = 10;

static sensor_data_struct my_sensor_data;

void message(const uint8_t *payload, size_t size, int rssi)
{
  Serial.println("-- MESSAGE");
  Serial.print("Received " + String(size) + " bytes RSSI=" + String(rssi) + "db");
  for (int i = 0; i < size; i++)
  {
    Serial.print(" " + String(payload[i]));
    // Serial.write(payload[i]);
  }

  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");
  join_count = 0;
  pinMode(MYLED, OUTPUT);
  pinMode(MYBTN, INPUT);

  // https://github.com/HelTecAutomation/Heltec_ESP32/blob/master/src/oled/API.md
  Heltec.display->init();
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->drawString(0, 0, "HeltecLoRaWan V0.53");
  Heltec.display->display();
  // 10 second delay before joining to ensure
  // a spurious join not performed when updating firmware.
  // TTN backend seems to get confused with too many joins
  delay(10000);

  // See ttn functionality https://www.thethingsnetwork.org/docs/devices/arduino/usage.html

  ttn.begin();
  ttn.onMessage(message); // Declare callback function for handling downlink
                          // messages from server
  Serial.println("Ready to join...");
  ttn.join(devEui, appEui, appKey);
  Serial.print("Joining TTN ");

  while (!ttn.isJoined())
  {
    Serial.print(".");
    join_count += 0.5;
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Joining ");
    Heltec.display->drawString(70, 0, String(join_count));
    Heltec.display->display();

    delay(500);
  }
  Serial.println("\njoined !");
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Joined!");
  Heltec.display->display();
  ttn.showStatus();
}

void loop()
{

  my_sensor_data.count += 1;
  my_sensor_data.hall = hallRead();
  my_sensor_data.data1 = 513;
  my_sensor_data.data2 = 18;
  my_sensor_data.data3 = 7;
  my_sensor_data.data4 = 514;

  // Blink led before sending data
  for (size_t i = 0; i < 5; i++)
  {
    digitalWrite(MYLED, HIGH);
    delay(100);
    digitalWrite(MYLED, LOW);
    delay(100);
  }
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Sending: ");
  Heltec.display->drawString(50, 0, String(my_sensor_data.count));
  Heltec.display->drawString(85, 0, "@");
  Heltec.display->drawString(100, 0, String(tx_delay));
  Heltec.display->display();

  // Note you can set the SF here - see API https://www.thethingsnetwork.org/docs/devices/arduino/api/network.html
  if (ttn.sendBytes((uint8_t *)&my_sensor_data, sizeof(my_sensor_data)))
  {
    Serial.print("mydata sent: ");
    Serial.println(my_sensor_data.count);
  }

  // long blink after sending data
  digitalWrite(MYLED, HIGH);
  delay(1000);
  digitalWrite(MYLED, LOW);
  // overall loop delay
  for (size_t i = 0; i < tx_delay; i++)
  {
    // Check to see if prog button is pressed every second
    // Change the
    if (digitalRead(MYBTN) == 0)
    {
      if (tx_delay == 10)
      {
        tx_delay = 60;
        i = tx_delay;
      }
      else if (tx_delay == 60)
      {
        tx_delay = 600;
        i = tx_delay;
      }
      else if (tx_delay == 600)
      {
        tx_delay = 10;
        i = tx_delay;
      }
      Heltec.display->clear();
      Heltec.display->drawString(0, 0, "Delay Sec : ");
      Heltec.display->drawString(90, 0, String(tx_delay));
      Heltec.display->display();
      for (size_t i = 0; i < 8; i++)
      {
        digitalWrite(MYLED, HIGH);
        delay(50);
        digitalWrite(MYLED, LOW);
        delay(50);
      }
    }
    delay(1000);
  }
}