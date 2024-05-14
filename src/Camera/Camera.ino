#include <SoftwareSerial.h>
#include <OV7670_FIFO.h>
const int rxPin = 0;  // WiFi 모듈의 RX 핀 (아두이노 우노 보드의 0번 핀)
const int txPin = 1;  // WiFi 모듈의 TX 핀 (아두이노 우노 보드의 1번 핀)
SoftwareSerial espSerial(rxPin, txPin); // SoftwareSerial 객체 생성
#define VSYNC_PIN 2
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
OV7670_FIFO mycam;
uint8_t img[320*240*2];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  espSerial.begin(115200);
  
  mycam.begin(115200);
  mycam.setpin_reset(7);
  mycam.setpin_vsync(VSYNC_PIN);
  mycam.reset_fifo_burst();

  espSerial.println("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"");
  delay(5000);
    // WiFi 연결 상태 확인
  while (!espSerial.find("OK")) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
    espSerial.println("AT+CWJAP?");
  }

  // WiFi 연결 성공 시 IP 주소 출력
  Serial.println("WiFi connected");
  Serial.println("IP Address: ");
  espSerial.println("AT+CIFSR");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mycam.read_fifo_length() >= 2) {
    mycam.get_image(img);
    
    // Print the image to the serial monitor
    for (int i = 0; i < sizeof(img); i++) {
      Serial.write(img[i]);
    }
    
    // Wait for a short delay to control frame rate
    delay(100); // Adjust the delay time according to frame rate
  }
}