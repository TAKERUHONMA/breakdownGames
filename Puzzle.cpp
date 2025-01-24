#include "Puzzle.h"
#include <assert.h>

static const int R = 20;
static const int OFF_X = 500;
static const int OFF_Y = 200;
static const int WIDTH= 6;
static const int HEIGHT = 5;
static const unsigned int COLORS[] ={
GetColor(255,102,102), //red
GetColor(153,204,153), //green
GetColor(102,153,255), //blue
GetColor(255,255,104), //yellow
GetColor(255,204,255), //pink
};
static const int ColorNum = sizeof(COLORS) / sizeof(COLORS[0]);

Puzzle::Puzzle(GameObject* parent)
{
	selX = -1;
	selY = -1;
	mouseX = 0;
	mouseY = 0;
	state = S_IDLE;
	lastButton = false;
	eraseTimer = 0;
	eraseAlpha = 0;
	soundErase = LoadSoundMem("Assets/erase.mp3");
	assert(soundErase > 0);
	soundFall = LoadSoundMem("Assets/fall.mp3");
	assert(soundFall > 0);
	fallSoundRequested = false;

	for (int y = 0; y < HEIGHT; y++)
	{
		std::vector<Piece> tmp;
		for (int x = 0; x <WIDTH; x++)
		{
			Piece p;
			p.color = rand() % ColorNum;
			p.erase = 0;
			p.fallAdjust = -1;
			p.groupNum =0;
			tmp.push_back(p);
		}
		field.push_back(tmp);
	}
	
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			field[y][x].erase = 0;
			field[y][x].groupNum = 0;
		}
	}
}

Puzzle::~Puzzle()
{
	if (soundErase > 0)
	{
		DeleteSoundMem(soundErase);
	}
	if (soundFall > 0)
	{
		DeleteSoundMem(soundFall);
	}
}

void Puzzle::Update()
{
	switch (state)
	{
	case S_IDLE:
		UpdateIdle();
		break;
	case S_MOVE:
		UpdateMove();
		break;
	case S_ERASE:
		UpdateErase();
		break;
	case S_FALL:
		UpdateFall();
		break;
	case S_ATTACK:
		UpdateAttack();
		break;
	default:
		assert(false);
		break;
	}
}

void Puzzle::Draw()
{
	for (int y = 0; y <HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int xpos = OFF_X + x * R * 2;
			int ypos = OFF_Y + y * R * 2+field[y][x].fallAdjust;
			bool alphaSet = false;

			if (x == selX && y == selY)
			{
				alphaSet = true;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			}
			if (state == S_ERASE && field[y][x].erase>0)
			{
				alphaSet = true;
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, eraseAlpha);
			}
			int r = field[y][x].color;
			DrawCircle(xpos,ypos, R, COLORS[r]);
			if (alphaSet)
			{
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			/*DrawFormatString(xpos,ypos, GetColor(255, 255, 255), "%d", field[y][x].erase);
			
			if (field[y][x].groupNum != 0&&field[y][x].groupNum != -1)
			{
				DrawFormatString(xpos,ypos+200 + y * R * 2, GetColor(255, 255, 255), "%d", field[y][x].groupNum);
			}*/
		}
	}
}

void Puzzle::UpdateIdle()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		if (lastButton == false) 
		{
			GetMousePoint(&mouseX, &mouseY);
			for (int y = 0; y < HEIGHT; y++)
			{
				for (int x = 0; x <WIDTH; x++)
				{
					int xpos = OFF_X + x * R * 2;
					int ypos = OFF_Y + y * R * 2;
					if ((mouseX >= xpos - R && mouseX <= xpos + R) &&(mouseY >= ypos - R && mouseY <= ypos+ R))
					{
						selX = x;
						selY = y;
						state = S_MOVE;
					}
				}
			}
		}
		lastButton = true;
	}
	else
	{
		lastButton = false;
	}
}

void Puzzle::UpdateMove()
{
	GetMousePoint(&mouseX, &mouseY);
	int newX=-1;
	int newY=-1;
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int xpos = OFF_X + x * R * 2;
			int ypos = OFF_Y + y * R * 2;
			if ((mouseX >= xpos - R && mouseX <= xpos + R) &&(mouseY >=ypos- R && mouseY <= ypos + R))
			{
				newX = x;
				newY = y;
			}
		}
	}
	if (newX>=0&&(newX != selX || newY != selY))
	{
		std::swap(field[selY][selX],field[newY][newX]);
		selX = newX;
		selY = newY;
	}
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0)
	{
		selX = -1;
		if (CanErase())
		{
			eraseTimer = 0;
			state = S_ERASE;
			PlaySoundMem(soundErase, DX_PLAYTYPE_BACK);
		}

		else
		{
			state = S_IDLE;
		}
	}
}

void Puzzle::UpdateErase()
{
	int a = 255;
	eraseTimer += 1.0f / 60.0f;
	eraseAlpha = a - a * eraseTimer / 0.1f;
	if (eraseAlpha < 0.0f)
	{
		eraseAlpha = 0.0f;
	}
	if (eraseTimer >= 1.0f)
	{
		PrepareFall();
		state = S_FALL;
		fallSoundRequested = false;
	}
}

void Puzzle::UpdateFall()
{
	bool fallAll = true;
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			Piece& c = field[y][x];
			if (c.fallAdjust < 0)
			{
				fallAll = false;
				int b = 1;
				c.fallAdjust+=5.0f*b;
				b += 0.2;
				if (c.fallAdjust > 0)
				{
					c.fallAdjust = 0;
				}
			}
		}
	}

	if (fallAll)
	{
		if (fallSoundRequested == false)
		{
			PlaySoundMem(soundFall, DX_PLAYTYPE_BACK);
			fallSoundRequested = true;
		}
		else
		{
			if (CheckSoundMem(soundFall) == 0)
			{
				if (CanErase())
				{
					eraseTimer = 0;
					state = S_ERASE;
					PlaySoundMem(soundErase, DX_PLAYTYPE_BACK);
				}
				else
				{
					state = S_IDLE;
				}
			}
		}
	}
}

void Puzzle::UpdateAttack()
{
}

bool Puzzle::CanErase()
{
	bool a=false;
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			field[y][x].erase = 0;
			field[y][x].groupNum = 0;
		}
	}

	int num = 1;
	for (int y = 0; y <HEIGHT; y++)
	{
		for (int x = 1; x <WIDTH-1; x++)
		{
			if (field[y][x - 1].color == field[y][x].color && field[y][x].color == field[y][x + 1].color)
			{
				
				field[y][x - 1].erase = 1;
				field[y][x].erase = 1;
				field[y][x + 1].erase = 1;

				if (field[y][x-1].groupNum != -1)
				field[y][x - 1].groupNum = num;
				num++;
				field[y][x].groupNum =-1;
				field[y][x + 1].groupNum =-1;
				a = true;
			}
		}
	}

	for (int y = 1; y < HEIGHT-1; y++)
	{
		for (int x=0; x < WIDTH; x++)
		{
			if (field[y - 1][x].color == field[y][x].color && field[y][x].color == field[y + 1][x].color)
			{
				field[y - 1][x].erase = 1;
				field[y][x].erase = 1;
				field[y + 1][x] .erase = 1;

				if (field[y - 1][x].groupNum != -1)
				{
					field[y - 1][x].groupNum = num;
					num++;
				}
				field[y][x].groupNum = -1;
				field[y+1][x].groupNum = -1;
				a = true;
			}
		}
	}

	if (a)
	{
		return true;
	}
	
	else
	{
		return false;
	}
}

void Puzzle::PrepareFall()
{
	for (int x = 0; x < WIDTH; x++)
	{
		int erased = 0;
		for (int y = HEIGHT - 1; y >= 0; y--)
		{
			if (field[y][x].erase == 1)
			{
				erased++;
			}
			else
			{
				field[y + erased][x] = field[y][x];
				field[y + erased][x].fallAdjust = -R * 2*erased;
			}
		}

		for (int y = 0; y < erased; y++)
		{
			field[y][x].color = rand() % ColorNum;
			field[y][x].erase = 0;
			field[y][x].fallAdjust = -R * 2*erased;
		}
	}
}
