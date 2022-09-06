# TTGOTS-Dof9-System

使用ArduinoIDE編譯的TTGOTS開發版，版上編號是2.0，TftLcd螢幕是1.8寸，本專案是用於九軸感測與藍芽傳輸與螢幕顯示之整合開源專案，可綁在欲測物件上使用(例如:人體四肢、動物四肢、機械手臂、噴射飛彈...)，但此主分支不包含陀螺儀姿態解算，僅有單純9軸數據的藍芽傳輸，若欲求快速姿態解算請選擇[AHRS](https://github.com/Chen-Wi-Ki/TTGOTS-Dof9-System/tree/AHRS)分支。

![](/Image/img1.jpg)

## 硬體架購

[TTGO-TS開發版](https://www.ruten.com.tw/item/show?22209065526773)

[黑色綁帶](https://shopee.tw/%E3%80%90%E7%8F%BE%E8%B2%A8%E3%80%91%E5%BD%88%E6%80%A7%E8%90%8A%E5%8D%A1%E5%BF%83%E7%8E%87%E8%87%82%E5%B8%B6-%E5%BF%83%E8%B7%B3%E5%B8%B6-%E5%85%89%E5%AD%B8%E5%BC%8F%E5%BF%83%E8%B7%B3%E5%B8%B6-%E6%9B%BF%E6%8F%9B%E8%87%82%E5%B8%B6-%E5%BF%83%E7%8E%87%E5%B8%B6-TopAction-Scosche-rhythm-i.206020478.12324114699)

[3D列印件](https://github.com/Chen-Wi-Ki/TTGOTS-Dof9-System/tree/main/CAD)

3.7V鋰電池(容量隨意，正負極別接錯就好了)

## 關於功能部分

1.藍牙SPP配對後，會不間斷傳送9軸原始藍芽訊號給連接的設備

2.按鈕A是關機鍵(長按約3秒後會進入10uA的深度睡眠模式)，按鈕C是藍芽配對鍵+關機時的重啟鍵，按鍵B暫無作用

4.TFT-LCD作為藍芽配對資訊或指令接收顯示使用,不另外顯示9軸資訊
