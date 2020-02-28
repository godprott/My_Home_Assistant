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

#define mode_topic "control/doorbell/mode"
#define mode_topic_return "control/doorbell/mode/return"
#define control_topic "control/doorbell"
#define control_topic_return "control/doorbell/return"
#define time_run_topic "control/doorbell/time"
#define time_run_topic_return "control/doorbell/time"
#define RELAYPIN D7

unsigned long last = 0;
char message[100]; // lay gia tri tu mqtt server

int mode = 3;
int timerun = 10;
int check = 1;
int check2 = 0;
int state = 0;
int congtac = 0;

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

 if(check == 1){
  timerun = timerun*1000;
  check=0;
  }

 if(check2 == 1){
  last = millis();
  check2 = 0;
  }

if(state == 1)
{
  if(mode == 1) // luon tat
  {
     if(congtac != 0)
     {
        digitalWrite(RELAYPIN,HIGH); //tat
        congtac = 0;
        client.publish(control_topic_return,String("OFF").c_str(), true);
     }
  }
  if(mode == 3) // luon bat
  {
     if(congtac != 1)
     {
        digitalWrite(RELAYPIN,LOW); //bat
        congtac = 1;
        client.publish(control_topic_return,String("ON").c_str(), true);
     }
  }
  if(mode == 2)  //bat trong 1 khoang thoi gian nhat dinh
  {
     if((unsigned long)(millis()-last) < timerun)
     {
        if(congtac != 1)
        {
            digitalWrite(RELAYPIN,LOW); //bat
            congtac = 1;
            client.publish(control_topic_return,String("ON").c_str(), true);
        }
     }
     else
     {
         if(congtac != 0)
        {
           digitalWrite(RELAYPIN,HIGH); //tat
           congtac = 0;
           client.publish(control_topic_return,String("OFF").c_str(), true);
        }
     }
  }
}
if(state == 0)
{
  if(congtac != 0)
     {
        digitalWrite(RELAYPIN,HIGH); //tat
        congtac = 0;
        client.publish(control_topic_return,String("OFF").c_str(), true);
     }
}

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
  if( topic == mode_topic)
  {
    mode = msgString.toInt();  // 1 2 3
    client.publish(mode_topic_return,msgString.c_str(), true);
  }
  if(topic == time_run_topic)
  {
    timerun = msgString.toInt();  // s
    client.publish(time_run_topic_return,msgString.c_str(), true);
    check=1;
  }
   if(topic == control_topic)
  {
    state = msgString.toInt();  //1 bat, 0 tat
    if(state == 1)
    {
      check2 = 1;  
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266DoorBell",mqtt_user, mqtt_password)) {
      Serial.println("connected");
     // Can sub topic nao thi viet tiep duoi day:
        client.publish(mode_topic_return,String(mode).c_str(), true);
        client.publish(time_run_topic_return,String(timerun).c_str(), true);
        client.publish(control_topic_return,String("OFF").c_str(), true);
        digitalWrite(RELAYPIN,HIGH); //tat
        client.subscribe(mode_topic);   
        client.subscribe(control_topic);   
        client.subscribe(time_run_topic);   
    } else {
      Serial.print("failed, error = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
