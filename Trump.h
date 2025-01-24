#pragma once
#include "Engine/GameObject.h"
#include <vector>

using std::vector;

class Trump : public GameObject
{
    int gHandle;                 // �g�����v�摜�n���h��
    int counter;                 // �^�C�}�[
    int selectedCard1;           // �I�𒆂�1���ڂ̃J�[�h
    int selectedCard2;           // �I�𒆂�2���ڂ̃J�[�h
    int flipTimer;               // ���Ԃ��܂ł̃^�C�}�[

    struct Card
    {
        int num;                 // �J�[�h�̔ԍ��i1�`13�j
        int mark;                // �J�[�h�̃}�[�N�i0�`3�j
        float x, y;              // �`��ʒu
        bool isFaceUp;           // �\������
        bool isMatched;          // �}�b�`�ς݂�
    };

    vector<Card> cards;          // �Q�[���S�̂̃J�[�h

public:
    Trump(GameObject* parent);
    ~Trump() {};
    void Update() override;      // �Q�[�����W�b�N�̍X�V
    void Draw() override;        // �J�[�h�̕`��
};