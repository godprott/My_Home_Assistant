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

#define pub_topic "sensor/sensordoor"  


unsigned long last = 0;
unsigned long last2 = 0;
char message[100]; // lay gia tri tu mqtt server

const int sensor = D1;
int state; 
int before_state = 3;
int delaytime = 900000; //15p


void setup() {
  Serial.begin(9600);     
  setup_wifi();           //Connect to Wifi network
  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);
  pinMode(sensor, INPUT_PULLUP);
}

void loop() {
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

 if((unsigned long)(millis()-last2) > 200)
 {
    state = digitalRead(sensor);

    if((state != before_state) || ((unsigned long)(millis()-last) > delaytime) )
    {
        if (state == HIGH){
        client.publish(pub_topic,"Mở cửa", true);   
        Serial.println("mo cua");
    }
    else
    {
        client.publish(pub_topic,"Đóng cửa", true);   
        Serial.println("dong cua");
    }
    before_state = state;
    last = millis();
  }
  last2 = millis();
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
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266DoorSensor",mqtt_user, mqtt_password)) {
      Serial.println("connected");
     // Can sub topic nao thi viet tiep duoi day:

    } else {
      Serial.print("failed, error = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
