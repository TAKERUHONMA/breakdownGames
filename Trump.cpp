#include "Trump.h"
#include <assert.h>
#include <algorithm>

Trump::Trump(GameObject* parent)
    : gHandle(LoadGraph("Assets/trump.png")), selectedCard1(-1), selectedCard2(-1), flipTimer(0)
{
    counter = 0;

    // �J�[�h�̏�����
    for (int n = 1; n <= 13; n++) 
    {
        for (int m = 0; m <= 3; m++) 
        { // 4�}�[�N�����ɕύX
            Card c;
            c.num = n;
            c.mark = m;
            c.isFaceUp = false;
            c.isMatched = false;
            cards.push_back(c);
        }
    }

    // �J�[�h���V���b�t��
    std::random_shuffle(cards.begin(), cards.end());

    assert(gHandle >= 0);
}

void Trump::Update()
{
    // �L�[�������ꂽ���ǂ������Ǘ�����t���O
    static bool isKeyPressedRight = false;
    static bool isKeyPressedLeft = false;
    static bool isKeyPressedup = false;
    static bool isKeyPresseddown = false;

    // ���L�[�������ꂽ�Ƃ�
    if (CheckHitKey(KEY_INPUT_RIGHT)) 
    {
        if (!isKeyPressedRight) 
        {
            // �L�[��������Ă��Ȃ������ꍇ�A�J�[�h��1�i�߂�
            if (selectedCard1 < (int)cards.size() - 1) 
            {
                selectedCard1++;
            }
            isKeyPressedRight = true;  // �t���O�𗧂Ă�
        }
    }
    else 
    {
        // ���L�[�������ꂽ��t���O��߂�
        isKeyPressedRight = false;
    }

    // ���L�[�������ꂽ�Ƃ�
    if (CheckHitKey(KEY_INPUT_LEFT)) 
    {
        if (!isKeyPressedLeft) 
        {
            // �L�[��������Ă��Ȃ������ꍇ�A�J�[�h��1�߂�
            if (selectedCard1 > 0) 
            {
                selectedCard1--;
            }
            isKeyPressedLeft = true;  // �t���O�𗧂Ă�
        }
    }
    if (CheckHitKey(KEY_INPUT_UP)) 
    {
        if (!isKeyPressedup) 
        {
            // �L�[��������Ă��Ȃ������ꍇ�A�J�[�h��1�i�߂�
            if (selectedCard1 < (int)cards.size() - 1) 
            {
                selectedCard1++;
            }
            isKeyPressedup = true;  // �t���O�𗧂Ă�
        }
    }
    else 
    {
        // ���L�[�������ꂽ��t���O��߂�
        isKeyPressedup = false;
    }

    // ���L�[�������ꂽ�Ƃ�
    if (CheckHitKey(KEY_INPUT_DOWN)) 
    {
        if (!isKeyPresseddown) 
        {
            // �L�[��������Ă��Ȃ������ꍇ�A�J�[�h��1�߂�
            if (selectedCard1 > 0) 
            {
                selectedCard1--;
            }
            isKeyPresseddown = true;  // �t���O�𗧂Ă�
        }
    }
    else 
    {
        // ���L�[�������ꂽ��t���O��߂�
        isKeyPresseddown = false;
    }

    // �I�����ꂽ�J�[�h���߂���
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
                flipTimer = 60; // ��莞�Ԍ�ɗ��Ԃ�
            }
        }
    }

    // �}�b�`����
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
        int x = (i % 12) * 60; // 1���8���z�u
        int y = (i / 12) * 100;

        // �I�𒆂̃J�[�h�g
        if (i == selectedCard1)
        {
            DrawBox(x - 2, y - 2, x + 58, y + 82, GetColor(0, 255, 0), TRUE);
        }

        if (cards[i].isMatched) 
        {
            continue; // ��������J�[�h�͕\�����Ȃ�
        }

        if (cards[i].isFaceUp) 
        {
            // �\����
            DrawRectGraph(x, y, cards[i].num * 56, cards[i].mark * 80, 56, 80, gHandle, TRUE);
        }
        else 
        {
            // ������
            //DrawBox(x, y, x + 56, y + 80, GetColor(0, 0, 255), TRUE);
            DrawRectGraph(x, y, cards[i].num * 0, cards[i].mark * 0, 56, 80, gHandle, TRUE);
        }
    }
}