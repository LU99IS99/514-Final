#include <SwitecX25.h>
//Client
#include "BLEDevice.h"

#define STEPS (315*3)

SwitecX25 motor1(315*3, D4, D5, D6, D7);

int UVindex = 0;
char* UVindexChar;

const int ledGreen = D1;
const int ledYellow = D2;
const int ledRed = D3;

// BLE 服务器名称和 UUID
#define bleServerName "ESP32-S-test" // BLE 服务名称 // BLE 服务器名称 服务器客户端的名称必须匹配

static BLEUUID pwmServiceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");              // bmeServiceUUID  TO pwmServiceUUID
static BLEUUID UVindexCharacteristicUUID("cba1d466-344c-4be3-ab3f-189f80dd7518"); // temperatureCharacteristicUUID TO UVindexCharacteristicUUID 

static boolean doConnect = false;// 检查是否连接到服务器的变量
static boolean connected = false;

static BLEAddress *pServerAddress;// 要连接的服务器的地址，该地址将在扫描期间找到
 
static BLERemoteCharacteristic* UVindexCharacteristic;// 要读取的特性 temperatureCharacteristic TO UVindexCharacteristic

//Activate notify
const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

boolean newUVindex = false; //是否有新的数据  newTemperature TO newUVindex

//连接服务器
bool connectToServer(BLEAddress pAddress) {
   BLEClient* pClient = BLEDevice::createClient();
 
  pClient->connect(pAddress);//连接服务器
  Serial.println(" - Connected to server");
 
  // 获取服务器中的参数UUID
  BLERemoteService* pRemoteService = pClient->getService(pwmServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(pwmServiceUUID.toString().c_str());
    return (false);
  }
 
  // 获取服务器中的特征
  UVindexCharacteristic = pRemoteService->getCharacteristic(UVindexCharacteristicUUID);

  if (UVindexCharacteristic == nullptr) 
  {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");
 
  //分配回调函数，处理接收到的新数据
  UVindexCharacteristic->registerForNotify(UVindexNotifyCallback);
  return true;
}

// 检查服务器 回调函数
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) { //检查找到的设备是否具有正确的 BLE 服务器名称
      advertisedDevice.getScan()->stop(); //停止扫描
      pServerAddress = new BLEAddress(advertisedDevice.getAddress()); //获取服务器地址
      doConnect = true; //标识true后开始建立连接
      Serial.println("Device found. Connecting!");
    }
  }
};
 
//数据更新通知 回调函数
static void UVindexNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {

  int Tempindex = 0;
  UVindexChar = (char*)pData;
  Tempindex = atoi(UVindexChar);
  if(Tempindex != UVindex)
  {
    newUVindex = true;
  }
  Serial.println("Current UV Index Level:");
  Serial.println(UVindex);
}

void setup(void)
{
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);

  // run the motor against the stops
  motor1.zero();
  Serial.begin(115200);// 115200 的波特率启动串行通信
  Serial.println("Starting Arduino BLE Client application...");

  BLEDevice::init(""); //初始化BLE设备
 
  //扫描附近的设备
  BLEScan* pBLEScan = BLEDevice::getScan();//pBLEScan扫描仪
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());//设置回调函数
  pBLEScan->setActiveScan(true);//启动扫描活动
  pBLEScan->start(30);// 扫描30秒

}

void loop(void)
{
  static int targetPos = 0;
  // the motor only moves when you call update
  motor1.update();

  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
		
      Serial.println("We are now connected to the BLE Server.");
	  
      UVindexCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
    }
    doConnect = false;
  }
  //数据更新，刷新屏幕显示
  if (newUVindex){
    newUVindex = false;
    if(UVindex < 8)
    {
      targetPos = 900/8*UVindex;
      if(UVindex < 3)
      {
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledRed, LOW);
      }
      else if(UVindex >= 3 && UVindex <6)
      {
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, HIGH);
        digitalWrite(ledRed, LOW);
      }
      else
      {
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledYellow, LOW);
        digitalWrite(ledRed, HIGH);
      }
    }
    else
    {
      targetPos = 900;
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledYellow, LOW);
      digitalWrite(ledRed, HIGH);
    }


    motor1.setPosition(targetPos);
  }
}