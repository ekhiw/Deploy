#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "NTPClient.h"

//needed for wifi manager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"         //https://github.com/tzapu/WiFiManager

#include "ESP8266HTTPClient.h"

#include <ArduinoJson.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", 3600*7, 60000);

unsigned long t0,tnow;
unsigned long interval;


void initWifiManager() {
  WiFiManager wifiManager;
  //  wifiManager.resetSettings();
  wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 11, 1), IPAddress(192, 168,11, 1), IPAddress(255, 255, 255, 0));
  wifiManager.autoConnect("Ekhiw Device");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  initWifiManager();


  timeClient.begin();
  timeClient.update();
  t0 = timeClient.getEpochTime();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(cekInterval(5)){ //run every n second
    Serial.println("trigger "+pingServer());
    
  }
}

String pingServer(){
  String staging = "https://id-staging.sehati.co/api/ping";
  WiFiClient client;
  HTTPClient https;
  https.begin(client, "id-staging.sehati.co",443,"/api/ping",true);
  Serial.print("HTTPS REQUEST");
  int httpCode= https.GET();
  if(httpCode == 200){
    Serial.print("Success ");
    Serial.println(httpCode);
    Serial.println(https.getString());
    return https.getString();
  }else{
    Serial.print("fail ");
    Serial.println(httpCode);
    Serial.println(https.getString());
    return "fail";
  }
}

bool cekInterval(int interval){
  timeClient.update();
  tnow = timeClient.getEpochTime();
  unsigned long tdif = tnow-t0;
  if(tdif>=interval){
    t0=tnow;
    return true;
  }else{
    return false;
  }
}
