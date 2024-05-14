
#include <WiFi.h>

const char* ssid = "YourWiFiSSID";                                      // WiFi 네트워크 이름
const char* password = "YourWiFiPassword";                               // WiFi 비밀번호
WiFiServer server(80);                                    // WIFI 객체 생성 : ESP32에 WIFI 서버를 설정하고, 해당 서버가 포트 80
                                                          //에서 클라이언트의 요청을 수신할 수 있도록 하기 위함( HTTP 프로토콜에서는 기본적으로 포트 80을 사용)

#define MOTOR_PIN_1 3                                     // 모터 제어 핀 1 (예: GPIO 21)
#define MOTOR_PIN_2 5                                  // 모터 제어 핀 2 (예: GPIO 22)
#define MOTOR_PIN_3 7
#define MOTOR_PIN_4 9

void setup() {
  Serial.begin(115200);                                      // 시리얼 통신 시작
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
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  WiFiClient client = server.available();       // 클라이언트 요청 확인
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";                    // 클라이언트의 HTTP 요청을 저장할 문자열 초기화
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();                 // 클라이언트로부터 바이트 수신
        Serial.write(c);                        // 시리얼 모니터에 출력
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html><head><title>ESP32 Web Server</title></head><body>");
            client.println("<h1>Welcome to ESP32 Web Server</h1>");
            client.println("<p>Click <a href=\"/?F\">Forward</a> to move forward.</p>");
            client.println("<p>Click <a href=\"/?B\">Backward</a> to move backward.</p>");
            client.println("<p>Click <a href=\"/?L\">Left</a> to move left.</p>");
            client.println("<p>Click <a href=\"/?R\">Right</a> to move right.</p>");
            client.println("<p>Click <a href=\"/?BL\">BackLeft</a> to move BackLeft.</p>");
            client.println("<p>Click <a href=\"/?BR\">BackRight</a> to move BackRight.</p>");
            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // 모터 제어
    if (currentLine.indexOf("F") != -1) {
      // 전진
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
    } else if (currentLine.indexOf("B") != -1) {
      // 후진
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
    } else if (currentLine.indexOf("L") != -1) {
                                                           // 좌회전
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
      delay(1000);                                            // 회전을 위한 딜레이
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
    } else if (currentLine.indexOf("R") != -1) {
                                                         // 우회전
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
      delay(1000);                                          // 회전을 위한 딜레이
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
    } else if (currentLine.indexOf("LB") != -1) {
                                                               // 왼쪽으로 후진
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
    } else if (currentLine.indexOf("RB") != -1) {
                                                              // 오른쪽으로 후진
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
    }

    // 연결 종료
    delay(100);
    client.stop();
    Serial.println("Client disconnected.");
  }
}