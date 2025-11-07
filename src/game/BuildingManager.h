#pragma once
#include <vector>
#include "Building.h"

class BuildingManager {
public:
    BuildingManager();

    // ===== 초기화 및 업데이트 =====
    void initBuildings();     // 최초 빌딩 랜덤 생성
    void updateAll();         // 모든 빌딩 상태 갱신 (하강, 제거, 보충)

    // ===== 데이터 접근 =====
    std::vector<Building>& getAll(); // 전체 빌딩 참조 반환

private:
    std::vector<Building> buildings;

    // ===== 내부 헬퍼 =====
    void addRandomBuilding();     // 새 빌딩 하나 추가
    bool isOverlapping(int newX) const; // X 좌표 겹침 검사
};
