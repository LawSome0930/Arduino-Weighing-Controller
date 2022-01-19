#include <HX711.h>
#include <Joystick.h>

HX711 scale0; // 4个HX711模块
HX711 scale1;
HX711 scale2;
HX711 scale3;

float w0, w1, w2, w3 = 0; // 称重传感实时值
float w0_min, w1_min, w2_min, w3_min = 0; // 称重最小值
float w0_max, w1_max, w2_max, w3_max = 0; // 称重最大值
float w0_rang, w1_rang, w2_rang, w3_rang = 0; // 测量范围
int init_finish = 0; // 初始化完成标志
int16_t a0, a1, a2, a3 = 0;
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  false, false,        // 0个按键，0个苦力帽
  true, true, true,   // 需要x，y，z轴
  true, false, false,   // 需要Rx轴，不需要Ry, Rz轴
  false, false,          // No rudder or throttle
  false, false, false    // No accelerator, brake, or steering
);


// 初始化程序
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // 开启串口

  scale0.begin(2, 3); // 定义4个HX711模块的针脚，第一个为信号输出针脚
  scale1.begin(4, 5); // ，第二个为时钟信号输入。
  scale2.begin(6, 7);
  scale3.begin(16, 10);
  init_weighing();

  //  Joystick.setXAxisRange(0, 65535); // 设定各轴的输出范围为0到65535
  //  Joystick.setYAxisRange(0, 65535);
  //  Joystick.setZAxisRange(0, 65535);
  //  Joystick.setRxAxisRange(0, 65535);
  //  Joystick.begin(false); // 开始模拟joystick，关闭自动报告
}

// 循环程序
void loop() {

  w0 = scale0.get_units();  // 获取当前称重值
  w1 = scale1.get_units();
  w2 = scale2.get_units();
  w3 = scale3.get_units();

  // 饱和限幅
  if(w0 > w0_max)
  {
    w0 = w0_max;
  }
  if(w0 < w0_min)
  {
    w0 = w0_min;
  }
  if(w1 > w1_max)
  {
    w1 = w1_max;
  }
  if(w1 < w1_min)
  {
    w1 = w1_min;
  }
  if(w2 > w2_max)
  {
    w2 = w2_max;
  }
  if(w2 < w2_min)
  {
    w2 = w2_min;
  }
  if(w3 > w3_max)
  {
    w3 = w3_max;
  }
  if(w3 < w3_min)
  {
    w3 = w3_min;
  }
  
  a0 = (w0 - w0_min)/w0_rang*100;
  a1 = (w1 - w1_min)/w1_rang*100;
  a2 = (w2 - w2_min)/w2_rang*100;
  a3 = (w3 - w3_min)/w3_rang*100;
  
  Serial.print("axis0: ");
  Serial.print(a0);
  Serial.print("\n");

  Serial.print("axis1: ");
  Serial.print(a1);
  Serial.print("\n");

  Serial.print("axis2: ");
  Serial.print(a2);
  Serial.print("\n");

  Serial.print("axis3: ");
  Serial.print(a3);
  Serial.print("\n");
  delay(500);
  // Joystick.sendState();
}

/*******称重初始化，校准称重值范围********/
void init_weighing() {
  delay(5000);
  w0_min = scale0.get_units();  // 获取空载下的称重初始值
  w1_min = scale1.get_units();
  w2_min = scale2.get_units();
  w3_min = scale3.get_units();

  w0_max = w0_min;
  w1_max = w0_min;
  w2_max = w0_min;
  w3_max = w0_min;

  Serial.print("scale0.init: ");
  Serial.print(w0_min);
  Serial.print("\n");

  Serial.print("scale1.init: ");
  Serial.print(w1_min);
  Serial.print("\n");

  Serial.print("scale2.init: ");
  Serial.print(w2_min);
  Serial.print("\n");

  Serial.print("scale3.init: ");
  Serial.print(w3_min);
  Serial.print("\n");

  while (init_finish == 0)
  {
    float w0_tmp = scale0.get_units();
    float w1_tmp = scale1.get_units();
    float w2_tmp = scale2.get_units();
    float w3_tmp = scale3.get_units();

    if (w0_tmp > w0_max)  // 记录称重最大值
    {
      w0_max = w0_tmp;
    }
    if (w1_tmp > w1_max)
    {
      w1_max = w1_tmp;
    }
    if (w2_tmp > w2_max)
    {
      w2_max = w2_tmp;
    }
    if (w3_tmp > w3_max)
    {
      w3_max = w3_tmp;
    }

    char ans = Serial.read();
    if (ans == '1')
    {
      init_finish = 1;
    }
  }

  w0_rang = w0_max - w0_min;
  w1_rang = w1_max - w1_min;
  w2_rang = w2_max - w2_min;
  w3_rang = w3_max - w3_min;

  Serial.print("scale0.range: ");
  Serial.print(w0_rang);
  Serial.print("\n");
  Serial.print("scale1.range: ");
  Serial.print(w1_rang);
  Serial.print("\n");
  Serial.print("scale2.range: ");
  Serial.print(w2_rang);
  Serial.print("\n");
  Serial.print("scale3.range: ");
  Serial.print(w3_rang);
  Serial.print("\n");
  Serial.print("init finish\n");
  delay(5000);
}
