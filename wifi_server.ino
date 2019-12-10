#include <ESP8266WiFi.h>

#define IN_PIN 4
#define OUT_PIN 5

// create a webserver listening on port 80
WiFiServer server(80);

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

bool alreadyConnected = false;
bool notConnected = true;
bool notAvailable = true;

void setup() {
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);

  // Start serial communication 
  Serial.begin(115200);
  delay(1000);
  Serial.println('\n');

  // Connect to the network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  // Wait for the Wi-Fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  // Send the IP address of the ESP8266 to the computer
  Serial.println(WiFi.localIP());

  // start the server
  server.begin();
  server.setNoDelay(true);
}

void loop() {
  WiFiClient client = server.available();

  // continuously send movement commands to client
  if (client) {
    if (client.connected()) {
      if (!alreadyConnected) {
        Serial.println("Master robot connected");
        alreadyConnected = true;
      }

      while (!client.available()) {}

      if (client.available() > 0) {
        // read data from client
        int command = client.read();

        if (command != -1) {
          Serial.print("Command received: ");

          switch (command) {
            case 1:
              Serial.println("Forward");
              digitalWrite(OUT_PIN, 1);
              break;
            case 2:
              Serial.println("Backward");
              digitalWrite(OUT_PIN, 2);
              break;
            case 3:
              Serial.println("Left");
              digitalWrite(OUT_PIN, 3);
              break;
            case 4:
              Serial.println("Right");
              digitalWrite(OUT_PIN, 4);
              break;
            case 5:
              Serial.println("Stop");
              digitalWrite(OUT_PIN, 5);
              break;
            case 6:
              Serial.println("No command");
            default:
              break;
          }
        } else {
          Serial.println("No command received");
        }
      } else {
        alreadyConnected = false;
        client.stop();
      }
    } else {
      if (notConnected) {
        Serial.println("No client connected");
        notConnected = false;
      }
    }
  } else {
    if (notAvailable) {
      Serial.println("No client available");
      notAvailable = false;
    }
  }
}
