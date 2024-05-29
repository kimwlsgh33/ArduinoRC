#include <WiFi.h>
#include <WiFiClient.h>                                                  // TCP/IP을 사용하기 위해 WiFiClient 라이브러리 추가

const char* ssid = "YourWiFiSSID";                                      // WiFi 네트워크 이름
const char* password = "YourWiFiPassword";                               // WiFi 비밀번호
WiFiServer server(80);                                                    // WIFI 객체 생성 : ESP32에 WIFI 서버를 설정하고, 해당 서버가 포트 80
                                                                        //에서 클라이언트의 요청을 수신할 수 있도록 하기 위함( HTTP 프로토콜에서는 기본적으로 포트 80을 사용)

#define MOTOR_PIN_1 3                                                  // 모터 제어 핀 1 (예: GPIO 21)
#define MOTOR_PIN_2 5                                                  // 모터 제어 핀 2 (예: GPIO 22)
#define MOTOR_PIN_3 7
#define MOTOR_PIN_4 9

void setup() {
  Serial.begin(115200);                                                 // 시리얼 통신 시작
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(MOTOR_PIN_3, OUTPUT);
  pinMode(MOTOR_PIN_4, OUTPUT);

  // WiFi 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // 서버 시작
  server.begin();
  Serial.println("TCP/IP server started");                              // TCP/IP 서버로 변경됨
}

void loop() {
  WiFiClient client = server.available();                                 // 클라이언트 요청 확인
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";                                            // 클라이언트의 요청을 저장할 문자열 초기화
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();                                         // 클라이언트로부터 바이트 수신
        Serial.write(c);                                                // 시리얼 모니터에 출력
        if (c == '\n') {
          if (currentLine.length() == 0) {
            cout << "command isn't available" << endl;
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // 연결 종료
    delay(100);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
