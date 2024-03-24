
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include <time.h>

Rectangle randomizerRect2(int upperX, int lowerX, int upperY, int lowerY) {
	srand(time(NULL));
	Rectangle value = {rand() % (upperX - lowerX + 1) + lowerX, rand() % (upperY - lowerY + 1) + lowerY};
	return value;
}

int main(void) {

	srand(time(NULL));

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dong");
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetWindowMaxSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	InitAudioDevice();

	Sound paddleHit = LoadSound("resources/paddleHit.wav");
	Sound death = LoadSound("resources/death.wav");

	SetTargetFPS(60);

	Rectangle ball = {4, 24, 10, 10};
	Vector2 ballSpeed = {5, 5};

	Rectangle floor = {0, 580, 800, 20};
	Rectangle ceiling = {0, 0, 800, 20};

	Rectangle paddleOne = {0, 300, 15, 50};
	Rectangle paddleTwo = {785, 300, 15, 50};

	int padOneScore = 0;
	int padTwoScore = 0;

	while (!WindowShouldClose()) {

		bool floorCollison = CheckCollisionRecs(ball, (Rectangle){0, 575, 800, 20});
		bool ceilingCollison = CheckCollisionRecs(ball, ceiling);

		bool paddleOneCollison = CheckCollisionRecs(ball, paddleOne);
		bool paddleTwoCollison = CheckCollisionRecs(ball, paddleTwo);

		bool paddleOneCollisonFloor = CheckCollisionRecs(paddleOne, floor);
		bool paddleOneCollisonCeiling = CheckCollisionRecs(paddleOne, ceiling);

		bool paddleTwoCollisonFloor = CheckCollisionRecs(paddleTwo, floor);
		bool paddleTwoCollisonCeiling = CheckCollisionRecs(paddleTwo, ceiling);

		if(floorCollison || ceilingCollison) {
			ballSpeed.y *= -1;
		}

		if(paddleOneCollison || paddleTwoCollison) {
			ballSpeed.x *= -1; 
			PlaySound(paddleHit);
		}

		if(paddleOneCollisonCeiling) paddleOne.y = 25;
		if(paddleTwoCollisonCeiling) paddleTwo.y = 25;

		if(paddleOneCollisonFloor) paddleOne.y = 535;
		if(paddleTwoCollisonFloor) paddleTwo.y = 535;

		if(ball.x <= 0){
			ball = randomizerRect2(200, 20, 40, 22);
			ballSpeed.x = 5;
			ballSpeed.y = 5;
			PlaySound(death);
			padTwoScore++;
		}

		if(ball.x >= 800){
			ball = randomizerRect2(200, 20, 40, 22);
			ballSpeed.x = 5;
			ballSpeed.y = 5;
			PlaySound(death);
			padOneScore++;
		}

		ball.x += ballSpeed.x;
		ball.y += ballSpeed.y;

		if (IsKeyDown(KEY_W)) paddleOne.y -= 10.0f;
		if (IsKeyDown(KEY_S)) paddleOne.y += 10.0f;
		if (IsKeyDown(KEY_UP)) paddleTwo.y -= 10.0f;
		if (IsKeyDown(KEY_DOWN)) paddleTwo.y += 10.0f;

		BeginDrawing();

			ClearBackground(BLACK);

			DrawRectangleV((Vector2){ball.x, ball.y}, (Vector2){10, 10}, WHITE);

			DrawRectangleV((Vector2){floor.x, floor.y}, (Vector2){floor.width, floor.height}, WHITE);
			DrawRectangleV((Vector2){ceiling.x, ceiling.y}, (Vector2){ceiling.width, ceiling.height}, WHITE);
			
			DrawRectangleRec(paddleOne, WHITE);
			DrawRectangleRec(paddleTwo, WHITE);

			DrawText(TextFormat("%d", padOneScore), 200, 75, 50, WHITE);
			DrawText(TextFormat("%d", padTwoScore), 575, 75, 50, WHITE);

		EndDrawing();
	}

	UnloadSound(paddleHit);
	UnloadSound(death);

	CloseAudioDevice();  

	CloseWindow();

	return 0;
}