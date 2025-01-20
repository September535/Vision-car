#include <WiFi.h>
#include "esp_camera.h"
#include <WebServer.h>
#include "html.h"
#include <vector>
#include "SetMotor.h"

WiFiServer server(100);  // 创建一个端口为100的服务器对象
WebServer webServer(81);

byte Stop[17] = { 0xff, 0x55, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00 };  // 停止命令的缓冲区
String sendBuff;                                                                                                           // 用于存储从串口接收到的数据的缓冲区
String Version = "1.0.1";
bool ED_client = true;                                                                                                     // 跟踪是否有客户端连接的标志
bool WA_en = false;                                                                                                        // 启用客户端数据处理的标志

#define RXD2 14  // 定义Serial2的RX引脚为14号引脚
#define TXD2 13  // 定义Serial2的TX引脚为13号引脚
#define LED 12   // 定义LED的GPIO引脚为12号引脚
void CameraWebServer_init();  // 声明初始化相机Web服务器的函数.

uint32_t Car_Speed = 40;  // 定义车辆速度变量

void setup() {
  Serial.begin(115200);  // 初始化串口波特率为115200
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  // 初始化Serial2，用于与相机通信
  i2c_init();  // 初始化I2C通信
  CameraWebServer_init();  // 调用初始化相机Web服务器的函数
  server.begin();  // 启动Web服务器
  delay(100);  // 延时100毫秒

  ledcSetup(0, 3000, 8);  // 设置PWM通道0的频率为3000Hz，分辨率为8位
  ledcAttachPin(LED, 0);  // 将LED引脚绑定到PWM通道0
  ledcWrite(0, 25);  // 设置PWM通道0的占空比为25

  webServer.on("/", []() {  // 定义Web服务器的根目录处理函数
    webServer.send(200, "text/html", html);  // 发送HTML页面
  });

  webServer.on("/control", []() {  // 定义Web服务器的控制处理函数
    String cmd = webServer.arg("cmd");  // 获取URL参数cmd
    Serial.println(cmd);  // 打印cmd到串口

    std::vector<uint8_t> data;  // 定义一个字节向量用于存储数据
    if (cmd == "car") {  // 如果cmd为"car"，则处理车辆控制指令
      String direction = webServer.arg("direction");  // 获取URL参数direction
      data = { 0xFF, 0x55, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0C, 0x00 };  // 初始化数据向量
      
      if (direction == "Forward") {  // 如果direction为"Forward"，则车辆前进
        Car_forward(Car_Speed);  // 调用Car_forward函数
        data.push_back(0x01);  // 添加数据到向量
      }
      if (direction == "Backward") {  // 如果direction为"Backward"，则车辆后退
        Car_backwards(Car_Speed);  // 调用Car_backwards函数
        data.push_back(0x02);  // 添加数据到向量
      }
      if (direction == "Left") {  // 如果direction为"Left"，则车辆左转

        Car_left(Car_Speed);  // 调用Car_left函数
        data.push_back(0x03);  // 添加数据到向量
      }
      if (direction == "Right") {  // 如果direction为"Right"，则车辆右转
        Car_right(Car_Speed);  // 调用Car_right函数
        data.push_back(0x04);  // 添加数据到向量
      }
      if (direction == "Anticlockwise") {  // 如果direction为"Anticlockwise"，则处理相关指令
        data.push_back(0x09);  // 添加数据到向量
      }
      if (direction == "Clockwise") {  // 如果direction为"Clockwise"，则处理相关指令
        data.push_back(0x0A);  // 添加数据到向量
      }
      if (direction == "stop") {  // 如果direction为"stop"，则车辆停止
        Car_stop();  // 调用Car_stop函数
        data.push_back(0x00);  // 添加数据到向量
      }
    }

    if (cmd == "Quality") {  // 如果cmd为"Quality"，则处理图像质量控制指令

      String angle = webServer.arg("angle");  // 获取URL参数angle
      uint32_t angleValue = angle.toInt(); 
      Serial.println(angleValue);
      sensor_t *s = esp_camera_sensor_get();  // 获取摄像头传感器对象
      if (angleValue == 1) s->set_framesize(s, FRAMESIZE_QQVGA);   // 设置图像质量
      if (angleValue == 2) s->set_framesize(s, FRAMESIZE_VGA);  // 设置图像质量
      if (angleValue == 3) s->set_framesize(s, FRAMESIZE_XGA);    // 设置图像质量

      s->set_vflip(s, -1);   // 设置图像上下翻转
      s->set_hmirror(s, 1);  // 设置图像左右镜像

    }

    if (cmd == "Resolution") {  // 如果cmd为"Resolution"，则处理分辨率控制指令
      String angle = webServer.arg("angle");  // 获取URL参数angle
      Car_Speed = angle.toInt();  // 将angle转换为整数并设置为车辆速度
      Serial.println(Car_Speed);  // 打印车辆速度到串口
    }    

    if (cmd == "Lights") {  // 如果cmd为"Lights"，则处理灯光控制指令
      String angle = webServer.arg("angle");  // 获取URL参数angle
      uint32_t angleValue = angle.toInt();  // 将angle转换为整数
      ledcWrite(0, angleValue);  // 设置PWM通道0的占空比为angleValue
      Serial.println(angleValue);  // 打印angleValue到串口
    }
    uint8_t *buffer = &data[0];
    Serial2.write(buffer, data.size());
    webServer.send(200, "text/plain", "ok");
  });
  
  webServer.begin();
}
void loop() {
  uint8_t numStations = WiFi.softAPgetStationNum();
  if(numStations)webServer.handleClient();
   else Car_stop(); 
}
