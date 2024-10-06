#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <string.h>
#include "connect_mqtt.h"

#define rx D3
#define CSS_MAINCOLOR "blue"

#define doallms 50
/*
All down 11111111111111110000010010011011010010010011010010010011011010011011011010011011011011011011010010011011010010010010010010010011010010010011
ALL STOP 11111111111111110000010010011011010010010011010010010011011010011011011010011011011011011011010010011011010010010010010011010011010011010011
ALL up   11111111111111110000010010011011010010010011010010010011011010011011011010011011011011011011010010011011010010010010010010011011011011010010

Wohnzimmer:                                                                                                              x
down     11111111111111110000010010011011010010010011010010010011011010011011011010011011011011011011010010011011010010011010010010011011010010011011
stop     11111111111111110000010010011011010010010011010010010011011010011011011010011011011011011011010010011011010010011010010011010011010011010011
up       11111111111111110000010010011011010010010011010010010011011010011011011010011011011011011011010010011011010010011010010010010011010010010011

11111111111111110000010010011011010010010011010010010011011010011011011010011011011011011011010010011011010010010010010011010011010011010011
1111111111111111000001001001101101001001001101001001001101101001101101101001101101101101101101001001101101001001001001001001101101101101001
1111111111111111000001001001101101001001001101001001001101101001101101101001101101101101101101001001101101001001001001001001001101101101101
*/

const int timeofbit = 310;
bool ALL_UP[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1};
// bool ALL_STOP[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1};
bool ALL_STOP[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1};
bool ALL_DOWN[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0};

bool bool_state_retain = true;

static int timeout = 60;
const char *PARAM_MESSAGE = "message";

String ProjectHostname = "RollerShutterRemote";
WiFiManager wifiManager;
ESP8266WebServer server(80);

// char mqtt_sub_topic[] = (ProjectHostname + "-" + ESP.getChipId(), HEX).c_str();

void send433(bool send[141])
{
  for (int n = 0; n < 10; n++)
  {
    unsigned long save_time = micros();
    digitalWrite(LED_BUILTIN, LOW);
    for (int i = 0; i < 140; i++)
    {
      digitalWrite(rx, send[i]);

      while ((save_time + 300) > micros())
      {
        // wait
      }
      save_time = micros();
    }
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(rx, LOW);
    delay(10);
  }
}

void handleCss()
{
  // output of stylesheet
  // this is a straight forward example how to generat a static page from program memory
  String message;
  message = F("*{font-family:sans-serif}\n"
              "body{margin:10px}\n"
              "h1, h2{color:white;background:#dddddd;text-align:center}\n"
              "h1{font-size:1.2em;margin:1px;padding:5px}\n"
              "h2{font-size:1.0em}\n"
              "h3{font-size:0.9em}\n"
              "a{text-decoration:none;color:dimgray;text-align:center}\n"
              "main{text-align:center}\n"
              "article{vertical-align:top;display:inline-block;margin:0.2em;padding:0.1em;border-style:solid;border-color:#C0C0C0;background-color:#E5E5E5;width:20em;text-align:left}\n" // if you don't like the floating effect (vor portrait mode on smartphones!) - remove display:inline-block
              "article h2{margin-top:0;padding-bottom:1px}\n"
              "section {margin-bottom:0.2em;clear:both;}\n"
              "table{border-collapse:separate;border-spacing:0 0.2em}\n"
              "th, td{background-color:#C0C0C0}\n"
              "button{margin-top:0.3em}\n"
              "footer p{font-size:0.7em;color:dimgray;background:silver;text-align:center;margin-bottom:5px}\n"
              "nav{background-color:silver;margin:1px;padding:5px;font-size:0.8em}\n"
              "nav a{color:dimgrey;padding:10px;text-decoration:none}\n"
              "nav a:hover{text-decoration:underline}\n"
              "nav p{margin:0px;padding:0px}\n"
              ".on, .off{margin-top:0;margin-bottom:0.2em;margin-left:4em;font-size:1.4em;background-color:#C0C0C0;border-style:solid;border-radius:10px;border-style:outset;width:5em;height:1.5em;text-decoration:none;text-align:center}\n"
              ".on{border-color:green}\n"
              ".off{border-color:red}\n"

              ".toggle {\n"
              "position:relative;\n"
              "display:inline-block;\n"
              "width:40px;\n"
              "height:60px;\n"
              "background-color:#bbb;\n"
              "-webkit-border-radius:4px;\n"
              "-moz-border-radius:4px;\n"
              "border-radius:4px;\n"
              "text-align:center;\n"
              "}\n"

              ".toggle input {\n"
              "width:100%;\n"
              "height:100%;\n"
              "margin:0 0;\n"
              "padding:0 0;\n"
              "position:absolute;\n"
              "top:0;\n"
              "right:0;\n"
              "bottom:0;\n"
              "left:0;\n"
              "z-index:2;\n"
              "cursor:pointer;\n"
              "opacity:0;\n"
              "filter:alpha(opacity=0);\n"
              "}\n"

              ".toggle label {\n"
              "display:block;\n"
              "position:absolute;\n"
              "top:1px;\n"
              "right:1px;\n"
              "bottom:1px;\n"
              "left:1px;\n"
              "background-image:-webkit-linear-gradient(top,#fff 0%,#ddd 50%,#fff 50%,#eee 100%);\n"
              "background-image:-moz-linear-gradient(top,#fff 0%,#ddd 50%,#fff 50%,#eee 100%);\n"
              "background-image:-ms-linear-gradient(top,#fff 0%,#ddd 50%,#fff 50%,#eee 100%);\n"
              "background-image:-o-linear-gradient(top,#fff 0%,#ddd 50%,#fff 50%,#eee 100%);\n"
              "background-image:linear-gradient(top,#fff 0%,#ddd 50%,#fff 50%,#eee 100%);\n"
              "-webkit-box-shadow:0 2px 3px rgba(0,0,0,0.4),\n"
              "inset 0 -1px 1px #888,\n"
              "inset 0 -5px 1px #bbb,\n"
              "inset 0 -6px 0 white;\n"
              "-moz-box-shadow:0 2px 3px rgba(0,0,0,0.4),\n"
              "inset 0 -1px 1px #888,\n"
              "inset 0 -5px 1px #bbb,\n"
              "inset 0 -6px 0 white;\n"
              "box-shadow:0 2px 3px rgba(0,0,0,0.4),\n"
              "inset 0 -1px 1px #888,\n"
              "inset 0 -5px 1px #bbb,\n"
              "inset 0 -6px 0 white;\n"
              "-webkit-border-radius:3px;\n"
              "-moz-border-radius:3px;\n"
              "border-radius:3px;\n"
              "font:normal 11px Arial,Sans-Serif;\n"
              "color:#666;\n"
              "text-shadow:0 1px 0 white;\n"
              "cursor:text;\n"
              "}\n"

              ".toggle label:before {\n"
              "content:attr(data-off);\n"
              "position:absolute;\n"
              "top:6px;\n"
              "right:0;\n"
              "left:0;\n"
              "z-index:4;\n"
              "}\n"

              ".toggle label:after {\n"
              "content:attr(data-on);\n"
              "position:absolute;\n"
              "right:0;\n"
              "bottom:11px;\n"
              "left:0;\n"
              "color:#666;\n"
              "text-shadow:0 -1px 0 #eee;\n"
              "}\n"

              ".toggle input:checked + label {\n"
              "background-image:-webkit-linear-gradient(top,#eee 0%,#ccc 50%,#fff 50%,#eee 100%);\n"
              "background-image:-moz-linear-gradient(top,#eee 0%,#ccc 50%,#fff 50%,#eee 100%);\n"
              "background-image:-ms-linear-gradient(top,#eee 0%,#ccc 50%,#fff 50%,#eee 100%);\n"
              "background-image:-o-linear-gradient(top,#eee 0%,#ccc 50%,#fff 50%,#eee 100%);\n"
              "background-image:linear-gradient(top,#eee 0%,#ccc 50%,#fff 50%,#eee 100%);\n"
              "-webkit-box-shadow:0 0 1px rgba(0,0,0,0.4),\n"
              "inset 0 1px 7px -1px #ccc,\n"
              "inset 0 5px 1px #fafafa,\n"
              "inset 0 6px 0 white;\n"
              "-moz-box-shadow:0 0 1px rgba(0,0,0,0.4),\n"
              "inset 0 1px 7px -1px #ccc,\n"
              "inset 0 5px 1px #fafafa,\n"
              "inset 0 6px 0 white;\n"
              "box-shadow:0 0 1px rgba(0,0,0,0.4),\n"
              "inset 0 1px 7px -1px #ccc,\n"
              "inset 0 5px 1px #fafafa,\n"
              "inset 0 6px 0 white;\n"
              "}\n"

              ".toggle input:checked:hover + label {\n"
              "-webkit-box-shadow:0 1px 3px rgba(0,0,0,0.4),\n"
              "inset 0 1px 7px -1px #ccc,\n"
              "inset 0 5px 1px #fafafa,\n"
              "inset 0 6px 0 white;\n"
              "-moz-box-shadow:0 1px 3px rgba(0,0,0,0.4),\n"
              "inset 0 1px 7px -1px #ccc,\n"
              "inset 0 5px 1px #fafafa,\n"
              "inset 0 6px 0 white;\n"
              "box-shadow:0 1px 3px rgba(0,0,0,0.4),\n"
              "inset 0 1px 7px -1px #ccc,\n"
              "inset 0 5px 1px #fafafa,\n"
              "inset 0 6px 0 white;\n"
              "}\n"

              ".toggle input:checked + label:before {\n"
              "z-index:1;\n"
              "top:11px;\n"
              "}\n"

              ".toggle input:checked + label:after {\n"
              "bottom:9px;\n"
              "color:#aaa;\n"
              "text-shadow:none;\n"
              "z-index:4;\n"
              "}\n"
              ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: auto;cursor: pointer;border-radius: 4px;}\n"
              ".button-on {background-color: #1abc9c;}\n"
              ".button-on:active {background-color: #16a085;}\n"
              ".button-off {background-color: #34495e;}\n"
              ".button-off:active {background-color: #2c3e50;}\n");

  server.send(200, "text/css", message);
}

void UP()
{
  server.send(204);
  Serial.println("ALL UP");
  send_mqtt("Rollos/log/ALL", "open", bool_state_retain);
  send433(ALL_UP);
}

void STOP()
{
  server.send(204);
  Serial.println("ALL STOP");
  send_mqtt("Rollos/log/ALL", "stopped", bool_state_retain);
  send433(ALL_STOP);
}

void DOWN()
{
  server.send(204);
  Serial.println("DOWN");
  send_mqtt("Rollos/log/ALL", "closed", bool_state_retain);
  send433(ALL_DOWN);
}

void RESET()
{
  server.send(200, "text/html", "Restart_Ok");
  Serial.println("restart");
  send_mqtt("Rollos/log/system", "RESTART", false);
  delay(500);
  disconnect_mqtt();
  delay(500);
  ESP.restart();
}

void RESETWIFISETTINGS()
{
  server.send(200, "text/html", "Restart_Ok");
  Serial.println("restart");
  send_mqtt("Rollos/log/system", "RESTART", false);
  delay(500);
  disconnect_mqtt();
  delay(500);
  wifiManager.resetSettings();
  ESP.eraseConfig();
  ESP.restart();
  delay(3000);
}

void rootPage()
// a very simple example how to output a HTML page from program memory
{
  String message;
  message = F("<!DOCTYPE html>\n"
              "<html lang='en'>\n"
              "<head>\n"
              "<title> shade control </title>\n"
              "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"
              "<meta name=\"viewport\" content=\"width=device-width\">\n"
              "<link rel='stylesheet' type='text/css' href='/css'>\n"
              "</head>\n"
              "<body>\n"
              "<h1>shade control</h1>\n"
              /*
              "<p>This is an example how you can create a webpage.</p>\n"
              "<p>But as most of my webserver should have the same look and feel I'm using one layout for all html pages.</p>\n"
              "<p>Therefore my html pages come from the functions starting with handlePage and just share a common top and bottom.</p>\n"
              "<p>To go back to the formated pages <a href='0.htm'>use this link</a></p>\n"
              */
              "<p></p>\n"
              "<p></p>\n"
              "<p></p>\n"
              "<a class=\"button button-up\" href=\"/UP\">&#9650;</a>\n"
              "<p></p>\n"
              "<a class=\"button button-stop\" href=\"/STOP\">&#9689</a>\n"
              "<p></p>\n"
              "<a class=\"button button-down\" href=\"/DOWN\">&#9660;</a>\n"
              "<p></p>\n"
              "<a class=\"button restart\" href=\"/restart\">reboot</a>\n"
              "</body>\n"
              "</html>");
  /*
  "<span class=\"toggle\">"
    "<input type=\"checkbox\">"
    "<label data-off=\"&#8593;\" data-on=\"&#8595;\"></label>"
   "</span>"
   */

  server.send(200, "text/html", message);
}

void StartWebserver()
{
  server.onNotFound([]()
                    { server.send(404, "text/plain", "Link wurde nicht gefunden!"); });

  server.on("/", rootPage);
  server.on("/UP", UP);
  server.on("/STOP", STOP);
  server.on("/DOWN", DOWN);
  server.on("/reset", RESET);
  server.on("/restart", RESET);
  server.on("/reboot", RESET);
  server.on("/RESETWIFISETTINGS", RESETWIFISETTINGS);
  server.on("/css", handleCss);
  server.begin();
  Serial.println("Webserver gestartet.");
}

void wifiConnect()
{
  if (!(WiFi.status() == WL_CONNECTED))
  {
    server.close();
    server.stop();
    wifiManager.setConnectTimeout(60);
    wifiManager.setConfigPortalTimeout(180);
    bool res;
    res = wifiManager.autoConnect(ProjectHostname.c_str()); //

    if (!res)
    {
      Serial.println("Failed to connect");
      delay(3000);
      // reset and try again, or maybe put it to deep sleep
      ESP.restart();

      delay(5000);
    }
    else
    {
      if (MDNS.begin(ProjectHostname + "-" + String(ESP.getChipId(), HEX)))
      {
        Serial.println("DNS gestartet, erreichbar unter: ");
        Serial.println("http://" + String(ProjectHostname + "-" + String(ESP.getChipId(), HEX)) + ".lan/");
      }
      init_mqtt();
      subscribe_mqtt("Rollos/#");
      StartWebserver();
    }
  }
}



void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("");

  Serial.println("STARTED!!!");
  pinMode(rx, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(rx, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("pins set ready!!!");
  const String prj_name = ProjectHostname + "-" + String(ESP.getChipId(), HEX);
  WiFi.hostname(prj_name.c_str());
  ArduinoOTA.setHostname(prj_name.c_str());

  ArduinoOTA.onStart([]()
                     {
                      disconnect_mqtt();
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_FS
      type = "filesystem";
    } });

  wifiManager.setTimeout(timeout);
  wifiManager.setConfigPortalTimeout(timeout);
  wifiManager.setConnectTimeout(timeout);

  // Print Infos
  Serial.print(ProjectHostname);
  Serial.print(" on ESP ID: ");
  Serial.println(String(ESP.getChipId(), HEX));

  wifiConnect();
  StartWebserver();
  send_mqtt("Rollos/log/system", "Started", false);
  ArduinoOTA.begin();

  delay(500);
}

void loop()
{
  wifiConnect();
  ArduinoOTA.handle();
  MDNS.update();
  loop_mqtt();
  server.handleClient();

  if (new_mqtt_msg())
  {
    if (get_topic().equals("Rollos/cmnd"))
    {
      String msg = get_message();
      if (msg.equals("UP"))
      {
        Serial.println("UP from mqtt");
        UP();
      }
      else if (msg.equals("STOP"))
      {
        Serial.println("STOP from mqtt");
        STOP();
      }
      else if (msg.equals("DOWN"))
      {
        Serial.println("DOWN from mqtt");
        DOWN();
      }
    }
  }
  delay(50);
}