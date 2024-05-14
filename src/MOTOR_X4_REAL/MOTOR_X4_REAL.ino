
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "YourWiFiSSID";          // WiFi 네트워크 이름
const char* password = "YourWiFiPassword";  // WiFi 비밀번호
WebServer server(80);                       // 웹 서버 객체 생성 : ESP32에 웹 서버를 설정하고, 해당 서버가 포트 80
//에서 클라이언트의 요청을 수신할 수 있도록 하기 위함( HTTP 프로토콜에서는 기본적으로 포트 80을 사용)

#define MOTOR_PIN_1 21  // 모터 제어 핀 1 (예: GPIO 21)
#define MOTOR_PIN_2 22  // 모터 제어 핀 2 (예: GPIO 22)

void setup() {
  Serial.begin(115200);  // 시리얼 통신 시작
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);

  // WiFi 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // 서버 시작
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // 클라이언트 요청 처리
}

void handleRoot() {
  if (server.args() > 0) {           // 인자가 있는 경우
    String command = server.arg(0);  // 첫 번째 인자를 가져옴
    Serial.print("Received command: ");
    Serial.println(command);

    // 명령에 따라 모터 제어
    if (command == "F") {
      // 전진
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
    } else if (command == "B") {
      // 후진
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
    } else if (command == "L") {
      // 좌회전
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
      delay(1000);  // 회전을 위한 딜레이
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
    } else if (command == "R") {
      // 우회전
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
      delay(1000);  // 회전을 위한 딜레이
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
    } else if (command == "LB") {
      // 왼쪽으로 후진
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
    } else if (command == "RB") {
      // 오른쪽으로 후진
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
    }
    server.send(200, "text/plain", "OK");  // 성공 응답 전송
  } else {
    server.send(400, "text/plain", "Bad Request");  // 잘못된 요청 응답
  }
}
