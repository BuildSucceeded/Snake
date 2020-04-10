#pragma once

#include "resource.h"
#include "Point2D.h"
#include "Snake.h"

class Food
{
public:
	Food();
	~Food();

	void Initialize(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Reset(Snake* snake);
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);

	Point2D position;

private:

	ID2D1SolidColorBrush* m_pBlueBrush;
};

