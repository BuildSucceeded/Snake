#include "framework.h"
#include "Point2D.h"
#include "Snake.h"
#include "Engine.h"

Snake::Snake() : m_pGreenBrush(NULL), m_pRedBrush(NULL)
{
	Reset();
}

Snake::~Snake()
{
	SafeRelease(&m_pGreenBrush);
	SafeRelease(&m_pRedBrush);
}

void Snake::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a green and red brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&m_pGreenBrush
	);
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pRedBrush
	);
}

void Snake::Reset()
{
	length = 5;

	direction.x = 1;
	direction.y = 0;

	for (int i = 0; i < length; i++)
	{
		position[i].x = 10 - i;
		position[i].y = 10;
	}
}

void Snake::GoUp()
{
	if (direction.y == 0)
	{
		direction.x = 0;
		direction.y = -1;
	}
}
void Snake::GoDown()
{
	if (direction.y == 0)
	{
		direction.x = 0;
		direction.y = 1;
	}
}
void Snake::GoLeft()
{
	if (direction.x == 0)
	{
		direction.x = -1;
		direction.y = 0;
	}
}
void Snake::GoRight()
{
	if (direction.x == 0)
	{
		direction.x = 1;
		direction.y = 0;
	}
}

void Snake::Advance()
{
	for (int i = length - 1; i > 0; i--)
	{
		position[i].x = position[i - 1].x;
		position[i].y = position[i - 1].y;
	}
	position[0].x = position[0].x + direction.x;
	position[0].y = position[0].y + direction.y;
}

bool Snake::CheckSelfCollision()
{
	for (int i = 1; i < length; i++)
	{
		if (position[0].x == position[i].x && position[0].y == position[i].y)
		{
			return true;
		}
	}
	if (position[0].x < 0) return true;
	if (position[0].y < 0) return true;
	if (position[0].x >= RESOLUTION_X / CELL_SIZE) return true;
	if (position[0].y >= RESOLUTION_Y / CELL_SIZE) return true;
	return false;
}

bool Snake::CheckFoodCollision(int x, int y)
{
	if (position[0].x == x && position[0].y == y)
	{
		return true;
	}
	return false;
}

void Snake::Grow()
{
	position[length].x = position[length - 1].x;
	position[length].y = position[length - 1].y;
	length++;
}

void Snake::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws the snake using Direct2D

	// Snake Body
	for (int i = 1; i < length; i++)
	{
		D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
			D2D1::Point2F(position[i].x * CELL_SIZE + CELL_SIZE / 2, position[i].y * CELL_SIZE + CELL_SIZE / 2),
			CELL_SIZE / 2, CELL_SIZE / 2
		);
		m_pRenderTarget->FillEllipse(&ellipseBall, m_pGreenBrush);
	}

	// Snake Head
	D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
		D2D1::Point2F(position[0].x * CELL_SIZE + CELL_SIZE / 2, position[0].y * CELL_SIZE + CELL_SIZE / 2),
		CELL_SIZE / 2, CELL_SIZE / 2
	);
	m_pRenderTarget->FillEllipse(&ellipseBall, m_pRedBrush);

}