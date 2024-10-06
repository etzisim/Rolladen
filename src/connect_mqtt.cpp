#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <string.h>
#include "connect_mqtt.h"

String received_mqtt_topic;
String received_mqtt_msg;
boolean received_new_mqtt_msg;

WiFiClient espClient;
PubSubClient client(espClient);
// void mqtt::send_mqtt(String, String, boolean);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  received_mqtt_topic = topic;
  received_mqtt_msg = "";

  Serial.print("Message:");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    received_mqtt_msg = received_mqtt_msg + (char)payload[i];
  }
  received_new_mqtt_msg = true;

  Serial.println();
  Serial.println("-----------------------");
}

void loop_mqtt()
{
  client.loop();
  if (!client.connected())
  {
    disconnect_mqtt();
    connect_mqtt();
    subscribe_mqtt("Rollos/#");
  }
}

void init_mqtt()
{
  client.setServer("192.168.2.12", 1883);
  client.setCallback(callback);
}
void connect_mqtt()
{
  client.setBufferSize(5000);
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "simon", "152253", "Rollos/availability", 1, true, "offline"))
    {
      Serial.println("connected");
      send_mqtt("Rollos/availability", "online", true);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 0.5 seconds");
      // ESP.deepSleep(3.5 * 3600 * 1000000);
      delay(500);
    }
  }
}

void send_mqtt(String topic, String message, boolean rem)
{
  connect_mqtt();
  Serial.print("Size of message: ");
  Serial.println(message.length());

  client.publish(topic.c_str(), message.c_str(), rem);
}

void subscribe_mqtt(String topic)
{
  connect_mqtt();
  client.subscribe(topic.c_str());
}

void disconnect_mqtt()
{

  send_mqtt("Rollos/availability", "offline", true);
  delay(1000);
  client.disconnect();
  delay(1000);
}

bool new_mqtt_msg()
{
  boolean tmpVal = received_new_mqtt_msg;
  received_new_mqtt_msg = false;
  return tmpVal;
};

String get_topic()
{
  return received_mqtt_topic;
};

String get_message()
{
  return received_mqtt_msg;
};
