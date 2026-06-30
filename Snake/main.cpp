#include <iostream>
#include "raylib.h"
#include <deque>
#include <raymath.h>

using namespace std;

Color green =     {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }

    return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for (unsigned int i = 0; i < deque.size(); i++) {
        if (Vector2Equals(deque[i], element))
            return true;
    }

    return false;
}


class Food {
public:
    Vector2 position{};
    Texture2D texture;

    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("images/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food() {
        UnloadTexture(texture);
    }

    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        
        return Vector2 {x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
        Vector2 position = GenerateRandomCell();
        // check if food randomly spawns in a position where snake body is at
        while(ElementInDeque(position, snakeBody)) {
            position = GenerateRandomCell();
        }

        return position;
    }

    void Draw() {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }

};

class Snake {
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};

    // default: snake moves right at start of game
    Vector2 direction = {1, 0}; 

    bool addSegment = false;

    void Draw() {
        for (int i = 0; i < body.size(); i++) {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, darkGreen);
        }
    }

    void Update() {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true) {
            addSegment = false;
        } else {
            body.pop_back();
        }
    }

    void Reset() {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update() {
        if (running) {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithWalls();
        }
    }

    void CheckCollisionWithFood() {
        if(Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
        }
    }

    void CheckCollisionWithWalls() {
        if (snake.body[0].x >= cellCount || snake.body[0].x <= 0) {
            // snake.body.pop_back();
            GameOver();
        }

        if (snake.body[0].y >= cellCount || snake.body[0].y <= 0) {
            // snake.body.pop_back();
            GameOver();
        }
    }

    void GameOver() {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
    }
};

int main() {
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake");
    SetTargetFPS(60);
    InitAudioDevice();

    Game game = Game();

    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Update & Logic
        if (EventTriggered(0.2)) {
            game.Update();
        }

        // keybindings to move snake
        if (IsKeyPressed(KEY_W) && game.snake.direction.y !=  1) game.snake.direction = {0, -1};
        if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1) game.snake.direction = {0, 1};
        if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1) game.snake.direction = {1, 0};
        if (IsKeyPressed(KEY_A) && game.snake.direction.x !=  1) game.snake.direction = {-1, 0};

        // Drawing
        ClearBackground(green);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}