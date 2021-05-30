#include <HardwareSerial.h>
#include "ESP32_EC2X.h"
#include "internet.h"
#include "uc_mqtt.h"

INTERNET net;
UCxMQTT mqtt;

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define RXPIN  27
#define TXPIN  26

#define MQTT_SERVER      "broker.mqttdashboard.com"
#define MQTT_PORT        "8000"
#define MQTT_ID          "3G"
#define MQTT_USER        ""
#define MQTT_PASSWORD    ""


#define SUBSCRIBE "testtopic/1"

unsigned long previousmqtt = 0;
const long intervalmqtt = 2000;

HardwareSerial mySerial(2);

void debug(String data)
{
  Serial.println(data);
}
void setup()
{
  
  Serial.begin(115200);
  gsm.begin(&mySerial, 115200, RXPIN, TXPIN);
  gsm.Event_debug = debug;

  gsm.SetPowerKeyPin();
  Serial.println(F("ESP32-UC20"));
  gsm.PowerOn();
  while (gsm.WaitReady()) {}

  delay(8000);
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());
  Serial.println(F("Disconnect net"));
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS);
  Serial.println(F("Connect net"));
  net.Connect();
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());
  Serial.println(F("Start HTTP"));
  
  mqtt.callback = callback;     //กำหนด Function ที่จะทำงานเมื่อมี Message ที่ Sub ไว้เข้ามา
  connect_server();       //เรียก Function เพื่อเชื่อมต่อ Server
}
/*
  Callback คือ function ที่เรา map เอาไว้เมื่อมีการ publish topic ที่เรา subscribe เอาไว้ จะมีการเรียก Function นี้ขึ้นมา โดยจะมีการส่ง ชื่อ topic , payload และ length ของ payload มาให้
*/
void callback(String topic , char *payload, unsigned char length)
{
  Serial.println();
  Serial.println(F("%%%%%%%%%%%%%%%%%%%%%%%%%%%%"));
  Serial.print(F("Topic --> "));
  Serial.println(topic);    //แสดง Topic ที่ได้รับ
  payload[length] = 0;  // กำหนดให้ byte สุดท้ายของ payload เป็น 0 (ปิด String)
  String str_data(payload); //เก็บ payload ให้อยู่ในตัวแปล String
  Serial.print(F("Payload --> "));
  Serial.println(str_data); // แสดง payload

  if (topic == "testtopic") // ตรวจสอบว่า Topic คือ led หรือไม่
  {
    Serial.println("OKKKKKKKKKKKKKKKKKK"); // แสดง payload
  }
}
/*
  connect_server คือ Function สำหรับเชื่อมต่อกับ MQTT Server และเมื่อเชื่อมต่อสำเร็จแล้ว จะส่งชื่อ Topic ที่ต้องการ Subscribe ไปยัง Server
*/
void connect_server()
{
  do
  {
    Serial.println(F("Connect Server"));
    Serial.println(F("wait connect"));
    if (mqtt.DisconnectMQTTServer()) //สั่งให้ตัดการเชื่อมต่อกับ MQTT Server
    {
      mqtt.ConnectMQTTServer(MQTT_SERVER, MQTT_PORT); // เชื่อมต่อกับ MQTT Server
    }
    delay(500);
    Serial.println(mqtt.ConnectState()); // แสดงสถานะการเชื่อมต่อ
  }
  while (!mqtt.ConnectState()); //ตรวจเช็คและวนรอบเชื่อมต่อกับ MQTT Server จนสำเร็จจึงหลุด Loop
  Serial.println(F("Server Connected"));
  // Login โดยส่ง username , password
  unsigned char ret = mqtt.Connect(MQTT_ID, MQTT_USER, MQTT_PASSWORD);
  Serial.println(mqtt.ConnectReturnCode(ret));  //แสดงสถาณะการ login ที่ Server ตอบกลับมา
  if (ret == 0) // ถ้าหาก login สำเร็จ
  {
    mqtt.Subscribe(SUBSCRIBE);        // ส่ง Topic ที่ต้องการ Subscribe ไปยัง Server
  }
}

void loop()
{
  //ดึงค่าเวลาที่ Arduino ทำงานตั้งแต่เริ่มทำงานจนถึงปัจจุบัน (millisecond)
  unsigned long currentMillis = millis();
  /*เปรียบเทียบว่า ค่าเวลาปัจจุบัน เมื่อลบกับ ค่าเวลาก่อนหน้า มาค่ามากกว่าหรือเท่ากับ ค่าเวลาที่ตั้งเอาไว้หรือไม่
    ผลของเงื่อนไขนี้ก็คือ จะมีการทำงานตามเงื่อนไข(true) ในทุกๆคาบเวลาที่กำหนดไว้ในตัวแปล intervalmqtt (ในที่นี้คือ ทำงานทุกๆ 2 วินาที ซึ่งเป็นค่าที่ dth22 กำหนดเอาไว้ว่าควรอ่าค่าจาก Sensor ห่างกันอย่างน้อย 2 วินาที)
  */
  if (currentMillis - previousmqtt >= intervalmqtt)
  {
    float h = 0;  //อ่านค่า humidity จาก Sensor
    float t = 0; // อ่านค่า temp จาก sensor
    String str = String(t, 2);   // แปลงค่า temp จาก Float ให้อยู่ในรูป String แสดง ทศนิยม 2 ตำแหน่ง
    mqtt.Publish("Temp", str);   // Publish Topic Temp ไปยัง Server
    str = String(h, 2);  // แปลงค่า humidity จาก Float ให้อยู่ในรูป String แสดง ทศนิยม 2 ตำแหน่ง
    mqtt.Publish("Humi", str);   // Publish Topic Humi ไปยัง Server
    previousmqtt = currentMillis; // นำค่าเวลาปัจจุบัน ไปเก็บเป็นค่าเวลาก่อนหน้านี้
  }
  mqtt.MqttLoop();      // Function คอยตรวจจับเมื่อมีข้อมูลส่งมาจาก 3G
  if (!mqtt.ConnectState())   // ตรวจสอบว่ายังสามารถเชื่อมต่อกับ Server ได้อยู่หรือไม่
    connect_server();      //เชื่อมต่อกับ Server
}
