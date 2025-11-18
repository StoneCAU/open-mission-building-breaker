# 🎮 Building Breaker: Samurai Edition

> 클래식 플래시 게임 '키루비루(斬る·ビル)'의 현대적 C++ 리메이크  
> 우아한테크코스 8기 오픈미션 프로젝트

---

## 📸 게임 화면

### 메인 화면
<img width="595" height="469" alt="image" src="https://github.com/user-attachments/assets/40007884-eb37-440b-908c-810e5764608c" />

### 게임플레이
<img width="596" height="466" alt="image" src="https://github.com/user-attachments/assets/596d0c14-2377-4475-89dc-e2041dd7b6d6" />

### 게임오버
<img width="596" height="467" alt="image" src="https://github.com/user-attachments/assets/118238df-37be-4a21-8fbb-f138008144f3" />

---

## 📖 프로젝트 소개

### 게임 개요

**Building Breaker**는 하늘에서 끝없이 떨어지는 건물을 검으로 베어 파괴하는 액션 게임입니다. 플레이어는 사무라이 캐릭터를 조작하여 좌우로 이동하며 건물을 공격하고, 떨어지는 건물을 방어하며, 적절한 타이밍에 필살기를 활용해야 합니다. 최대한 오래 생존하며 높은 점수를 달성하는 것이 목표입니다.

**핵심 메커니즘**
- 🗡️ **공격**: 건물 최하단 층을 파괴
- 🛡️ **방어**: 떨어지는 건물을 튕겨내기
- ⚡ **필살기**: 화면의 모든 건물을 한 번에 파괴

### 게임 규칙

**기본 플레이**
- 하늘에서 여러 건물이 동시에 낙하합니다
- 플레이어는 좌우로 이동하며 건물 아래에 위치해야 합니다
- Z키로 공격하여 건물의 **최하단 층부터** 순차적으로 파괴합니다
- 각 층은 **HP를 가지고 있어** 여러 번 공격해야 파괴됩니다

**생존 메커니즘**
- 건물이 플레이어 머리 위로 떨어지면 **↓ 방향키로 방어**해야 합니다
- 방어 성공 시 건물이 위로 튕겨 올라갑니다
- 방어 실패 시 **라이프가 1 감소**하며, 라이프가 0이 되면 게임 오버입니다

**콤보 시스템**
- 연속으로 공격에 성공하면 **콤보가 증가**합니다
- 피격 당하면 콤보가 리셋됩니다
- 높은 콤보는 더 많은 점수를 제공합니다

**필살기**
- 공격 성공 시 화면 하단의 **게이지가 충전**됩니다
- 게이지가 100%가 되면 X키로 필살기 발동 가능
- 필살기는 **화면의 모든 건물을 한 번에 파괴**합니다

### 원작: 키루비루(斬る·ビル)

키루비루는 일본의 미니게임으로, 영화 '킬 빌(Kill Bill)'을 패러디한 제목이며, FREE GAME AWARDS에서 준그랑프리를 수상한 작품입니다. 한국에서는 '건물부수기', '집부수기'로 알려져 있으며, 2000년대 초중반 컴퓨터실에서 즐겨 플레이되던 추억의 플래시 게임입니다.

---

## 🎨 주요 기능

### 게임 시스템

✅ **콤보 시스템**: 연속 공격 성공 시 콤보 증가, 피격 시 리셋  
✅ **필살기 게이지**: 공격 성공 시 충전, 100% 달성 시 X키로 발동  
✅ **난이도 증가**: 시간 경과에 따라 건물 낙하 속도 및 HP 상승  
✅ **최고 점수**: 파일 기반 영구 저장

### 플레이어 액션

- **이동**: ← → 방향키
- **점프**: ↑ 방향키
- **공격**: Z키 (건물 최하단 층 파괴)
- **방어**: ↓ 방향키 (떨어지는 건물 튕겨내기)
- **필살기**: X키 (게이지 100% 시, 전체 건물 파괴)

### 테마: 사무라이 도장

- 🏯 **일본 전통 건축물**: 기와지붕, 목재 층
- 🥋 **사무라이 캐릭터**: 10프레임 스프라이트 애니메이션
- 🎨 **레트로 픽셀 아트**: 800x600 해상도의 고전 게임 감성
- 🎵 **테마 BGM**: 일본풍 배경음악 및 효과음

---

## 🎯 오픈미션: 낯선 도구 해커톤

### 미션 선정

**"콘솔 기반 C++ 게임 제작 → SDL2 그래픽 게임 전환"**

우아한테크코스 오픈미션은 자유 주제로 스스로 도전 목표를 설정하는 과제입니다.

평소 Java/Spring Boot로 백엔드 개발을 하던 저에게 C++과 게임 개발은 완전히 새로운 영역이었습니다. 본 프로젝트는 오픈미션의 **"낯선 도구 해커톤"** 방향으로, 익숙하지 않은 기술 스택을 활용해 동작하는 결과물을 만드는 것을 목표로 했습니다.

### 개발 전략: 콘솔 → SDL2 2단계 접근

**왜 처음부터 SDL2로 시작하지 않았나?**

SDL2는 강력하지만 학습 곡선이 있는 라이브러리입니다. C++도 낯선 상황에서 SDL2까지 동시에 학습하면 **핵심인 게임 로직 구현에 집중하기 어렵다**고 판단했습니다.

따라서 다음과 같은 **단계적 전략**을 수립했습니다:

**Phase 1 (1주차): 콘솔 프로토타입**
- ✅ 빠른 피드백: 출력 결과를 즉시 확인 가능
- ✅ 로직 검증: 물리, 충돌, 게임플레이에만 집중
- ✅ 디버깅 용이: `cout`으로 간단한 디버깅
- ✅ 낮은 진입장벽: 콘솔 I/O만 알면 시작 가능

**Phase 2 (2주차): SDL2 그래픽 전환**
- ✅ 검증된 로직: 이미 동작하는 게임 로직 재사용
- ✅ 시각화 집중: 렌더링, 애니메이션, 사운드에만 집중
- ✅ 아키텍처 검증: 플랫폼 독립성 실전 테스트

이 전략은 결과적으로 SDL2 학습 시간을 크게 단축시켰습니다.

### C++을 선택한 이유

**낯설지만, 적용 가능한 언어**

C++은 PS(Problem Solving) 문제 풀이에서는 자주 사용했지만, **실제 서비스나 완성된 프로그램을 만들어본 적은 없었습니다.** 메모리 관리, 포인터, 헤더 파일 분리 등 실전 개발 경험이 전무한 상태였기에 충분히 "낯선 도구"의 조건을 만족했습니다.

그럼에도 C++을 선택한 이유는:

1. **객체지향 언어**: 프리코스 1~3주차에서 학습한 객체 간 협력, 책임 분리, 인터페이스 설계 등을 직접 적용 가능
2. **게임 개발 적합성**: 네이티브 성능과 SDL2 같은 검증된 라이브러리 생태계 존재
3. **학습 가치**: 저수준 메모리 관리와 시스템 프로그래밍 경험을 통한 성장 기회
4. **제어 가능한 난이도**: 완전히 새로운 언어(Rust, Go)보다는 기존 지식을 일부 활용 가능

### 게임 개발을 선택한 이유

**객체를 표현하기에 가장 좋은 분야**

게임은 **객체지향 설계를 적용하기에 이상적인 도메인**입니다:

- **명확한 객체**: Player, Building, Floor 등 현실 세계의 개체가 명확히 대응
- **역할과 책임**: 각 객체가 수행해야 할 행동이 직관적으로 정의됨
- **실시간 상호작용**: 객체 간 협력과 메시지 전달이 매 프레임 발생
- **즉각적인 피드백**: 설계의 문제점이 실행 시 바로 드러남

또한, **어릴 적부터 게임을 만들어보고 싶었던 개인적인 동기**도 있었습니다. 플래시 게임부터 RPG 쯔꾸르까지 다양한 시도를 해왔지만, 제대로 된 프로그래밍 언어로 완성도 있는 게임을 만들어본 경험은 없었습니다. 이번 기회에 **동작하는 결과물**을 목표로 진지하게 게임 개발에 도전해보고자 했습니다.

### 왜 건물부수기(키루비루)를 리메이크했나?

**선정 기준**

원작 게임을 선택한 이유는 다음과 같습니다:

**1. 명확한 게임 구조**

키루비루는 게임 메커니즘이 매우 명확합니다:
- **입력**: 방향키(이동/점프/방어), Z(공격), X(필살기)
- **로직**: 중력, 충돌 판정, 층별 HP 관리
- **출력**: 캐릭터/건물 렌더링, 점수/콤보 표시

이러한 명확성 덕분에 **2주라는 제한된 기간 내에 MVP → 기능 확장 → 완성도 향상**이라는 단계적 개발이 가능했습니다.

**2. 적절한 기술적 난이도**

단순해 보이지만 실제로는 다음과 같은 기술적 도전 과제를 포함합니다:
- 물리 엔진 구현 (중력, 점프, 충돌)
- 실시간 상태 관리 (부착, 공격, 방어)
- 렌더링 최적화 (60 FPS 유지)
- 사운드 동기화

**3. 추억과 개인적 의미**

어릴 적 컴퓨터실에서 즐겨 하던 게임을 직접 구현한다는 것은 단순한 과제 이상의 의미가 있었습니다. 당시엔 "어떻게 만들었을까?" 궁금했던 게임을, 이제는 직접 만들 수 있다는 점에서 큰 동기부여가 되었습니다.

**리메이크 방향**

원작을 그대로 재현하기보다는, **현대적인 해석**과 **기술적 제약을 고려한 조정**을 가했습니다:

| 원작 | 리메이크 | 이유 |
|------|----------|------|
| 건물 1개씩 순차적으로 등장 | **여러 건물 동시 낙하** | 난이도 조절 및 화면 구성의 풍부함 |
| 점프 시 화면 스크롤 | **고정 화면 내 플레이** | 구현 복잡도 감소, 게임플레이 집중 |
| 긴 플레이 타임 (제1막/제2막) | **짧은 단판 게임** | 빠른 플레이 사이클, 접근성 향상 |
| 단순 픽셀 그래픽 | **사무라이 도장 테마** | 차별화된 비주얼 아이덴티티 |

---

## 🏗️ 기술 스택

### Core
- **Language**: C++17
- **Build System**: CMake 3.20+
- **Compiler**: MinGW-w64 (GCC 11.4.0)

### Graphics & Media
- **Rendering**: SDL2 2.0.20+
- **Image Loading**: SDL2_image
- **Font Rendering**: SDL2_ttf
- **Audio**: SDL2_mixer

### Development Tools
- **IDE**: CLion 2024
- **Version Control**: Git/GitHub
- **Asset Creation**: Aseprite (sprites), Audacity (audio)

---

## 📂 프로젝트 구조

```
src/
├── main.cpp
├── core/                      # 게임 로직 (플랫폼 독립적)
│   ├── building/
│   │   ├── Building.cpp
│   │   ├── BuildingManager.cpp
│   │   └── Floor.cpp
│   ├── game/
│   │   ├── Game.cpp          # 메인 게임 루프
│   │   ├── GameConfig.h
│   │   ├── GameOverDisplayData.h
│   │   ├── GameSession.cpp   # 게임 상태 관리
│   │   ├── GameStats.cpp     # 점수/콤보/게이지
│   │   └── ScoreManager.cpp
│   ├── player/
│   │   ├── CollisionResult.h
│   │   ├── Player.cpp
│   │   ├── PlayerAction.cpp
│   │   ├── PlayerActionType.h
│   │   ├── PlayerAttachment.cpp
│   │   ├── PlayerCollision.cpp
│   │   └── PlayerMovement.cpp
│   └── ui/                    # UI 메시지 시스템
│       ├── UIMessage.cpp
│       └── UIStrings.h
├── interfaces/                # 추상화 계층
│   ├── IInputHandler.h
│   ├── InputKey.h
│   └── IRenderer.h
└── platform/
├── console/               # 콘솔 구현체
│   ├── ConsoleRenderer.cpp
│   └── ConsoleInputHandler.cpp
└── sdl/                   # SDL2 구현체
├── base/
│   ├── SDLRenderer.cpp
│   └── SDLInputHandler.cpp
├── rendering/
│   ├── BuildingRenderer.cpp
│   ├── GameOverRenderer.cpp
│   ├── GameRenderer.cpp
│   ├── HUDRenderer.cpp
│   ├── MenuRenderer.cpp
│   ├── PlayerAnimationRenderer.cpp
│   └── UltimateEffectRenderer.cpp
└── assets/
├── AssetConfig.h
├── AssetManager.cpp
└── SoundManager.cpp
```

---

## 🚀 실행 방법

### 📌 공통 요구 사항
- CMake 3.20+
- C++17 지원 컴파일러

---

## 🎮 빠른 실행 (Windows 전용)

빌드 없이 바로 실행하고 싶다면 사전 빌드된 실행 파일을 다운로드하세요:

**[📥 Windows 실행 파일 다운로드 (Google Drive)](https://drive.google.com/file/d/1EWaQFP0YEofXgY05gvCaGDU-92jE7qH6/view?usp=sharing)**

1. 압축 파일 다운로드 및 압축 해제
2. `open_mission_building_breaker.exe` 실행

> ⚠️ **주의**: Windows Defender에서 경고가 나올 수 있습니다. "추가 정보" → "실행"을 클릭하세요.

---

## 🪟 Windows 빌드 & 실행

### 요구 사항
- Windows 10 이상
- MinGW-w64 (GCC) 또는 MSVC 2019+
- SDL2 개발 라이브러리 (`libs/` 폴더에 포함)

### 빌드

```bash
# 1. 저장소 클론
git clone https://github.com/StoneCAU/open-mission-building-breaker.git
cd open-mission-building-breaker

# 2. 빌드 디렉토리 생성
mkdir build && cd build

# 3. SDL2 그래픽 버전 빌드
cmake .. -DUSE_SDL=ON
cmake --build . --config Release

# 4. 실행
.\Release\open_mission_building_breaker.exe
```

### 콘솔 버전

```bash
cmake .. -DUSE_SDL=OFF
cmake --build .
.\Debug\open_mission_building_breaker.exe
```

---

## 🍎 macOS 빌드 & 실행

### 요구 사항
- macOS 12 Monterey 이상 (Intel / Apple Silicon)
- Xcode Command Line Tools
- SDL2 설치 (Homebrew 권장)

### SDL2 설치

```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### 빌드

```bash
# 1. 저장소 클론
git clone https://github.com/StoneCAU/open-mission-building-breaker.git
cd open-mission-building-breaker

# 2. 빌드 디렉토리 생성
mkdir build && cd build

# 3. SDL2 그래픽 버전 빌드
cmake .. -DUSE_SDL=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### 실행

```bash
./open_mission_building_breaker
```

### 콘솔 버전

```bash
cmake .. -DUSE_SDL=OFF
cmake --build .
./open_mission_building_breaker
```

### SDL2 경로 인식 실패 시 (Homebrew 경로 수동 지정)

**Apple Silicon (M1/M2/M3):**

```bash
cmake .. \
  -DUSE_SDL=ON \
  -DSDL2_DIR=/opt/homebrew/opt/sdl2/lib/cmake/SDL2
```

**Intel Mac:**

```bash
cmake .. \
  -DUSE_SDL=ON \
  -DSDL2_DIR=/usr/local/opt/sdl2/lib/cmake/SDL2
```

---

## 🎓 개발 성과

### 핵심 성과

이 프로젝트를 통해 달성한 가장 큰 성과는 **플랫폼 독립적 게임 로직 설계**입니다.

콘솔 기반으로 개발한 게임 로직을, **단 한 줄의 수정 없이** SDL2 그래픽 환경으로 전환할 수 있었습니다. 이는 단순히 인터페이스를 분리했다는 기술적 성취를 넘어, **좋은 아키텍처가 실제로 시간을 절약한다**는 것을 몸소 체험한 경험이었습니다.

### 기술적 학습

- ✅ C++ 실전 개발 경험 (스마트 포인터, RAII, 컴파일 타임 최적화)
- ✅ 게임 개발 기초 (물리 엔진, 충돌 처리, 상태 기계)
- ✅ Clean Architecture 실전 적용
- ✅ SDL2 라이브러리 활용