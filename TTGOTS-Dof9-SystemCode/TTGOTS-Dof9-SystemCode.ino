#include <MPU9250_asukiaaa.h>
#include "BluetoothSerial.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_TFTShield18.h>

Adafruit_ST7735 tft = Adafruit_ST7735(16, 17, 23, 5, 9); // CS,A0,SDA,SCK,RESET

//9軸I2C引腳
#define SDA_PIN 21
#define SCL_PIN 22

BluetoothSerial SerialBT;
boolean confirmRequestPending = true;

MPU9250_asukiaaa mySensor(MPU9250_ADDRESS_AD0_HIGH);

String ComeValue;     //傳來的藍牙數據
String GetValue;      //傳去的藍芽數據

String TitleSSPName = "Dof9Sys0"; //藍芽通道名稱及開頭名稱,暫定為Dof9Sys+(出廠編號)

float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;//感測值

const int buttonB = 37; 
int ButtonBState = 0;
uint32_t numValTemp;

void BTConfirmRequestCallback(uint32_t numVal)
{
    confirmRequestPending = true;
    //Serial.println(numVal);
    numValTemp = numVal;    
}

void BTAuthCompleteCallback(boolean success)
{
  confirmRequestPending = false;
  if (success)
  {
    Serial.begin(9600);
    Serial.println("Pairing success!!");
  }
  else
  {
    Serial.begin(9600);
    Serial.println("Pairing failed, rejected by user!!");
  }
}

void tftClearScreen() {
  tft.setRotation(0);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(2);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);
}

void tftPrintTitle() {
  tft.setRotation(0);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(18,7);
  tft.println(TitleSSPName);
  delay(500);
}

void setup() {
  //Serial.begin(9600);
  //while(!Serial){Serial.println("Serial Error...");}//通道測試
  //Serial.println("started run!!");
  
  pinMode(27,OUTPUT);//背光控制腳位
  digitalWrite(27,HIGH);//拉高背光腳位;若進省電模式時要拉低。
  tft.initR(INITR_BLACKTAB);//初始化ST7735S chip
  tftClearScreen();//完全清除畫面,消除邊界線用的,若未使用會殘留邊界不明像素
  tftPrintTitle();//初始資訊
  
  pinMode(buttonB, INPUT);
  SerialBT.enableSSP();
  SerialBT.onConfirmRequest(BTConfirmRequestCallback);
  SerialBT.onAuthComplete(BTAuthCompleteCallback);
  SerialBT.begin(TitleSSPName);         
  
  //初始化MPU9250 Library
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
}

void loop() {
  //▼讀取9軸訊號▼
  uint8_t sensorId;//MPU9250 感測器暫存位址
  int result;//MPU9250 感測器暫存器資料
  result = mySensor.readId(&sensorId);
  if (result == 0) {
    //Serial.println("sensorId: " + String(sensorId));
  } 
  else 
  {
    Serial.begin(9600);
    Serial.println("Cannot read sensorId " + String(result));
  }
  result = mySensor.accelUpdate();
  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
  } 
  else 
  {
    //Serial.println("Cannod read accel values");
  }
  result = mySensor.gyroUpdate();
  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
  } 
  else 
  {
    //Serial.println("Cannot read gyro values");
  }
  result = mySensor.magUpdate();
  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
  } 
  else 
  {
    //Serial.println("Cannot read mag values");
  }
  //▲讀取9軸訊號▲

  //▼藍芽通訊▼
  if (confirmRequestPending)//配對機制--當配對時會顯示暗碼
  {
    ButtonBState = digitalRead(buttonB);
    if (ButtonBState == LOW) 
    {
      SerialBT.confirmReply(true);//確認暗碼沒問題後點按IO37號的按鍵執行建立連線。
      tft.fillScreen(ST7735_BLACK);
      tft.setTextColor(ST7735_YELLOW);
      tft.setCursor(25,8);
      tft.println("9Ax-Sys");
      tft.setCursor(0,25);
    }
    else
    {
      tft.setTextColor(ST7735_RED);
      tft.setCursor(0,25);
      tft.println(">Wait link");  
      if(numValTemp!=0)
      {
        tft.println(">PN:"+(String)numValTemp);
        tft.println(">if ok,\n You can\n Push B\n Button...");
      }
    }
  }
   if (SerialBT.available())//確認是否有傳送資料給ESP32
   {
       ComeValue = SerialBT.readString();
       //Serial.print("ComeValue=");//COM Port測試
       //Serial.print(ComeValue);
       //Serial.println();
       tft.setTextColor(ST7735_BLUE);
       tft.println("C>"+ComeValue);
   }
   else//若沒有就持續傳遞9軸資訊過去
   {
      GetValue = String(aX)+String(",")+String(aY)+String(",")+String(aZ)+String(",")+String(gX)+String(",")+String(gY)+String(",")+String(gZ)+","+String(mDirection)+String(millis())+ "ms";
      //輸出9軸值
      SerialBT.println(GetValue);
      //Serial.println(GetValue);//COM Port測試
   }
     
   //▲藍芽通訊▲
}
