#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

String folderPath =
    "/home/바탕화면/3D_Printed_Arduino_RC_CAR-4772279/images/"; // 이미지들이
                                                                // 저장된 폴더
                                                                // 경로를 정의
void template_matching(); // 템플릿 매칭 함수 선언

int main() {
  template_matching(); // 템플릿 매칭 함수 호출

  return 0;
}

void template_matching() {
  // 메인 이미지와 템플릿 이미지를 지정된 폴더에서 로드
  Mat img = imread(folderPath + "circuit.bmp", IMREAD_COLOR);
  Mat templ = imread(folderPath + "crystal.bmp", IMREAD_COLOR);

  // 이미지가 성공적으로 로드되었는지 확인
  if (img.empty() || templ.empty()) {
    cerr << "Image load failed!" << endl;
    return;
  }

  img =
      img +
      Scalar(50, 50,
             50); // 메인 이미지를 밝게 하기 위해 각 픽셀 값에 스칼라 값을 더함

  Mat noise(img.size(),
            CV_32SC3); // 메인 이미지와 같은 크기의 노이즈 행렬을 생성
  randn(noise, 0, 10); // 평균 0, 표준편차 10인 정규분포를 따르는 난수(정의된
                       // 범위 내에서 무작위로 추출된 수)를 노이즈 행렬에 채움
  add(img, noise, img, Mat(), CV_8UC3); // 노이즈를 메인 이미지에 추가

  Mat res, res_norm; // 템플릿 매칭 결과와 정규화된 결과를 위한 행렬
  matchTemplate(
      img, templ, res,
      TM_CCOEFF_NORMED); // TM_CCOEFF_NORMED 방법을 사용하여 템플릿 매칭 수행
  normalize(res, res_norm, 0, 255, NORM_MINMAX,
            CV_8U); // 결과를 [0, 255] 범위로 정규화

  // 매칭 결과의 최대값과 그 위치를 저장할 변수
  double maxv;
  Point maxloc;

  // 결과 행렬에서 최소값과 최대값, 그 위치를 찾음
  minMaxLoc(res, 0, &maxv, 0, &maxloc);
  // 최대값 (유사도 점수)을 출력
  cout << "maxv: " << maxv << endl;

  // 메인 이미지에서 검출된 영역에 사각형을 그림
  rectangle(img, Rect(maxloc.x, maxloc.y, templ.cols, templ.rows),
            Scalar(0, 0, 255), 2);

  // 템플릿 이미지, 정규화된 결과, 그리고 검출된 영역이 있는 메인 이미지를 표시
  imshow("templ", templ);
  imshow("res_norm", res_norm);
  imshow("img", img);

  waitKey();           // 키 입력을 무한정 대기
  destroyAllWindows(); // 모든 창을 닫음
}
