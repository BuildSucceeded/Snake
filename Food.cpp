#include "framework.h"
#include "Point2D.h"
#include "Food.h"
#include "Engine.h"

Food::Food() : m_pBlueBrush(NULL)
{
}

Food::~Food()
{
	SafeRelease(&m_pBlueBrush);
}

void Food::Initialize(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a blue brush for drawing
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&m_pBlueBrush
	);
}

void Food::Reset(Snake* snake)
{
	srand((unsigned)time(NULL));

	bool found = false;
	while (!found)
	{
		position.x = rand() % (RESOLUTION_X / CELL_SIZE);
		position.y = rand() % (RESOLUTION_Y / CELL_SIZE);

		found = true;
		for (int i = 0; i < snake->length; i++)
		{
			if (snake->position[i].x == position.x && snake->position[i].y == position.y)
			{
				found = false;
			}
		}
	}
}

void Food::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Draws the food using Direct2D

	D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
		D2D1::Point2F(position.x * CELL_SIZE + CELL_SIZE / 2, position.y * CELL_SIZE + CELL_SIZE / 2),
		CELL_SIZE / 2, CELL_SIZE / 2
	);
	m_pRenderTarget->FillEllipse(&ellipseBall, m_pBlueBrush);

}