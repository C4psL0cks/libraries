/*
ตัวอย่างโคด โดย www.arduinoall.com
โคดสำหรับหาค่า calibration_factor เพื่อใช้กำหนดให้กับ load cell

ปรับค่าเริ่มต้นจากโคดบรรทัดนี้ float calibration_factor = -380000;
ค่าเริ่มต้นนี้เป็นค่าของ 5Kg แบบหยาบ

เปิด Serial Monitor แล้ว กด + หรือ - เพื่อปรับหาค่าแบบละเอียด

VCC - 5V
GND - GND
DOUT - 3
CLK - 2

*/

#include "HX711.h"
#define DOUT  3
#define CLK  2
HX711 scale(DOUT, CLK);
float calibration_factor = -380000; //  ปรับค่าเริ่มต้นตรงนี้
void setup() {
  Serial.begin(9600);
  Serial.println("Press + or - to calibration factor");
  Serial.println("ArduinoAll Calibration 0 Please Wait ... ");
  scale.set_scale();
  scale.tare(); //รีเซตน้ำหนักเป็น 0 
  long zero_factor = scale.read_average(); //อ่านค่าน้ำหนักเริ่มต้น
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}
void loop() {
  scale.set_scale(calibration_factor); //ปรับค่า calibration factor
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 2); // แสดงผลทศนิยม 2 หลัก
  Serial.print(" kg");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+')
      calibration_factor += 10;
    else if(temp == '-')
      calibration_factor -= 10;
  }
}
