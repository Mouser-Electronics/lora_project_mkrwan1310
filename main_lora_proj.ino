#include <MKRWAN.h>

LoRaModem modem;

unsigned char buffer[64];
bool rfid_match = false;
String employee_name;
int count;

// dataset - replace with your tag IDs
char tag1[13] = "";
char tag2[13] = "";
char tag3[13] = "";
char tag4[13] = "";
char tag5[13] = "";

// insert your info
String appEui = "";
String appKey = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600);
  while (!Serial);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  // connect to TTN
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  // set poll interval to 60 secs.
  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
}

void loop() {
  // all delays are for stabilty reasons
  delay(10000);

  Serial.println();
  Serial.println("Please place your rfid tag on the sensor");

  while (!Serial1.available()) {
  }

  if (Serial1.available())
  {
    delay(1000);
    // read tag information while tag is at maximum 7cm away from reader
    while (Serial1.available())
    {
      buffer[count++] = Serial1.read();
      if (count == 64)break;
    }

    delay(1000);

    // compare tag information with dataset and replace names
    if (compareTag(buffer, tag1)) {
      employee_name = "John";
    } else if(compareTag(buffer, tag2)) {
      employee_name = "Sarah";
    } else if(compareTag(buffer, tag3)) {
      employee_name = "Nora";
    } else if(compareTag(buffer, tag4)) {
      employee_name = "Jennifer";
    } else if(compareTag(buffer, tag5)) {
      employee_name = "Paul";
    } else {
      Serial.println("Error - unknown rfid tag");
    }

    // if match in dataset found -> add to spreadsheet over TTN
    if (rfid_match == true) {
      Serial.println("Employee Name: " + employee_name);
      String msg = employee_name;

      Serial.println();
      Serial.print("Adding " + msg + " to spreadsheat via TTN");
      Serial.println();

      delay(5000);

      int err;
      modem.beginPacket();
      modem.print(msg);
      err = modem.endPacket(true);
      if (err > 0) {
        Serial.println("Employee added!");
      } else {
        Serial.println("Error sending message :(");
        Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
        Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
      }
    }
    clearBufferArray();
    count = 0;
    rfid_match = false;
  }
}

// reset buffer function
void clearBufferArray()
{
  for (int i = 0; i < count; i++)
  {
    buffer[i] = NULL;
  }
}

// return true and set rfid_match = true if match found
boolean compareTag(unsigned char one[], char two[]) {
  for (int j = 0; j < 12; j++) {
    if (one[j + 1] != two[j]) return false;
  }
  rfid_match = true;
  return true;
}
