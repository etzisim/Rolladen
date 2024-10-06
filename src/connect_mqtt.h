#ifndef mqtt_h
#define mqtt_h

void init_mqtt();
void connect_mqtt();
void send_mqtt(String topic, String messge, boolean rem);
void subscribe_mqtt(String topic);
void disconnect_mqtt();
void loop_mqtt();
void set_upgrade_sub();
bool new_mqtt_msg();
String get_topic();
String get_message();

#endif