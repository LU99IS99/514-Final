#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

int UVindex = 0;  //紫外线等级

// BLE 服务器名称 服务器客户端的名称必须匹配
#define bleServerName "ESP32-S-test"

// 计时器变量
unsigned long lastTime = 0;
unsigned long timerDelay = 100;

bool deviceConnected = false; // 设备已连接布尔变量

// 服务、以摄氏度为单位的温度特性，湿度定义UUID
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59" // 服务 UUID

//  Characteristic特征  Descriptor描述 
  BLECharacteristic UVindexCharacteristic(
                      "cba1d466-344c-4be3-ab3f-189f80dd7518", 
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  BLEDescriptor UVindexDescriptor(BLEUUID((uint16_t)0x2902));

//回调函数 
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;// 客户端连接到服务器，状态为true
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  Serial.begin(115200);
  
  //使用BLE服务器名称创建一个新的BLE设备
  BLEDevice::init(bleServerName);

  // 将 BLE 设备设置为服务器并分配回调函数   
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service                                          bmeService
  BLEService *bmeService = pServer->createService(SERVICE_UUID); // 使用之前定义的服务 UUID 启动 BLE 服务

  bmeService->addCharacteristic(&UVindexCharacteristic);
  UVindexDescriptor.setValue("UVindex");
  UVindexCharacteristic.addDescriptor(&UVindexDescriptor);

  // 启动服务，服务器启动广告
  bmeService->start(); 
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  static int flag = 0;
  float sensorVoltage; 
  float sensorValue;
  sensorValue = analogRead(A0);

  sensorVoltage = sensorValue/1024*3.3;
  S12SD_level(sensorVoltage);
  // 蓝牙通信
  if (deviceConnected) 
  {
    //初次连接成功
    if(flag == 0)
    {
      flag = 1;
      Serial.println("Client connection successful");
    }

    static char UVindexTemp[2];
    dtostrf(UVindex, 2, 0, UVindexTemp);
    UVindexCharacteristic.setValue(UVindexTemp);
    UVindexCharacteristic.notify();
  }
  else
  {
    Serial.println("client connection failed");
  }
  
  delay(1000);
}

void S12SD_level(float sensorVoltage)
{
  if(sensorVoltage < 50)
  {
    UVindex = 0;
  }
  else if(sensorVoltage < 227 && sensorVoltage >= 50)
  {
    UVindex = 1;
  }
  else if(sensorVoltage < 318 && sensorVoltage >= 227)
  {
    UVindex = 2;
  }
  else if(sensorVoltage < 408 && sensorVoltage >= 318)
  {
    UVindex = 3;
  }
  else if(sensorVoltage < 503 && sensorVoltage >= 408)
  {
    UVindex = 4;
  }
  else if(sensorVoltage < 606 && sensorVoltage >= 503)
  {
    UVindex = 5;
  }
  else if(sensorVoltage < 696 && sensorVoltage >= 606)
  {
    UVindex = 6;
  }
  else if(sensorVoltage < 795 && sensorVoltage >= 696)
  {
    UVindex = 7;
  }
  else if(sensorVoltage < 881 && sensorVoltage >= 795)
  {
    UVindex = 8;
  }
  else if(sensorVoltage < 976 && sensorVoltage >= 881)
  {
    UVindex = 9;
  }
  else if(sensorVoltage < 1079 && sensorVoltage >= 976)
  {
    UVindex = 10;
  }
  else
  {
    UVindex = 11;
  }
}












