#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define wifi_ssid "Manh"
#define wifi_password "manhvan1012"


#define mqtt_server "192.168.1.167"
#define mqtt_user "mymqtt"      
#define mqtt_password "mymqtt" 
const uint16_t mqtt_port =  1883;


WiFiClient espClient;
PubSubClient client(espClient);

#define sub_topic "remote/command"  

char message[100]; // lay gia tri tu mqtt server

const uint16_t kIrLed = 4; 

IRsend irsend(kIrLed);

void setup() {
  Serial.begin(9600);     
  setup_wifi();           //Connect to Wifi network
  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);
  irsend.begin();
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

if(msgString=="power")
{
     Serial.println("SEND power");
     irsend.sendNEC(0xFF02FD,32);
     delay(50);
}
else
if(msgString=="tattieng")
{
     Serial.println("SEND tattieng");
     irsend.sendNEC(0xFF48B7,32);
     delay(50);
}
else
if(msgString=="audio")
{
     Serial.println("SEND audio");
     irsend.sendNEC(0xFFB847,32);
     delay(50);
}
else
if(msgString=="sub")
{
     Serial.println("SEND sub");
     irsend.sendNEC(0xFF8877,32);
     delay(50);
}
else
if(msgString=="f1")
{
     Serial.println("SEND f1");
     irsend.sendNEC(0xFF728D,32);
     delay(50);
}
else
if(msgString=="f2")
{
     Serial.println("SEND f2");
     irsend.sendNEC(0xFFF20D,32);
     delay(50);
}
else
if(msgString=="f3")
{
     Serial.println("SEND f3");
     irsend.sendNEC(0xFF0AF5,32);
     delay(50);
}
else
if(msgString=="f4")
{
     Serial.println("SEND f4");
     irsend.sendNEC(0xFF8A75,32);
     delay(50);
}
else
if(msgString=="home")
{
     Serial.println("SEND home");
     irsend.sendNEC(0xFFB24D,32);
     delay(50);
}
else
if(msgString=="menu")
{
     Serial.println("SEND menu");
     irsend.sendNEC(0xFF6A95,32);
     delay(50);
}
else
if(msgString=="back")
{
     Serial.println("SEND back");
     irsend.sendNEC(0xFF7887,32);
     delay(50);
}
else
if(msgString=="ok")
{
     Serial.println("SEND ok");
     irsend.sendNEC(0xFF926D,32);
     delay(50);
}
else
if(msgString=="len")
{
     Serial.println("SEND len");
     irsend.sendNEC(0xFFE21D,32);
     delay(50);
}
else
if(msgString=="xuong")
{
     Serial.println("SEND xuong");
     irsend.sendNEC(0xFFD22D,32);
     delay(50);
}
else
if(msgString=="trai")
{
     Serial.println("SEND trai");
     irsend.sendNEC(0xFF12ED,32);
     delay(50);
}
else
if(msgString=="phai")
{
     Serial.println("SEND phai");
     irsend.sendNEC(0xFF52AD,32);
     delay(50);
}
else
if(msgString=="<<")
{
     Serial.println("SEND <<");
     irsend.sendNEC(0xFFC23D,32);
     delay(50);
}
else
if(msgString==">>")
{
     Serial.println("SEND >>");
     irsend.sendNEC(0xFF629D,32);
     delay(50);
}
else
if(msgString==">||")
{
     Serial.println("SEND >||");
     irsend.sendNEC(0xFF22DD,32);
     delay(50);
}
else
if(msgString=="vuong")
{
     Serial.println("SEND vuong");
     irsend.sendNEC(0xFFA25D,32);
     delay(50);
}
else
if(msgString=="vol+")
{
     Serial.println("SEND vol+");
     irsend.sendNEC(0xFF30CF,32);
     delay(50);
}
else
if(msgString=="vol-")
{
     Serial.println("SEND vol-");
     irsend.sendNEC(0xFF18E7,32);
     delay(50);
}
else
if(msgString=="ch+")
{
     Serial.println("SEND ch+");
     irsend.sendNEC(0xFFD02F,32);
     delay(50);
}
else
if(msgString=="ch-")
{
     Serial.println("SEND ch-");
     irsend.sendNEC(0xFFA857,32);
     delay(50);
}
else
if(msgString=="mot")
{
     Serial.println("SEND mot");
     irsend.sendNEC(0xFF807F,32);
     delay(50);
}
else
if(msgString=="hai")
{
     Serial.println("SEND hai");
     irsend.sendNEC(0xFF40BF,32);
     delay(50);
}
else
if(msgString=="ba")
{
     Serial.println("SEND ba");
     irsend.sendNEC(0xFFC03F,32);
     delay(50);
}
else
if(msgString=="bon")
{
     Serial.println("SEND bon");
     irsend.sendNEC(0xFF20DF,32);
     delay(50);
}
else
if(msgString=="nam")
{
     Serial.println("SEND nam");
     irsend.sendNEC(0xFFA05F,32);
     delay(50);
}
else
if(msgString=="sau")
{
     Serial.println("SEND sau");
     irsend.sendNEC(0xFF609F,32);
     delay(50);
}
else
if(msgString=="bay")
{
     Serial.println("SEND bay");
     irsend.sendNEC(0xFFE01F,32);
     delay(50);
}
else
if(msgString=="tam")
{
     Serial.println("SEND tam");
     irsend.sendNEC(0xFF10EF,32);
     delay(50);
}
else
if(msgString=="chin")
{
     Serial.println("SEND chin");
     irsend.sendNEC(0xFF906F,32);
     delay(50);
}
else
if(msgString=="khong")
{
     Serial.println("SEND khong");
     irsend.sendNEC(0xFF00FF,32);
     delay(50);
}
else
if(msgString=="del")
{
     Serial.println("SEND del");
     irsend.sendNEC(0xFF50AF,32);
     delay(50);
}
else
if(msgString=="caps/num")
{
     Serial.println("SEND caps/num");
     irsend.sendNEC(0xFF6897,32);
     delay(50);
}


}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266RemoteDo",mqtt_user, mqtt_password)) {
      Serial.println("connected");
     // Can sub topic nao thi viet tiep duoi day:
      client.subscribe(sub_topic); 
    } else {
      Serial.print("failed, error = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
