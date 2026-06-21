#include <iostream>
#include "raylib.h"

Color Green       =       Color{38, 185, 154, 255};
Color Dark_Green  =       Color{20, 160, 133, 255};
Color Light_Green =       Color{129, 204, 184, 255};
Color Yellow      =       Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    float x, y;
    float speed_x = 4, speed_y = 4;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update(Sound hit_wall, Sound score, Sound cpu_scores) {
        x += speed_x;
        y += speed_y;

        // check collision logic for ball:
        // bounce off top and bottom screen window
        if(y + radius >= GetScreenHeight() || y + radius <= 0) {
            speed_y *= -1;
            PlaySound(hit_wall);
        }
        
        // Manage score if ball passes left or right screen window
        if(x + radius >= GetScreenWidth()) {
            player_score++;
            PlaySound(score);
            ResetBall();
        }
        if(x + radius <= 0) {
            cpu_score++;
            PlaySound(cpu_scores);
            ResetBall();
        }

    }

    void ResetBall() {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        speed_x = 4;
        speed_y = 4;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0)                          y = 0;
        if (y + height >= GetScreenHeight()) y = GetScreenHeight() - height;
    }
public:
    float width = 20, height = 80;
    float x, y;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle(x, y, width, height), 0.8, 0, WHITE);
    }

    void Update() {
        if(IsKeyDown(KEY_W))                 y = y - speed; 
        if(IsKeyDown(KEY_S))                 y = y + speed; 
        LimitMovement();
    }

};

class AiPaddle: public Paddle {
public:

void Update(int ball_center)
{
    if (y + height/2 > ball_center) y = y - speed;
    if (y + height/2 < ball_center) y = y + speed; 
    LimitMovement(); 
}

};

Ball ball;
Paddle left_paddle;
AiPaddle right_paddle;

int main() {

    const int screen_width = 1280;
    const int screen_height = 720;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    // Initialize the audio device context
    InitAudioDevice();

    // Load sounds
    Sound hit_wall = LoadSound("audio/hit_wall.wav");
    Sound hit_paddle = LoadSound("audio/hit_paddle.wav");
    Sound score = LoadSound("audio/score.wav");
    Sound cpu_scores = LoadSound("audio/cpu_score.wav");

    ball.radius = 10;
    ball.x = screen_width/2;
    ball.y = screen_height/2;

    left_paddle.x = 10;
    left_paddle.y = screen_height/2 - left_paddle.height/2;
    left_paddle.speed = 7;

    right_paddle.x = screen_width - right_paddle.width - 10;
    right_paddle.y = screen_height/2 - right_paddle.height/2;
    right_paddle.speed = 4;

    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Updating
        ball.Update(hit_wall, score, cpu_scores);
        left_paddle.Update();
        right_paddle.Update(ball.y);

        // Checking for collisions with paddles
        // ball gets faster the more it collides with paddles
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{left_paddle.x,  left_paddle.y,  left_paddle.width,  left_paddle.height }) ||
            CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{right_paddle.x, right_paddle.y, right_paddle.width, right_paddle.height}))
        {
            ball.speed_x *= -1;
            ball.speed_x *= 1.1; // increase magnitude by 10%
            PlaySound(hit_paddle);
        }
        
        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        left_paddle.Draw();
        right_paddle.Draw();
        DrawText(TextFormat("%i", player_score), screen_width/4 - 20, 20, 80,  WHITE);
        DrawText(TextFormat("%i", cpu_score), 3 * screen_width/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    // Unload resources
    UnloadSound(hit_wall);
    UnloadSound(hit_paddle);
    UnloadSound(score);
    UnloadSound(cpu_scores);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}