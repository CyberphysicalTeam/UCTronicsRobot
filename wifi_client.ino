#include <ESP8266WiFi.h>

#define IN_PIN 4
#define OUT_PIN 5

// set the IP address of the server
// SET TO IP ADDRESS GIVEN TO SERVER BY ROUTER
IPAddress server(0, 0, 0, 0);
WiFiClient client;

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
bool detected = false;

void setup() {
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  // Wait for the Wi-Fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  // display connection details
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}

void loop() {
  // connect to server
  if (!client.connected()) {
    if (client.connect(server, 80)) {
      Serial.println("Connected to slave robot");
    } else {
      Serial.println("Slave robot not detected");
    }
  }

  while (client.connected()) {
    // get movement command from Arduino
    int command = digitalRead(IN_PIN);
    int data;

    // send command to client (other bot)
    Serial.print("Command sent: ");
    switch (command) {
      case 1:
        Serial.println("Forward");
        data = 1;
        break;
      case 2:
        Serial.println("Backward");
        data = 2;
        break;
      case 3:
        Serial.println("Left");
        data = 3;
        break;
      case 4:
        Serial.println("Right");
        data = 4;
        break;
      case 5:
        Serial.println("Stop");
        data = 5;
        break;
      default:
        Serial.println("No command");
        data = 6;
        break;
    }
    // send command to server
    client.write(data);
  }
}
