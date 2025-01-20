#ifndef SERMOTOR_H
#define SERMOTOR_H
#include "Arduino.h"
#include <Wire.h>

#define sda 16  // 定义I2C的SDA引脚为16号引脚
#define scl 13  // 定义I2C的SCL引脚为13号引脚
uint8_t slave_addr = 0x30;  // 定义I2C从设备的地址

void i2c_init() {
  Serial.begin(115200);  // 初始化串口波特率为115200
  Wire.begin(sda, scl);  // 以master模式加入I2C总线，指定SDA和SCL引脚
}

void i2c_Write(uint8_t d1, uint8_t d2, uint16_t speed) {
  Wire.beginTransmission(slave_addr>>1);  // 向从设备发送数据
  Wire.write(0x00);    // 发送数据包头

  Wire.write(d1);      // 发送右电机控制指令
  Wire.write(d2);      // 发送左电机控制指令
  Wire.write(speed);   // 发送PWM速度调节值

  Wire.endTransmission();  // 结束数据发送
}

void Car_run(int d1, int d2, uint16_t speed) {  // 函数接收右电机、左电机状态和速度参数
  if(d1==2) d1=2;   // 如果d1为2，则设置为2（反转）
  if(d2==2) d2=2;   // 如果d2为2，则设置为2（反转）

  i2c_Write(d1, d2, speed);  // 调用i2c_Write函数发送数据
}

void Car_forward(uint16_t speed) {
  Car_run(1, 1, speed);  // 前进，左右电机都正转
}

void Car_backwards(uint16_t speed) {
  Car_run(2, 2, speed);  // 后退，左右电机都反转
}

void Car_left(uint16_t speed) {
  Car_run(1, 2, speed);  // 左转，右电机反转，左电机正转
}

void Car_right(uint16_t speed) {
  Car_run(2, 1, speed);  // 右转，右电机正转，左电机反转
}

void Car_stop() {
  Car_run(0, 0, 0);  // 停止，左右电机都停止
}

#endif