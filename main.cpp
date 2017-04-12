#include <iostream>
#include <ctime>
#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <SDL_ttf.h>

#include "painter.h"
#include "SDL_utils.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Rapid Roll";

int main(int argc, char* argv[]) {
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    SDL_Texture* gameover = loadTexture("game_over.bmp", renderer);
    SDL_Texture* heart = loadTexture("heart.bmp", renderer);
    SDL_Texture* background = loadTexture("background.bmp", renderer);
    if (background == nullptr || gameover == nullptr || heart == nullptr) {
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(gameover);
        SDL_DestroyTexture(heart);
        quitSDL(window, renderer);
    }

    int score = 0;

    TTF_Font* font = NULL;
    font = TTF_OpenFont("VTIR.TTF", 21);
    if (font == NULL)
    {
        cout << "chua nap font" << endl;
    }
    SDL_Color White = {255, 255, 255};
    SDL_Surface* lives = TTF_RenderText_Solid(font, "Lives", White);
    SDL_Texture* title_live = SDL_CreateTextureFromSurface(renderer, lives);
    SDL_Surface* scores = TTF_RenderText_Solid(font, "Score", White);
    SDL_Texture* title_score = SDL_CreateTextureFromSurface(renderer, scores);

    //SDL_RenderPresent(renderer);

    Painter painter(window, renderer);
    int live = 3;

    refreshScreen(window, renderer, painter, background, heart, live);

    //toa do ban dau cua ball
    int x = 300;
    int y = 400;

    SDL_Rect green_bar[600];
    srand(time(0));
    green_bar[0].x = 285;
    green_bar[0].y = 410;
    green_bar[0].h = 5;
    green_bar[0].w = 45;
    for (int i = 1; i < 600; i++) {
        green_bar[i].h = 5;
        green_bar[i].w = 45;
        green_bar[i].x = rand() % 500;
        green_bar[i].y = 595;
    }

    /*SDL_Rect red_bar[500];
    for (int i = 0; i < 500; i++) {
        red_bar[i].x = rand() % 500;
        red_bar[i].y = 595;
        red_bar[i].h = 5;
        red_bar[i].w = 45;
    }*/

    int step = 30;
    bool ballUp = true;
    bool rightDirection = true;
    bool doNotMove = true;
    SDL_Event e;
    bool running = true;
    bool gameOver = false;
    int green_cur = 0; //red_cur = 0;
    float time = 300;
    while (running) {
        SDL_Delay(time);
        time -= 0.5;

        SDL_Surface* score_cur = TTF_RenderText_Solid(font, numberToString(score).c_str(), White);
        SDL_Texture* Score_render = SDL_CreateTextureFromSurface(renderer, score_cur);

        SDL_RenderClear(renderer);
        //ve lai phong nen cua game
        refreshScreen(window, renderer, painter, background, heart, live);
        renderTexture(title_live,renderer, 605, 200, 45, 20);
        renderTexture(title_score, renderer, 680, 200, 50, 20);
        renderTexture(Score_render, renderer, 685, 230, 40, 20);

        //neu thua hien thi Game Over
        /*if (gameOver) {
            renderTexture(gameover, renderer, 100, 200, 400, 250);
            renderTexture(title_score, renderer, 240, 450, 50, 20);
            renderTexture(Score_render, renderer, 300, 450, 40, 20);
            running = false;
        }*/
        //ve hinh tron
        painter.setColor(GREEN_COLOR);
        painter.setPosition(x, y);
        painter.createCircle(10);
        //ve green bar
        for (int i = 0; i <= green_cur; i++) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &green_bar[i]);
            }
        //ve red bar
        /*for (int i =0; i <= red_cur; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &green_bar[i]);
            }*/
        if (gameOver) {
            renderTexture(gameover, renderer, 100, 200, 400, 250);
            renderTexture(title_score, renderer, 240, 450, 50, 20);
            renderTexture(Score_render, renderer, 300, 450, 40, 20);
            running = false;
        }
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&e)) {
            // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
            if (e.type == SDL_QUIT) running = false;

            // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
            if (e.type == SDL_KEYDOWN) {
                // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
                if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
                else if (e.key.keysym.sym == SDLK_LEFT) {
                        rightDirection = false;
                        doNotMove = false;
                        }
                else if (e.key.keysym.sym == SDLK_RIGHT) {
                        rightDirection = true;
                        doNotMove = false;
                        }
                }
        }

        if (doNotMove) x = x;
        else if (rightDirection)
            x = x + step/2;
        else
            x = x - step/2;
        for (int i = 0; i <= green_cur; i++) {
            if ((x >= green_bar[i].x-19) && (x <= green_bar[i].x+45) && (y >= green_bar[i].y-20) && (y <= green_bar[i].y-10)){
                ballUp = true;
                score += 100;
            }
        }
        if (ballUp)
            y -= step/2;
        else y += step/2;
        doNotMove = true;
        ballUp = false;

        if (green_bar[green_cur].y <= 450) green_cur++;

        //if (red_bar[red_cur].y <= 250) red_cur++;

        //kiem tra neu bong cham rang cua thi mat di mot live
        if (y <= 10 || y >= 590) {
            live--;
            x = green_bar[green_cur].x;
            y = green_bar[green_cur].y - 25;
        }

        for (int i = 0; i <= green_cur; i++)
            green_bar[i].y -= step/2;
        /*for (int i = 0; i <= red_cur; i++)
            red_bar[i].y -= step/2;*/
        if (live == 0) gameOver = true;
    }

	waitUntilKeyPressed();
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(gameover);
	SDL_DestroyTexture(heart);
    SDL_DestroyTexture(title_live);
    SDL_DestroyTexture(title_score);
    SDL_FreeSurface(lives);
    quitSDL(window, renderer);
    return 0;
}
