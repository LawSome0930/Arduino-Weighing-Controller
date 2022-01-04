#include <HX711.h>
#include <Joystick.h>

HX711 scale0; // 4个HX711模块
HX711 scale1;
HX711 scale2;
HX711 scale3;

float w0,w1,w2,w3 = 0;

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  false, false,                 // 0个按键，0个苦力帽
  true, true, true,   // 需要x，y，z轴
  true, false, false,   // 需要Rx轴，不需要Ry, Rz轴
  false, false,          // No rudder or throttle
  false, false, false    // No accelerator, brake, or steering
);


// 初始化程序
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // 开启串口
    
  scale0.begin(2,3);  // 定义4个HX711模块的针脚，第一个为信号输出针脚
  scale1.begin(4,5);  // ，第二个为时钟信号输入。
  scale2.begin(6,7);
  scale3.begin(16,10);
  
//  Joystick.setXAxisRange(0, 65535); // 设定各轴的输出范围为0到65535
//  Joystick.setYAxisRange(0, 65535);
//  Joystick.setZAxisRange(0, 65535);
//  Joystick.setRxAxisRange(0, 65535);
//  Joystick.begin(false); // 开始模拟joystick，关闭自动报告
}

// 循环程序
void loop() {
  // put your main code here, to run repeatedly:
  w0 = scale0.get_units();
  Serial.print("scale0.UNITS: ");
  Serial.print(w0);

  // Joystick.sendState();
}
