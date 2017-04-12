#include <cmath>
#include <cstdlib>
#include <iostream>

#include "painter.h"
#include "SDL_utils.h"

using namespace std;

Painter::Painter(SDL_Window* window, SDL_Renderer* renderer_)
    : renderer(renderer_)
{
    SDL_RenderGetLogicalSize(renderer, &width, &height);
    if (width == 0 && height == 0)
        SDL_GetWindowSize(window, &width, &height);

    setPosition(width/2, height/2);
    setAngle(0);
    setColor(WHITE_COLOR);
    clearWithBgColor(BLUE_COLOR);
}

void Painter::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Painter::setAngle(float angle)
{
    this->angle = angle - floor(angle/360)*360;
}

void Painter::setColor(SDL_Color color)
{
    this->color = color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
}

void Painter::clearWithBgColor(SDL_Color bgColor)
{
    SDL_Color curColor = color;
    setColor(bgColor);
    SDL_RenderClear(renderer);
    setColor(curColor);
}

void Painter::moveForward(float length)
{
    float prevX = x, prevY = y;
    jumpForward(length);
    SDL_RenderDrawLine(renderer, (int)prevX, (int)prevY, (int)x, (int)y);
}

void Painter::jumpForward(float length)
{
    float rad = (angle / 180) * M_PI;
    x += (cos(rad) * length);
    y -= (sin(rad) * length);
}

void Painter::setRandomColor()
{
    Uint8 r = rand() % 256;
    Uint8 g = rand() % 256;
    Uint8 b = rand() % 256;
    SDL_Color color = {r, g, b};
    setColor(color);
}

void Painter::createCircle(float radius)
{
    double rad = (angle / 180) * M_PI;
    int centerX = x + cos(rad) * radius;
    int centerY = y - sin(rad) * radius;

    int dx = radius;
    int dy = 0;
    int err = 0;

    while (dx >= dy)
    {
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
        SDL_RenderDrawPoint(renderer, centerX + dy, centerY + dx);
        SDL_RenderDrawPoint(renderer, centerX - dy, centerY + dx);
        SDL_RenderDrawPoint(renderer, centerX - dx, centerY + dy);
        SDL_RenderDrawPoint(renderer, centerX - dx, centerY - dy);
        SDL_RenderDrawPoint(renderer, centerX - dy, centerY - dx);
        SDL_RenderDrawPoint(renderer, centerX + dy, centerY - dx);
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY - dy);

        if (err <= 0)
        {
            dy += 1;
            err += 2*dy + 1;
        }
        if (err > 0)
        {
            dx -= 1;
            err -= 2*dx + 1;
        }
    }
}

void Painter::createSquare(float size)
{
    for (int i = 0; i < 4; ++i) {
        moveForward(size);
        turnLeft(90);
    }
}

void Painter::createParallelogram(float size)
{
    for (int i = 0; i < 2; ++i) {
        moveForward(size);
        turnLeft(60);
        moveForward(size);
        turnLeft(120);
    }
}

void refreshScreen(SDL_Window* &window, SDL_Renderer* &renderer, Painter painter, SDL_Texture* &background, SDL_Texture* &heart, int live) {
    painter.clearWithBgColor(BLACK_COLOR);
    painter.setColor(WHITE_COLOR);
    painter.setPosition(0,0);
    painter.turnRight(60);
    painter.moveForward(10);
    painter.turnLeft(120);
    painter.moveForward(10);
    for (int i = 0; i < 59; i++) {
        painter.turnRight(120);
        painter.moveForward(10);
        painter.turnLeft(120);
        painter.moveForward(10);
    }
    painter.setPosition(0,600);
    painter.moveForward(10);
    painter.turnRight(120);
    painter.moveForward(10);
    for (int i = 0; i < 59; i++) {
        painter.turnLeft(120);
        painter.moveForward(10);
        painter.turnRight(120);
        painter.moveForward(10);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 600, 0, 600, 600);
    renderTexture(background, renderer,601, 0);

    for (int i = 1; i <= live; i++) {
        renderTexture(heart, renderer, 610, 200 + i*20, 20,20);
        }
}
