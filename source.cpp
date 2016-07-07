#include "DxLib.h"

#define BALL_SPEED 5
#define BALL_SIZE 5
#define BAR_SPEED 5
#define BAR_WIDTH 50
#define BAR_HEIGHT 10
#define BLOCK_WIDTH 50
#define BLOCK_HEIGHT 31
#define MAX_BLOCKS 144
#define BLOCK_MAP_INDEX_X 12
#define BLOCK_MAP_INDEX_Y 12
#define FIELD_WIDTH 300
#define FIELD_HEIGHT 480
#define X_POSI 170

void visual(void);
void block_cr(void);

void my_move_ball(void);
void my_move_bar(void);
void my_cd_check(void);
void my_game_over_check(void);
void my_draw_field(void);
void my_draw_op_ed(void);

int my_get_key(void);

struct BLOCKS{
	int x;
	int y;
	int flag;
	int color;
};

struct BLOCKS blocks[MAX_BLOCKS];

int block_map[BLOCK_MAP_INDEX_Y][BLOCK_MAP_INDEX_X] = {
	{ 1, 2, 3, 1, 2, 3 },
	{ 2, 3, 1, 2, 3, 1 },
	{ 3, 1, 2, 3, 1, 2 },
	{ 1, 2, 3, 1, 2, 3 },
	{ 2, 3, 1, 2, 3, 1 },
	{ 3, 1, 2, 3, 1, 2 },
	{ 1, 2, 3, 1, 2, 3 },
	{ 2, 3, 1, 2, 3, 1 },
	{ 3, 1, 2, 3, 1, 2 },
	{ 1, 2, 3, 1, 2, 3 }

};

int bar_x;
int bar_y;
int ball_x;
int ball_y;
int ball_start_flag;
int x_direction;
int y_direction;
int score;
int key[256];
int game_over_flag;

int Color_White;
int Color_Black;
int Color_Green1;
int Color_Green2;
int Color_Green3;

int back_img;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	visual();
	block_cr();

	while (ScreenFlip() == 0 && ProcessMessage() == 0
		&& ClearDrawScreen() == 0 && my_get_key() == 0){

		my_move_ball();
		my_move_bar();
		my_cd_check();
		my_game_over_check();
		my_draw_field();
		my_draw_op_ed();

	}
	DxLib_End();
	return 0;
}

void visual(){
	bar_x = 0;
	bar_y = FIELD_HEIGHT - BAR_HEIGHT;
	ball_x = BAR_WIDTH / 2;
	ball_y = (FIELD_HEIGHT - BAR_HEIGHT) - BALL_SIZE;
	ball_start_flag = 0;
	x_direction = 0;
	y_direction = 0;
	score = 0;
	game_over_flag = 0;
	Color_White = GetColor(255, 255, 255);
	Color_Black = GetColor(0, 0, 0);
	Color_Green1 = GetColor(255, 0, 0);
	Color_Green2 = GetColor(0, 255, 0);
	Color_Green3 = GetColor(0, 0, 255);
	back_img = LoadGraph("img3.jpg");
}

void block_cr(){
	int i, j;
	int count = 0;
	for (i = 0; i < BLOCK_MAP_INDEX_Y; i++){
		for (j = 0; j < BLOCK_MAP_INDEX_X; j++){
			if (block_map[i][j] != 0){
				blocks[count].x = (BLOCK_WIDTH * j);
				blocks[count].y = (BLOCK_HEIGHT * i);
				blocks[count].flag = 1;
				switch (block_map[i][j]){
				case 1:
					blocks[count].color = Color_Green1;
					break;
				case 2:
					blocks[count].color = Color_Green2;
					break;
				case 3:
					blocks[count].color = Color_Green3;
					break;
				default:
					break;
				}
			}
			count++;
		}
	}
}

void my_move_ball(){
	if (ball_start_flag != 0){
		if (x_direction % 2 == 0){
			ball_x += BALL_SPEED;
		}
		else{
			ball_x -= BALL_SPEED;
		}

		if (y_direction % 2 == 0){
			ball_y += BALL_SPEED;
		}
		else{
			ball_y -= BALL_SPEED;
		}
	}
}

void my_move_bar(){
	if (key[KEY_INPUT_RETURN] > 0){
		ball_start_flag = 1;
	}
	if (ball_start_flag != 0){
		if (key[KEY_INPUT_RIGHT] > 0 && (bar_x + BAR_WIDTH) < FIELD_WIDTH){
			bar_x += BAR_SPEED;
		}
		if (key[KEY_INPUT_LEFT] > 0 && bar_x > 0){
			bar_x -= BAR_SPEED;
		}
	}
}

void my_cd_check(){
	int count;
	int score_flag = 0;

	if (ball_x >= FIELD_WIDTH){
		x_direction = 1;
	}
	if (ball_x <= 0){
		x_direction = 0;
	}
	if (ball_y <= 0){
		y_direction = 0;
	}

	if (ball_x > bar_x && ball_x < (bar_x + BAR_WIDTH) && ball_y >= bar_y){
		y_direction = 1;
	}

	for (count = 0; count < MAX_BLOCKS; count++){
		if (blocks[count].flag == 1){
			if (ball_x >= blocks[count].x && ball_x <= blocks[count].x + BLOCK_WIDTH){
				if (ball_y == blocks[count].y){
					y_direction = 1;
					blocks[count].flag = 0;
					score_flag++;
				}
				else if (ball_y == blocks[count].y + BLOCK_HEIGHT){
					y_direction = 0;
					blocks[count].flag = 0;
					score_flag++;
				}
			}

			if (ball_y >= blocks[count].y && ball_y <= blocks[count].y + BLOCK_HEIGHT){
				if (ball_x == blocks[count].x){
					x_direction = 1;
					blocks[count].flag = 0;
					score_flag++;
				}
				else if (ball_x == blocks[count].x + BLOCK_WIDTH){
					x_direction = 0;
					blocks[count].flag = 0;
					score_flag++;
				}
			}
		}
	}
	if (score_flag != 0){
		score++;
	}
}

void my_game_over_check(){
	int flag_count = 0;
	int count;

	if (ball_y > (480 + BALL_SIZE)){
		game_over_flag = 1;
	}
	for (count = 0; count < MAX_BLOCKS; count++){
		flag_count = flag_count + blocks[count].flag;
	}
	if (flag_count == 0){
		game_over_flag = 1;
	}
}

void my_draw_field(){
	int count;
	DrawGraph(0, 0, back_img, TRUE);
	DrawBox(X_POSI + bar_x, bar_y,X_POSI +  bar_x + BAR_WIDTH, bar_y + BAR_HEIGHT, Color_Green1, true);
	DrawCircle(X_POSI + ball_x, ball_y, BALL_SIZE, Color_White, false, 1);
	for (count = 0; count < MAX_BLOCKS; count++){
		if (blocks[count].flag == 1){
			DrawBox(X_POSI + blocks[count].x, blocks[count].y, X_POSI + blocks[count].x + BLOCK_WIDTH, blocks[count].y + BLOCK_HEIGHT, blocks[count].color, true);
			DrawBox(X_POSI + blocks[count].x, blocks[count].y, X_POSI + blocks[count].x + BLOCK_WIDTH, blocks[count].y + BLOCK_HEIGHT, Color_Black, false);

		}
	}
	DrawBox(X_POSI + 0, 0,X_POSI + FIELD_WIDTH, FIELD_HEIGHT, Color_White, false);
}

void my_draw_op_ed(){
	if (ball_start_flag == 0 && game_over_flag == 0){
		DrawFormatString(230, 230, Color_White, "press enter key");
	}
	if (game_over_flag == 1){
		DrawFormatString(240, 210, Color_White, "‚f‚`‚l‚d@‚n‚u‚d‚q");
		ball_start_flag = 0;
	}
}

int my_get_key(){
	char keys[256];
	GetHitKeyStateAll(keys);
	for (int i = 0; i < 256; i++){
		if (keys[i] != 0){
			key[i]++;
		}
		else{
			key[i] = 0;
		}
	}
	return 0;
}
