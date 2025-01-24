#pragma once
#include "Engine/GameObject.h"
#include <vector>

/// <summary>
/// パズドラのプレイ画面
/// </summary>


class Puzzle:
    public GameObject
{
public:
    Puzzle(GameObject* parent);
    ~Puzzle();
    void Update() override;
    void Draw() override;
private:
    struct Piece
    {
        int color;
        int erase;
        int groupNum;
        float fallAdjust;
    };
   std::vector<std::vector<Piece>> field;
    int selX, selY;
    int mouseX, mouseY;
    bool lastButton;
    float eraseTimer;
    int eraseAlpha;
    int soundErase;
    int soundFall;
    bool fallSoundRequested;
    enum State
    {
        S_IDLE, S_MOVE, S_ERASE, S_FALL, S_ATTACK
    };
    State state;
    void UpdateIdle();
    void UpdateMove();
    void UpdateErase();
    void UpdateFall();
    void UpdateAttack();
    bool CanErase();
    void PrepareFall();
};
