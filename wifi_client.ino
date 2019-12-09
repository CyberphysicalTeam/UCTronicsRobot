#include <ESP8266WiFi.h>

#define IN_PIN 4
#define OUT_PIN 5

// set the IP address of the server
IPAddress server(192,168,1,55);
WiFiClient client;

const char* ssid = "ORBI85";           
const char* password = "Retired2018!"; 

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

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  // Send the IP address of the ESP8266 to the computer
  Serial.println(WiFi.localIP());
}

void loop() {
  // connect to server on port 80
  if (client.connect(server, 80)) {
    if (!detected) {
      Serial.println("Connected to master robot");
      detected = true;
    }

    // get command from server
    int command = client.read();
    
    // send command to Arduino received from server
    if (command > 0) {
      Serial.print("Command received: ");
      Serial.println(command);
    
      switch(command) {
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
    Serial.println("Master robot not detected");
  }
}
