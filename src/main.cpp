#include <TTN_esp32.h>
#define MYLED 25

/***************************************************************************
 *  Go to your TTN console register a device then the copy fields
 *  and replace the CHANGE_ME strings below
 ****************************************************************************/
const char *devEui = "0000000000000001";                 // Change to TTN Device EUI
const char *appEui = "70B3D57ED0030CD0";                 // Change to TTN Application EUI
const char *appKey = "2FBD3BAEFF80F0A8B0E9589B985B905C"; // Chaneg to TTN Application Key

static uint8_t mydata[] = "DAS0001";

TTN_esp32 ttn;

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
  pinMode(MYLED, OUTPUT);
  ttn.begin();
  ttn.onMessage(message); // Declare callback function for handling downlink
                          // messages from server
  Serial.println("Ready to join...");
  ttn.join(devEui, appEui, appKey);
  Serial.print("Joining TTN ");
  while (!ttn.isJoined())
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\njoined !");
  ttn.showStatus();
}

void loop()
{
  static float nb = 1;
  nb += 1;

  if (ttn.sendBytes(mydata, sizeof(mydata) - 1))
  {
    Serial.print("mydata sent: ");
    Serial.println(nb);
  }
  digitalWrite(MYLED, HIGH);
  delay(1000);
  digitalWrite(MYLED, LOW);
  delay(10000);
}