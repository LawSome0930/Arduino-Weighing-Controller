#include <HX711.h>
#include <Joystick.h>

HX711 scale0; // 4个HX711模块
HX711 scale1;
HX711 scale2;
HX711 scale3;

float w[]={0, 0, 0 ,0}; // 实时称重值
float w_min[]={0, 0, 0 ,0}; // 空载称重初始值
float w_max[]={0, 0, 0 ,0}; // 称重最大值
float w_range[]={0, 0, 0 ,0}; // 测量范围
int init_finish = 0; // 初始化完成标志
int16_t axis[] = {0, 0 , 0, 0}; // joystick模拟轴各轴的值

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
  init_weighing();  // 初始化校准称重值

  //  Joystick.setXAxisRange(-32768, 32767); // 设定各轴的输出范围为-32768到32767
  //  Joystick.setYAxisRange(-32768, 32767);
  //  Joystick.setZAxisRange(-32768, 32767);
  //  Joystick.setRxAxisRange(-32768, 32767);
  //  Joystick.begin(false); // 开始模拟joystick，关闭自动报告
  Serial.print("Joystick begin"); // 开始模拟joystick，关闭自动报告
}

// 循环程序
void loop() {

  w[0] = scale0.get_units();  // 获取当前称重值
  w[1] = scale1.get_units();
  w[2] = scale2.get_units();
  w[3] = scale3.get_units();

  // 饱和限幅
  for(int i=0;i<4;i++)
  {
    if(w[i] > w_max[i])
    {
      w[i] = w_max[i];
    }
    if(w[i] < w_min[i])
    {
      w[i] = w_min[i];
    }
  }

  // 计算各轴模拟值
  for(int i=0;i<4;i++)
  {
    axis[i] = (w[i] - w_min[i])/w_range[i]*65535-32768;
  }
  
  
  Serial.print("axis0: ");
  Serial.print(axis[0]);
  Serial.print("\n");

  Serial.print("axis1: ");
  Serial.print(axis[1]);
  Serial.print("\n");

  Serial.print("axis2: ");
  Serial.print(axis[2]);
  Serial.print("\n");

  Serial.print("axis3: ");
  Serial.print(axis[3]);
  Serial.print("\n");
  delay(500);
  // Joystick.sendState();
}

/*******称重初始化，校准称重值范围********/
void init_weighing() {
  delay(5000);
  float w_tmp[]={0, 0, 0 ,0};
  w_min[0] = scale0.get_units();  // 获取空载下的称重初始值
  w_min[1] = scale1.get_units();
  w_min[2] = scale2.get_units();
  w_min[3] = scale3.get_units();

  for(int i=0;i<4;i++)
  { 
    w_max[i] = w_min[i];  // 给最大值赋予初值
  }

  Serial.print("scale0.init: ");
  Serial.print(w_min[0]);
  Serial.print("\n");

  Serial.print("scale1.init: ");
  Serial.print(w_min[1]);
  Serial.print("\n");

  Serial.print("scale2.init: ");
  Serial.print(w_min[2]);
  Serial.print("\n");

  Serial.print("scale3.init: ");
  Serial.print(w_min[3]);
  Serial.print("\n");

  Serial.print("请将踏板踩至最大行程, 输入数字1确认\n");

  // 等待用户获得最大行程
  while (init_finish == 0)
  {

    w_tmp[0] = scale0.get_units();
    w_tmp[1] = scale1.get_units();
    w_tmp[2] = scale2.get_units();
    w_tmp[3] = scale3.get_units();

    for(int i=0;i<4;i++)  // 记录称重最大值
    {
      if(w_tmp[i] > w_max[i])
      {
        w_max[i] = w_tmp[i];
      }
    }

    // 用户踩完后，输入数字1确认
    char ans = Serial.read();
    if (ans == '1')
    {
      init_finish = 1;
    }
  }

  for(int i=0;i<4;i++)
  {
    w_range[i] = w_max[i] - w_min[i];
  }

  Serial.print("scale0.range: ");
  Serial.print(w_range[0]);
  Serial.print("\n");
  Serial.print("scale1.range: ");
  Serial.print(w_range[1]);
  Serial.print("\n");
  Serial.print("scale2.range: ");
  Serial.print(w_range[2]);
  Serial.print("\n");
  Serial.print("scale3.range: ");
  Serial.print(w_range[3]);
  Serial.print("\n");
  Serial.print("init finish\n");
  delay(5000);
}
