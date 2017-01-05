Remote Control Car
===
language: [English](https://github.com/yuukidach/Remote-Control-Car/blob/master/README.en.md)

## 概括
这是我参加比赛的一个参赛作品，比赛要求能在“大赛要求.pdf”里看到

## 功能
1. 利用PS2控制小车行进方向;
	
	因为PS2的摇杆不好用，所以我这里利用按钮来操作。当然，如果你喜欢用摇杆的话也是可以用的。当摇杆被推到极限以后，作用和按钮的效果是一样的。

2. 通过机械臂进行物体的抓取;
  
  这里抓取的物体为2个边长为5cm的方形物块和一个直径为5cm的圆形物快。

3. 黑线循迹;

4. 探测红外灯头并触碰开关;

5. 自调整上30°的斜坡

## 材料
1. STM32F103 

2. PS2

3. L298N

4. 直流电机-25GA370

5. 电池 11.1V

6. 杜邦线

7. 面包板

8. 小车底盘

9. 开关

10. 机械臂(包括舵机)

11. PCA9685

12. 红外复眼

13. 循迹模块

14. 超声波模块

15. 稳压器

16. MPU6050

## 详细描述
![小车底盘](https://github.com/yuukidach/Remote-Control-Car/blob/master/Pictures/%E5%BA%95%E7%9B%98.jpg?raw=true)
小车底板定为常见的方形底板，最终结构为四层：
底  层：超声波、前灰度传感器、红外寻迹灯、后灰度传感器以及电机；
第一层：前红外复眼、电池、后红外复眼；
第二层：机械臂底座、PS2遥控接收器、降压器、电子陀螺仪、左L298N、右L298N；
第三层：寻迹模块、PCA模块、STM32、寻迹模块；
顶  层：燃料收纳槽。

![](https://github.com/yuukidach/Remote-Control-Car/blob/master/Pictures/%E7%94%B5%E6%BA%90%E5%88%86%E9%85%8D%E5%9B%BE.jpg?raw=true)

![](https://github.com/yuukidach/Remote-Control-Car/blob/master/Pictures/%E7%B3%BB%E7%BB%9F%E4%BF%A1%E5%8F%B7%E5%9B%BE.jpg?raw=true)
