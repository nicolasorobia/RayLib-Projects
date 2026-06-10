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

    // check collision logic for ball
    if(y + radius >= GetScreenHeight() || y + radius <= 0) speed_y *= -1;
    if(x + radius >= GetScreenWidth() || x + radius <= 0) speed_x *= -1;

}

};

Ball ball;

int main() {

    const int screen_width = 1024;
    const int screen_height = 728;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    ball.radius = 10;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 4;
    ball.speed_y = 4;

    int rec_width = 20;
    int rec_height = 80;
    int padding = 10;
    int left_paddle_xpos = padding;
    int right_paddle_xpos = screen_width - (rec_width + padding);
    const int y_rect_center = screen_height/2 - rec_height/2;

    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Updating
        ball.Update();

        // check collision logic for ball
        // if (ball.y >= screen_height || ball.y <= 0) ball.speed_y *= -1;
        // if (ball.x >= screen_width || ball.x <= 0) ball.speed_x *= -1;

        // Drawing
        ClearBackground(BLACK);
        DrawLine(screen_width/2, 0, screen_width/2, 728, WHITE);
        ball.Draw();
        DrawRectangle(left_paddle_xpos, y_rect_center, rec_width, rec_height, WHITE);
        DrawRectangle(right_paddle_xpos, y_rect_center, rec_width, rec_height, WHITE);

        

        EndDrawing();
    }


    CloseWindow();
    return 0;
}