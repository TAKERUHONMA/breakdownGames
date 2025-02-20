#pragma once
#include "Engine/GameObject.h"
#include <vector>

using std::vector;

class Trump : public GameObject
{
    int count;                   //クールタイム
    int gHandle;                 // トランプ画像ハンドル
    int counter;                 // タイマー
    int selectedCard1;           // 選択中の1枚目のカード
    int selectedCard2;           // 選択中の2枚目のカード
    int flipTimer;               // 裏返しまでのタイマー
    int flipCooldown = 0;        // クールタイムのカウントダウン

    struct Card
    {
        int num;                 // カードの番号（1～13）
        int mark;                // カードのマーク（0～3）
        float x, y;              // 描画位置
        bool isFaceUp;           // 表向きか
        bool isMatched;          // マッチ済みか
    };

    vector<Card> cards;          // ゲーム全体のカード

public:
    Trump(GameObject* parent);
    ~Trump() {};

    
    void Initialize() override;  //初期化
    void Update() override;      // ゲームロジックの更新
    void Draw() override;        // カードの描画
    void TrumpCount(int _count);           //トランプをカウントする
};