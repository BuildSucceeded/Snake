#include "framework.h"
#include "Point2D.h"
#include "Engine.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

Engine::Engine() : m_pDirect2dFactory(NULL), m_pRenderTarget(NULL), m_pWhiteBrush(NULL)
{
    snake = new Snake();
    food = new Food();
    food->Reset(snake);

    playing = true;
    keyPressed = false;

    score = 5;
    highScore = 5;
}

Engine::~Engine()
{
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pWhiteBrush);
}

HRESULT Engine::InitializeD2D(HWND m_hwnd)
{
    // Initializes Direct2D, to draw with
    D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
    m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
        &m_pRenderTarget
    );

    // Initialize text writing factory and format
    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(m_pDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
    );

    m_pDWriteFactory->CreateTextFormat(
        L"Verdana",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        20,
        L"", //locale
        &m_pTextFormat
    );

    m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

    m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &m_pWhiteBrush
    );

    snake->Initialize(m_pRenderTarget);
    food->Initialize(m_pRenderTarget);

    return S_OK;
}

void Engine::KeyUp(WPARAM wParam)
{
    if (!keyPressed)
    {
        if (wParam == VK_UP)
            snake->GoUp();
        if (wParam == VK_DOWN)
            snake->GoDown();
        if (wParam == VK_LEFT)
            snake->GoLeft();
        if (wParam == VK_RIGHT)
            snake->GoRight();
        keyPressed = true;
    }
}

void Engine::Reset()
{
    // This method reset the game, given that the game was won or lost
    if (!playing)
    {
        snake->Reset();
        food->Reset(snake);
        playing = true;
        score = 5;
    }
}

void Engine::Logic(double elapsedTime)
{
    // This is the logic part of the engine.
    if (playing)
    {
        snake->Advance();

        if (snake->CheckFoodCollision(food->position.x, food->position.y))
        {
            food->Reset(snake);
            snake->Grow();
            score++;
            if (score > highScore)
                highScore = score;
        }

        if (snake->CheckSelfCollision())
        {
            playing = false;
        }

        keyPressed = false;
    }

    Sleep(FRAME_SLEEP);
}

HRESULT Engine::Draw()
{
    // This is the drawing method of the engine.
    // It simply draws all the elements in the game using Direct2D
    HRESULT hr;

    m_pRenderTarget->BeginDraw();

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    // Draw score
    D2D1_RECT_F rectangle2 = D2D1::RectF(0, 0, RESOLUTION_X, 200);
    
    WCHAR scoreStr[64];
    swprintf_s(scoreStr, L"Score: %d            Top Score: %d     ", score, highScore);
    m_pRenderTarget->DrawText(
        scoreStr,
        35,
        m_pTextFormat,
        rectangle2,
        m_pWhiteBrush
    );

    snake->Draw(m_pRenderTarget);
    food->Draw(m_pRenderTarget);
    
    hr = m_pRenderTarget->EndDraw();

    return S_OK;
}