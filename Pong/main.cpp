#include <iostream>
#include "raylib.h"

class Ball {
public:
float x, y;
int speed_x, speed_y;
int radius;

void Draw() {
    DrawCircle(x, y, radius, WHITE);
}

void Update() {
    x += speed_x;
    y += speed_y;

    // check collision logic for ball:
    // bounce off top and bottom screen window
    if(y + radius >= GetScreenHeight() || y + radius <= 0) speed_y *= -1;
    // bounce off left and right screen window
    if(x + radius >= GetScreenWidth() || x + radius <= 0) speed_x *= -1;

}
};

class Paddle {
public:
float width = 20, height = 80;
float x, y;
int speed;

void Draw() {
    DrawRectangle(x, y, width, height, WHITE);
}
};

Ball ball;
Paddle left_paddle;
Paddle right_paddle;

int main() {

    const int screen_width = 1024;
    const int screen_height = 768;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    ball.radius = 10;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 4;
    ball.speed_y = 4;

    left_paddle.x = 10;
    left_paddle.y = screen_height/2 - left_paddle.height/2;
    left_paddle.speed = 7;

    right_paddle.x = screen_width - right_paddle.width - 10;

    

    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Updating
        ball.Update();
        

        // Drawing
        ClearBackground(BLACK);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        left_paddle.Draw();

        

        EndDrawing();
    }


    CloseWindow();
    return 0;
}