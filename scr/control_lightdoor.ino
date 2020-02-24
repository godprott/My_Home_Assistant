#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Manh"
#define wifi_password "manhvan1012"

#define mqtt_server "192.168.1.167"
#define mqtt_user "mymqtt"      
#define mqtt_password "mymqtt" 
const uint16_t mqtt_port =  1883;

WiFiClient espClient;
PubSubClient client(espClient);

#define pub_topic "control/lightdoor/return"  
#define sub_topic "control/lightdoor"
#define RELAYPIN D7


long last = 0;
char message[100]; // lay gia tri tu mqtt server


void setup() {
  Serial.begin(9600);     
  setup_wifi();           //Connect to Wifi network
  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);
  pinMode(RELAYPIN,OUTPUT);
}

void loop() {
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid,wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  int i=0;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (i = 0; i < length; i++) {
    message[i] = payload[i];
  }
   message[i] = '\0';
   String msgString = String(message);
   Serial.println("Message: " + msgString);
  Serial.println();
  if(msgString == "0")
  {
      digitalWrite(RELAYPIN,HIGH); //tat
      client.publish(pub_topic,"0", true);
  }
  else
  {
      digitalWrite(RELAYPIN,LOW); //bat
      client.publish(pub_topic,"1", true);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266DoorLight",mqtt_user, mqtt_password)) {
      Serial.println("connected");
     // Can sub topic nao thi viet tiep duoi day:
	 digitalWrite(RELAYPIN,HIGH); //tat
     client.publish(pub_topic,"0", true);
     client.subscribe(sub_topic);   
    } else {
      Serial.print("failed, error = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
