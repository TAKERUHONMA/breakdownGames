#include "Trump.h"
#include <assert.h>
#include <algorithm>

Trump::Trump(GameObject* parent)
    : gHandle(LoadGraph("Assets/trump.png")), selectedCard1(-1), selectedCard2(-1), flipTimer(0)
{
    counter = 0;

    // カードの初期化
    for (int n = 1; n <= 13; n++) 
    {
        for (int m = 0; m <= 3; m++) 
        { // 4マークだけに変更
            Card c;
            c.num = n;
            c.mark = m;
            c.isFaceUp = false;
            c.isMatched = false;
            cards.push_back(c);
        }
    }

    // カードをシャッフル
    std::random_shuffle(cards.begin(), cards.end());

    assert(gHandle >= 0);
}

void Trump::Update()
{
    // キーが押されたかどうかを管理するフラグ
    static bool isKeyPressedRight = false;
    static bool isKeyPressedLeft = false;
    static bool isKeyPressedup = false;
    static bool isKeyPresseddown = false;

    // →キーが押されたとき
    if (CheckHitKey(KEY_INPUT_RIGHT)) 
    {
        if (!isKeyPressedRight) 
        {
            // キーが押されていなかった場合、カードを1つ進める
            if (selectedCard1 < (int)cards.size() - 1) 
            {
                selectedCard1++;
            }
            isKeyPressedRight = true;  // フラグを立てる
        }
    }
    else 
    {
        // →キーが離されたらフラグを戻す
        isKeyPressedRight = false;
    }

    // ↑キーが押されたとき
    if (CheckHitKey(KEY_INPUT_LEFT)) 
    {
        if (!isKeyPressedLeft) 
        {
            // キーが押されていなかった場合、カードを1つ戻す
            if (selectedCard1 > 0) 
            {
                selectedCard1--;
            }
            isKeyPressedLeft = true;  // フラグを立てる
        }
    }
    if (CheckHitKey(KEY_INPUT_UP)) 
    {
        if (!isKeyPressedup) 
        {
            // キーが押されていなかった場合、カードを1つ進める
            if (selectedCard1 < (int)cards.size() - 1) 
            {
                selectedCard1++;
            }
            isKeyPressedup = true;  // フラグを立てる
        }
    }
    else 
    {
        // ↑キーが離されたらフラグを戻す
        isKeyPressedup = false;
    }

    // ↓キーが押されたとき
    if (CheckHitKey(KEY_INPUT_DOWN)) 
    {
        if (!isKeyPresseddown) 
        {
            // キーが押されていなかった場合、カードを1つ戻す
            if (selectedCard1 > 0) 
            {
                selectedCard1--;
            }
            isKeyPresseddown = true;  // フラグを立てる
        }
    }
    else 
    {
        // ↓キーが離されたらフラグを戻す
        isKeyPresseddown = false;
    }

    // 選択されたカードをめくる
    if (CheckHitKey(KEY_INPUT_RETURN) && selectedCard1 >= 0 && selectedCard1 < (int)cards.size()) 
    {
        if (!cards[selectedCard1].isFaceUp && !cards[selectedCard1].isMatched) 
        {
            cards[selectedCard1].isFaceUp = true;

            if (selectedCard2 == -1) 
            {
                selectedCard2 = selectedCard1;
            }
            else 
            {
                flipTimer = 60; // 一定時間後に裏返す
            }
        }
    }

    // マッチ判定
    if (flipTimer > 0) 
    {
        flipTimer--;
        if (flipTimer == 0) 
        {
            if (cards[selectedCard1].num == cards[selectedCard2].num )//&&
            {// cards[selectedCard1].mark == cards[selectedCard2].mark) {
                cards[selectedCard1].isMatched = true;
                cards[selectedCard2].isMatched = true;
            }
            else 
            {
                cards[selectedCard1].isFaceUp = false;
                cards[selectedCard2].isFaceUp = false;
            }
            selectedCard1 = -1;
            selectedCard2 = -1;
        }
    }
}

void Trump::Draw()
{
    for (int i = 0; i < (int)cards.size(); i++) 
    {
        int x = (i % 12) * 60; // 1列に8枚配置
        int y = (i / 12) * 100;

        // 選択中のカード枠
        if (i == selectedCard1)
        {
            DrawBox(x - 2, y - 2, x + 58, y + 82, GetColor(0, 255, 0), TRUE);
        }

        if (cards[i].isMatched) 
        {
            continue; // そろったカードは表示しない
        }

        if (cards[i].isFaceUp) 
        {
            // 表向き
            DrawRectGraph(x, y, cards[i].num * 56, cards[i].mark * 80, 56, 80, gHandle, TRUE);
        }
        else 
        {
            // 裏向き
            //DrawBox(x, y, x + 56, y + 80, GetColor(0, 0, 255), TRUE);
            DrawRectGraph(x, y, cards[i].num * 0, cards[i].mark * 0, 56, 80, gHandle, TRUE);
        }
    }
}