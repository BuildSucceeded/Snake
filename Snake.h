#pragma once

#include "resource.h"
#include "Point2D.h"

class Snake
{
public:
	Snake();
	~Snake();

	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	void GoUp();
	void GoDown();
	void GoLeft();
	void GoRight();
	void Reset();
	void Advance();
	bool CheckSelfCollision();
	bool CheckFoodCollision(int x, int y);
	void Grow();
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);

	Point2D position[1000];
	Point2D direction;
	int length;

private:

	ID2D1SolidColorBrush* m_pRedBrush;
	ID2D1SolidColorBrush* m_pGreenBrush;
};
