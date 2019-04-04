#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "ssid";
const char* password = "password";
const char* mqtt_server = "192.168.31.200";
const int port = 1883;

const char* topic_name = "chat";
const char* client_id = "esp8266_000001";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, port);
  client.setCallback(callback);
}

void loop() {
  String msg = "";
  reconnect();
  client.loop();

  while(Serial.available() > 0){
    msg += char(Serial.read());
    delay(2);
  }
  
  int msglen = msg.length();
  if (msglen > 0) {
    Serial.println(msg);
    char msgArr[64];
    msg.toCharArray(msgArr, 64);
    client.publish(topic_name, msgArr);
  }
}

void setup_wifi(){
  delay(10);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
  Serial.println("");
}

void callback(char* topic, byte* payload, unsigned int lenght){
  String callMsg = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  for(u16_t i = 0; i < lenght; i++){
    callMsg += char(payload[i]);
  }
  Serial.println(callMsg);
}

void reconnect(){
  while(!client.connected()){
    if(client.connect(client_id)){
      client.subscribe(topic_name);
      Serial.println("connect success");
    }
    else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}
