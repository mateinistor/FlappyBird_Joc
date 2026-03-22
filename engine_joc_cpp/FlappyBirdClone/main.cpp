#include "raylib.h"

int main()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Flappy Bird");
    Texture2D var = LoadTexture("banu.jpeg");
    Texture2D pipe = LoadTexture("spring.jpeg");
    Texture2D end = LoadTexture("final.jpeg");

    SetTargetFPS(60);
    bool gameOver = false;
    
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


    while (!WindowShouldClose())
    {
        if (gameOver == false)
        {
            if (IsKeyPressed(KEY_SPACE))
                jump = -15.0f;
            jump += gravity;
            birdY += jump;

            pipeX -= pipeSpeed;
            if(pipeX + pipeWidth < 0)
            {
                gapY = GetRandomValue(100, 650);
                pipeX = screenWidth;
            }
            if (birdY <= 0 || birdY + 100 >= screenHeight || ((birdY <= gapY - gapSize / 2 || birdY >= gapY + gapSize / 2 -100) && (pipeX <= birdX + 100 && pipeX >= birdX)))
                gameOver = true;
        }
        if (gameOver == true)
        {


            if (IsKeyPressed(KEY_ENTER))
            {
                birdY = screenHeight / 2.0f;
                jump = 0;
                pipeX = screenWidth;
                gameOver = false;
            }
        }

    BeginDrawing();
    ClearBackground(SKYBLUE);

    //DrawCircle((int)birdX, (int)birdY, birdRadius, YELLOW);
    DrawTexture(var, (int)birdX, (int)birdY, WHITE);
    
    //DrawRectangle(pipeX, 0, pipeWidth, gapY - gapSize / 2, GREEN); //sus
    DrawTexturePro(pipe,
        { 0.0f, 0.0f, (float)pipe.width, (float)pipe.height },
        { pipeX, 0, pipeWidth, gapY - gapSize / 2 },
        { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(pipe,
        { 0.0f, 0.0f, (float)pipe.width, (float)pipe.height },
        { pipeX, gapY + gapSize / 2, pipeWidth, screenHeight - (gapY + gapSize / 2) },
        { 0, 0 }, 0.0f, WHITE);

    if (gameOver == true)
    {
        DrawText("Ai pierdut!", 600, 300, 50, RED);
        DrawText("Apasa [ENTER] pentru a reincepe", 500, 400, 50, RED);
        DrawTexture(end, 800, 500, WHITE);
    }

    //DrawRectangle(pipeX, gapY + gapSize / 2, pipeWidth, screenHeight, GREEN); //jos

    EndDrawing();
    }

    CloseWindow();
    return 0;
}