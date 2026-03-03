# 퍼스널 모빌리티의 안전을 위한 임베디드 헬멧 시스템

<br>

## 📝 프로젝트 개요
* **수행 기간:** 2022. 09. 01. ~ 2022. 12. 20. (경희대학교)
* **수행 인원:** 2명
* **프로젝트 목표:** 퍼스널 모빌리티(전동 킥보드 등) 이용자의 사고를 예방하고, 사고 발생 시 신속한 구조 요청이 가능한 임베디드 헬멧 시스템 개발
* **주요 내용:**
  * **사고 감지 및 긴급 신고**: 진동 센서를 통한 충격 감지 시 스마트폰 연동 구조 메시지 전송
  * **조도 기반 LED 제어**: 주변 밝기에 따른 주행등(LED) 자동 점등을 통한 시각적 인지 보조
  * **후방 물체 감지**: 초음파 센서를 활용한 후방 접근 물체 감지 및 단계별 경고(LED/버저) 수행
<br>

---

## 👤 나의 역할 및 수행 업무
* **담당 역할:** 시스템 아키텍처 설계, MCU 펌웨어 개발, 센서/통신 인터페이스 구현 및 통합
* **수행 업무:**
    * 시스템 사양 결정(표준 안전거리 및 사고 대처 시간 고려) 및 HW/SW 설계 로직 구성
    * **사고 감지 기능**: 외부 인터럽트를 활용한 진동 센서 충격 감지 및 사고 처리 알고리즘 구현
    * **통신 기능**: USART1 기반의 블루투스 모듈 인터페이스 개발 및 스마트폰 연동 데이터 송신 구현
    * **후방 감지 기능**: 초음파 센서 제어 및 거리 데이터 기반 단계별 경고 알림 로직 개발
    * 전체 시스템 통합, 회로 납땜 및 기능 검증
<br>

---

## 🛠️ 사용 기술 및 환경

| 분류 | 상세 내용 |
| :--- | :--- |
| **Languages** | C |
| **MCU** | AVR ATmega128 |
| **Sensors** | Vibration Sensor, Ultrasonic Sensor, Ambient Light Sensor, LED, Buzzer |
| **Protocols** | UART, Bluetooth |
| **Tools** | AVR Studio 4, JTAG ICE |
<br>

---

## ⚙️ 시스템 아키텍처
**System Block Diagram**<br>
<img width="756" height="277" alt="image" src="https://github.com/user-attachments/assets/800444a9-e47a-4f69-b270-a7e6810cf4c4" />
<br>

**System Flowchart**<br>
<img width="748" height="611" alt="image" src="https://github.com/user-attachments/assets/ad3c0406-bb67-4342-9c1e-872092a6bfbc" />
<br>

**Circuit Diagram**<br>
<img width="919" height="678" alt="KakaoTalk_20221216_233648538" src="https://github.com/user-attachments/assets/58cafe9a-f324-40ab-bac2-19c3cf7be240" />


<br>

---

## 🎬 주요 기능 및 결과물
**전체 기능 설명 및 데모 영상**

[![임베디드 헬멧 프로젝트 결과 영상](https://img.youtube.com/vi/RcIIAL9TnNg/0.jpg)](https://youtu.be/RcIIAL9TnNg)

<br>

---

## 🔍 트러블슈팅 및 회고

#### 📌 Case #1. 인터럽트 간섭에 의한 초음파 거리 측정 오차 및 기능 오동작 해결
* **문제 상황:** 개별 기능 통합 후, 충격 감지 시 초음파 센서를 통한 후방 거리 측정값이 비정상적으로 커지거나 시스템이 원활하게 동작하지 않는 현상 발생
* **원인 분석:**
  * **(1)** 진동 센서가 충격을 감지했을 때 발생하는 외부 인터럽트 서비스 루틴(ISR) 내부에 5초의 Time Delay와 LED/버저 동작 로직이 포함되어 ISR 수행 시간이 과도하게 길어짐
  * **(2)** 초음파 센서가 ECHO 신호의 폭을 측정하는 동안 긴 ISR이 실행되면서 타이머 값이 비정상적으로 누적되어 거리 오차 발생
* **해결 방법:**
  * **ISR 최적화**: 인터럽트 처리 루틴에서는 최소한의 변수 플래그(Flag)만 세팅하도록 변경
  * **로직 구조 개선**: 실시간성이 중요한 거리 측정 기능을 보호하기 위해, if-else 조건문을 통해 충격 대응 로직과 후방 감지 로직을 명확히 분리하여 상호 간섭 제거
<br>
