#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Replace with your smartwatch's service UUID
#define WATCH_SERVICE_UUID "your-watch-service-uuid-here"

// To store the found BLE device
BLEAdvertisedDevice* myDevice;

// Callback for when a BLE device is detected during scanning
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.print("Found Device: ");
        Serial.println(advertisedDevice.getName().c_str());

        // Check if the device advertises the desired service UUID (smartwatch)
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(WATCH_SERVICE_UUID))) {
            Serial.println("Smartwatch found!");
            myDevice = new BLEAdvertisedDevice(advertisedDevice);  // Store the found device
            BLEDevice::getScan()->stop();  // Stop scanning once the device is found
        }
    }
};

// BLE Client setup
void connectToSmartwatch() {
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println("Connecting to smartwatch...");

    // Connect to the smartwatch
    pClient->connect(myDevice);
    Serial.println("Connected to smartwatch");

    // Get the smartwatch's service
    BLERemoteService* pRemoteService = pClient->getService(BLEUUID(WATCH_SERVICE_UUID));
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find the service UUID: ");
        Serial.println(WATCH_SERVICE_UUID);
        pClient->disconnect();
        return;
    }

    // Now you can read specific characteristics (e.g., heart rate)
    BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(BLEUUID("your-characteristic-uuid-here"));
    if (pRemoteCharacteristic == nullptr) {
        Serial.println("Failed to find the characteristic.");
    } else {
        // Read the characteristic value
        std::string value = pRemoteCharacteristic->readValue();
        Serial.print("Characteristic value: ");
        Serial.println(value.c_str());
    }

    pClient->disconnect();  // Disconnect after reading
}

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");

    // Start scanning for BLE devices
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);  // Scan for 30 seconds
}

void loop() {
    // If the smartwatch is found, connect to it
    if (myDevice) {
        connectToSmartwatch();
        myDevice = nullptr;  // Reset after connecting
    }

    delay(2000);  // Optional delay to avoid rapid reconnection attempts
}
