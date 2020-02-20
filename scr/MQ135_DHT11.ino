#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPtimeESP.h>
#include "DHT.h"
#include "MQ135.h"

NTPtime NTPch("ch.pool.ntp.org");

#define wifi_ssid "Manh"
#define wifi_password "manhvan1012"

#define mqtt_server "192.168.1.167"
#define mqtt_user "mymqtt"      
#define mqtt_password "mymqtt" 
const uint16_t mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

#define temperature_topic "sensor/temperature"  
#define humidity_topic "sensor/humidity"
#define air_quality_topic "sensor/airquality"
#define time_send_topic "sensor/timesend"
#define delay_topic "setting/mq135dht11"
#define delay_topic_return "setting/mq135dht11/return"

int delaytime=5;
unsigned long last = 0;
char message[100]; // lay gia tri tu mqtt server
int check =1;

#define DHTTYPE DHT11
#define DHTPIN D1
DHT dht(DHTPIN, DHTTYPE);

#define PIN_MQ135 A0    //Khai báo pin nối với chân AO
MQ135 mq135_sensor = MQ135(PIN_MQ135);   //Khai báo đối tượng thư viện

strDateTime dateTime;

void setup() {
  Serial.begin(9600);     
  setup_wifi();           //Connect to Wifi network
  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);             
  dht.begin();
}

void loop() {
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(check == 1){
  delaytime = delaytime*1000;
  check=0;
  }
 // Serial.print(delaytime);
  if((unsigned long)(millis()-last) > delaytime)
  {
  dateTime = NTPch.getNTPtime(7.0, 0);
   byte Hour = dateTime.hour;      // Gio
   byte Minute = dateTime.minute;  // Phut
   byte Second = dateTime.second;  // Giay
   int Year = dateTime.year;       // Nam
   byte Month = dateTime.month;    // Thang
   byte Day =dateTime.day;         // Ngay

  String lolol = String(Hour)+"h/"+String(Minute)+"p/"+String(Second)+"s "+String(Day)+"/"+String(Month)+"/"+String(Year);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.print(" | Humidity : ");
  Serial.println(humidity);
 float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
  Serial.print("PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");
  float resistance = mq135_sensor.getResistance();
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  client.publish(temperature_topic, String(temperature).c_str(), true);   //false: ko cho tin nhan giu lai tren mqtt server
  client.publish(humidity_topic, String(humidity).c_str(), true);    
  client.publish(air_quality_topic, String(correctedPPM).c_str(), true);
  client.publish(time_send_topic,lolol.c_str() , true);   
  last = millis();
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
  delaytime = msgString.toInt();
  client.publish(delay_topic_return,msgString.c_str(), true);
  check=1;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266Client",mqtt_user, mqtt_password)) {
      Serial.println("connected");
     // Can sub topic nao thi viet tiep duoi day:
     client.publish(delay_topic_return,String(delaytime).c_str(), true);
     client.subscribe(delay_topic);
    } else {
      Serial.print("failed, error = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
