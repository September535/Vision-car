# Vision Car

> ESP32-CAM 视觉小车、基础电机实验、STC 固件与 PS3 控制组件的示例集合。

## 项目内容

| 目录 | 说明 |
| --- | --- |
| `1 Control the LEDs of the Car` | LED 开关实验 |
| `2 Control the Brightness of LEDs` | LED PWM 调光实验 |
| `3 Motor Drive Mode` | 小车电机驱动实验 |
| `4 Video Smart Car` | ESP32-CAM 视频与网页控制 |
| `5 STC Burn Firmware` | STC 电机控制固件及工程文件 |
| `PS3_Controller_Host` | ESP32 PS3 手柄控制库和示例 |

仓库主体位于内层 `Vision-car/` 目录。

## 主要功能

- LED 与 PWM 基础实验
- 小车电机方向和速度控制
- ESP32-CAM 视频传输
- 浏览器端小车控制
- STC 从控电机固件
- PS3 手柄连接、按键、震动和传感器示例

## 开发环境

- Arduino IDE 与 ESP32 Arduino Core
- ESP32-CAM
- STC / 8051 工程工具（用于 `5 STC Burn Firmware`）
- PS3 Controller Host 依赖按嵌套目录说明安装

## 使用方式

1. 先从 LED 或电机示例验证硬件。
2. 使用视频小车前，确认摄像头型号、Wi-Fi 参数和电机引脚。
3. 需要 PS3 控制时，按 `PS3_Controller_Host/README.md` 配置库和蓝牙地址。
4. 仅在硬件版本匹配时烧录 STC 固件。
5. 小车离地完成方向、急停和网络断开测试后再落地运行。

## 仓库维护说明

仓库包含 Keil/STC 的 `Listings/`、`Objects/` 和预编译 HEX 文件。源码与可再生成产物目前混合存放，后续适合拆分为“源码”“发布固件”和“工具生成文件”。

## 许可证

当前仓库根目录未附带统一许可证。嵌套第三方库可能采用独立授权，使用和分发时应分别检查。
