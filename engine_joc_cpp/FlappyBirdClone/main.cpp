#include "raylib.h"
#include <fstream>
using namespace std;

enum GameScreen { MENIU, JOC, GAME_OVER };

Rectangle sursa = { 55.0f, 0.0f, 250, 180 };
Rectangle dest = { 100, 100, 1700, 900 };

void draw_gif(Image anim, Texture2D tex, int *cadruCurent, int* contorTimp, int viteza, int x, int y, int nrCadre)
{
    (*contorTimp)++;
    if (*contorTimp >= viteza)
    {
        (*cadruCurent)++;
        if (*cadruCurent >= nrCadre)
            *cadruCurent = 0;
        int offset = anim.width * anim.height * 4 * (*cadruCurent);
        UpdateTexture(tex, ((unsigned char*)anim.data) + offset);
    }
    DrawTexturePro(tex, sursa, dest, { 0, 0 }, 0.0f, WHITE);
}

int incarcaHighScore()
{
    int scorSalvat = 0;
    ifstream fin("highscore.txt");
    if (fin.is_open())
    {
        fin >> scorSalvat;
        fin.close();
    }
    return scorSalvat;
}

void salveazaHighScore(int scor)
{
    ofstream fout("highscore.txt");
    if (fout.is_open())
    {
        fout << scor;
        fout.close();
    }
}


int main()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Flappy Bird");
    GameScreen ecranCurent = MENIU;

    int score = 0;
    int high_score = incarcaHighScore();

    Texture2D var = LoadTexture("banu.jpeg");
    Texture2D pipe = LoadTexture("spring.jpeg");
    Texture2D end = LoadTexture("final.jpeg");
    Texture2D MENU = LoadTexture("banu2.jpeg");

    SetTargetFPS(60);
    bool gameOver = false;
    bool running = true;

    float birdX = 300.0f;
    float birdY = screenHeight / 2.;
    float birdRadius = 15.0f;
    float jump = 0.0f;
    float gravity = 1.f;

    float pipeX = 1280;
    float pipeWidth = 160;
    float pipeSpeed = 10;
    float gapY = 360;
    float gapSize = 280;

    int nrCadreGif = 0;
    Image imgGif = LoadImageAnim("rari2.gif", &nrCadreGif);
    Texture2D texGif = LoadTextureFromImage(imgGif);
    int cadruActual = 0;
    int timpActual = 0;

    //butoane
        Rectangle buton_start = { 250, 200, 350, 100 };
        Rectangle buton_exit = { 250, 330, 350, 100 };
        Rectangle buton_meniu = {200 ,500 ,350, 100};

    while (!WindowShouldClose() && running)
    {
        Vector2 mouse_pos = GetMousePosition();
        if (ecranCurent == MENIU)
        {
            if (score > high_score)
                high_score = score;
            score = 0;
            //if (IsKeyPressed(KEY_ENTER))
            //{
            //    pipeX = screenWidth;
            //    gameOver = false;
            //    jump = 0;
            //    birdY = screenHeight / 2.;
            //    ecranCurent = JOC;
            //}
            if (CheckCollisionPointRec(mouse_pos, buton_start))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    gameOver = false;
                    birdY = screenHeight / 2.;
                    jump = 0;
                    pipeX = 1920;
                    ecranCurent = JOC;
                }
            }
            if (CheckCollisionPointRec(mouse_pos, buton_exit))
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    running = false;

        }
        else if (ecranCurent == JOC)
        {
            if (gameOver == false)
            {
                if (IsKeyPressed(KEY_SPACE))
                    jump = -15.0f;
                jump += gravity;
                birdY += jump;

                pipeX -= pipeSpeed;
                if (pipeX + pipeWidth < 0)
                {
                    gapY = GetRandomValue(100, 650);
                    pipeX = screenWidth;
                }
                if (pipeX == birdX)
                    score++;
                if (birdY <= 0 || birdY + 100 >= screenHeight || ((birdY <= gapY - gapSize / 2 || birdY >= gapY + gapSize / 2 - 100) && (pipeX <= birdX + 100 && pipeX >= birdX)))
                    gameOver = true;
            }
            if (gameOver == true)
            {
                ecranCurent = GAME_OVER;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mouse_pos, buton_meniu))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    ecranCurent = MENIU;
                }
            }
            //if (IsKeyPressed(KEY_SPACE))
            //{
            //    gameOver = false;
            //    ecranCurent = MENIU;
            //}
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        switch (ecranCurent)
        {
        case JOC:
            DrawTexture(var, (int)birdX, (int)birdY, WHITE);

            DrawTexturePro(pipe,
                { 0.0f, 0.0f, (float)pipe.width, (float)pipe.height },
                { pipeX, 0, pipeWidth, gapY - gapSize / 2 },
                { 0, 0 }, 0.0f, WHITE);
            DrawTexturePro(pipe,
                { 0.0f, 0.0f, (float)pipe.width, (float)pipe.height },
                { pipeX, gapY + gapSize / 2, pipeWidth, screenHeight - (gapY + gapSize / 2) },
                { 0, 0 }, 0.0f, WHITE);
            DrawText(TextFormat("Score: %i", score), 20, 20, 40, BLACK);
            DrawText(TextFormat("High Score: %i", high_score), 20, 60, 40, BLACK);

            break;
        case MENIU:
            draw_gif(imgGif, texGif, &cadruActual, &timpActual, 8, 100, 100, nrCadreGif);
            //DrawTexture(MENU, 0, 0, WHITE);
            DrawText("Esti in meniul jocului!", 455, 755, 100, BLACK);
            DrawText("Esti in meniul jocului!", 450, 750, 100, RED);

            //DrawText("Apasa [ENTER] pentru a incepe", 500, 800, 50, WHITE);

            if (CheckCollisionPointRec(mouse_pos, buton_start))
                DrawRectangleRec(buton_start, BLUE);
            else
                DrawRectangleRec(buton_start, RED);
               
            if(CheckCollisionPointRec(mouse_pos, buton_exit))
                DrawRectangleRec(buton_exit, BLUE);
            else
                DrawRectangleRec(buton_exit, RED);

            DrawText("START", buton_start.x + 40, buton_start.y + 15, 80, RAYWHITE);
            DrawText("EXIT", buton_exit.x + 70, buton_exit.y + 15, 80, RAYWHITE);

            break;
        case GAME_OVER:
            DrawText("Ai pierdut!", 600, 300, 100, RED);
            //DrawText("Apasa [SPACE] pentru a te intoarce in meniu", 500, 400, 50, RED);
            DrawTexture(end, 800, 500, WHITE);
            if (CheckCollisionPointRec(mouse_pos, buton_meniu))
                DrawRectangleRec(buton_meniu, BLUE);
            else
                DrawRectangleRec(buton_meniu, RED);
            DrawText("MENIU", buton_meniu.x + 40, buton_meniu.y + 15, 80, RAYWHITE);
            break;

        }
        EndDrawing();
    }
    salveazaHighScore(high_score);
    CloseWindow();
    return 0;
}