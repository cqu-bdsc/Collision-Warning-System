# Collision-Warning-System

Collision Warning System，DSRC 

## 进度更新

2018-7-13\
概要设计说明书更新，版本V2.0  

2018-7-19\
详细设计说明书更新

2018-7-24\
Android与PC通过UDP传输JSON完成

2018-7-27\
PC端信息处理模块完成

2018-8-4\
PC端信息显示模块完成

2018-8-5\
Android信息采集模块完成

### 模块设计任务

#### 周易

- [x] UDP通信模块
- [x] 信息处理模块
- [x] 信息显示模块

#### 张浪

- [x] 信息采集模块
- [x] UDP通信模块
- [ ] 显示界面模块

* * *

### 详细设计说明

#### 项目架构

Github 协作开发

Branch 分支

- master\
  		---README.md        本文件\
  		---DOC 			说明文档
- android\
  		---APP 			安卓项目
- pc\
       ---Server PC   QT C++ 桌面UI项目

#### 数据说明

Android端与PC端通信为传输以以下数据格式定义的JSON数据包。

> [JSON介绍](https://www.json.org/json-zh.html)：一种轻量级的数据交换格式。 易于人阅读和编写。同时也易于机器解析和生成。

Android端采集车辆信息后，将其打包成JSON数据包发送给PC，其采集与发送信息格式定义如下。

| 信息   | Item        |  Type  | 备注 |
| ---   | --------    | ----- | ---- |
| ID    | “id”        |  int   | 以本机的MAC地址进行Hash得到|
| 时间戳 | “timeStamp” |  int   | 单位：秒（s）,以发送事件到1970年第一秒的秒数 |
| 速度   | “speed”     | double | 单位：m/s, 保留3位小数，车辆车头方向的速度信息   |
| 方向   | “direction” | int    | 车辆车头方向的朝向，0为正北，90正东，-90正西，180或-180正南|
| 纬度   | “lat”       | double |使用百度SDK获得GPS定位信息，保留3位   |
| 经度   | “lon”       | double |同上 |
|加速度  | “acc”       | double | 单位：m/s^2, 保留1位小数   |

PC端接收到Android端发送的JSON数据后，对其进行解析并进行处理，同时处理完成后将结果返回给Android端，其返回的信息格式定义如下。

| 信息    | Item        |  Type  | 备注 |
| ---     | --------    | -----  | ---- |
| ID      | “id”        |  int   | 以发送过来的车辆ID为准|
| 碰撞时间 | “time”      |  int   | 单位：秒（s） |
| 碰撞距离 | “distance”   | double | 单位：m，保留3位小数  |
| 警报     | “warning” | boolean| True: 碰撞， False: 不碰撞|
| 地图     |             |        |   |
| 碰撞轨迹 |             |        |    |

