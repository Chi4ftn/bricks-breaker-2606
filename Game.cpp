#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	bricks.clear();
	for (int i = 0; i < 5; ++i)
	{
		Box nBrick;
		nBrick.width = 10;
		nBrick.height = 2;
		nBrick.x_position = i * 15 + 5;
		nBrick.y_position = 5;
		nBrick.doubleThick = true;
		nBrick.color = ConsoleColor::DarkGreen;
		bricks.push_back(nBrick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (const Box& brick : bricks)
	{
	brick.Draw();
	}

	if (bricks.empty()) {
		Console::SetCursorPosition(25, 15);
		std::cout << "You Win! Press 'R' to play again." << std::endl;
	}
	else if (ball.y_position >= WINDOW_HEIGHT - 1) 
	{
		Console::SetCursorPosition(25, 15);
		std::cout << "You Lose. Press 'R' to play again." << std::endl;
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (auto iter = bricks.begin(); iter != bricks.end();)
	{
		if (iter->Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			iter->color = ConsoleColor(iter->color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (iter->color == ConsoleColor::Black)	{ iter = bricks.erase(iter); }
			else { ++iter; }
		}
		else { ++iter; }
	}
	

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty())
	{
		ball.moving = false;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT - 1) {
		ball.moving = false;
	}
}
