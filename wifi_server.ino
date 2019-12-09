#include <ESP8266WiFi.h>

#define IN_PIN 4
#define OUT_PIN 5

// create a webserver listening on port 80
WiFiServer server(80); 

const char* ssid = "ORBI85";           
const char* password = "Retired2018!";     

void setup() {
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);

  // Start the Serial communication to send messages to the computer
  Serial.begin(115200);         
  delay(10);
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
}

void loop() { 
  WiFiClient client = server.available();

  // continuously send movement commands to client
  if (client) {
    if (client.connected()) {
      // get movement command from Arduino
      int command = digitalRead(IN_PIN);
      int data;
      
      // send command to client (other bot)
      Serial.print("Command sent: ");
      switch(command) {
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
      // send command
      server.write(data);
      
      // flush command sent to client
      client.flush();
    }
    // terminate connection with client
    client.stop();
  }
}
