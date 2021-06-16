// This example uses an ESP8266 or ESP32
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://www.shiftr.io/try.
//
// by Joël Gähwiler, Soren Kristensen

#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char ssid[] = "ssid";
const char pass[] = "pass";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "public", "public")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(MQTTClient *client, char topic[], char bytes[], unsigned int len, unsigned int total_len, unsigned int index) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - payload len:");
  Serial.print(len);
  Serial.print(" total_len:");
  Serial.print(total_len);
  Serial.print("index:");
  Serial.println(index);
  //A large message is broken up into several message received calls - same topic and index shows the start pointer for the chunk
  //total_len is the length of the entire message, len is length of data in this chunk
  //small messages have len=total_len and index=0 (hence one message call for small messages)
  
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("public.cloud.shiftr.io", net);
  client.onMessageAdvanced(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
}
