# 基於PIC18F MCU之多功能車系統
## 作品照片
  
![image](https://i.imgur.com/90pkBFD.jpg)  
## PIC18F 晶片介紹
- 8-bit microcontrollers
- using the Harvard architecture
- two-stage instruction pipeline
- RISC-based
- memory-mapped I/O
## 系統環境
- IDE：MPLAB 5.25
- 程式語言：C language
- 編譯器：XC8
- 燒錄器具：pickit3 / pickit4
- 晶片: pic18f4520
## 功能說明
- 可用遙控器控制前、後、左、右、停，並在車子行進時搭配點矩陣顯示對應箭頭方向。
- 遙控器按下1可進入走迷宮模式，蜂鳴器會鳴唱馬力歐音樂。在此模式下，紅外線三路循跡模組會感應地面為白色或黑色，其中地圖為兩張全開紙做成的白底黑線的迷宮。當壓到黑線時，蜂鳴器會發出嗶嗶聲，並且點矩陣顯示器顯示哭臉。
- 車頭有超音波感測器持續偵測前方障礙物距離，我們放了一片板子在終點，當感測距離小於7公分時，蜂鳴器會鳴唱馬力歐音樂，LED燈則搭配音樂節奏閃爍，同時點矩陣會顯示笑臉。
- 按下3可進入延黑軌道前行的模式，紅外線循跡模組會持續感應車子是否有走在黑線中間，當偏左或偏右時，會自動調整的前進方向。在此模式下，蜂鳴器會鳴唱火車快飛的音樂。
- 按下7，車子的七彩燈會發出七彩燈光。
- 按下9，會有多顆LED呼吸燈。
- 當進入迷宮模式或沿線模式時，七段顯示器會開始計秒：到達終點時則會停止計秒。
## 使用硬體
- PIC18F4520晶片*4
- L298N馬達驅動模組
- HS0038B紅外線模組
- HC-SR04超音波模組
- KSM117 三路循跡模組
- 共陽極5mm透明高亮 RGB LED (七彩燈)
- 七段顯示器
- 點矩陣顯示器
- 無源蜂鳴器
## 程式檔案說明
- controller.c：燒錄於主晶片，接受紅外線訊號，控制其他三個PIC18F晶片的行為
- tracker.c：傳送紅外線三路循跡模組感測值至主晶片
- ultrasound.c：傳送超音波偵測距離之結果給主晶片
- buzzer.c：接受主晶片的訊號鳴唱指定歌曲
- motor.c：接受主晶片的訊號控制馬達輪子
- LED.c：接受主晶片的訊號控制各色LED
## 系統架構
  
![image](https://i.imgur.com/QirdhJX.png)  
## 流程圖
  
![image](https://i.imgur.com/URJqT7E.png)  
## demo影片
https://www.youtube.com/watch?v=tqTttgeLKx8