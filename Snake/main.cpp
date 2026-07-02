#include <iostream>
#include "raylib.h"
#include <deque>
#include <raymath.h>

using namespace std;

Color green =     {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

float cellSize = 30;
float cellCount = 25;
float offset = 75;
float windowSize = 2 * offset + cellSize * cellCount;

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
        // then make it spawn somewhere else
        while(ElementInDeque(position, snakeBody)) {
            position = GenerateRandomCell();
        }

        return position;
    }

    void Draw() {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
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
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, cellSize, cellSize};
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
    int score = 0;
    int hiscore = 0;
    bool running = false;
    Sound eat, wall;

    Game() {
        InitAudioDevice();
        eat = LoadSound("audio/eat.mp3");
        wall = LoadSound("audio/wall.mp3");
    }

    ~Game() {
        UnloadSound(eat);
        UnloadSound(wall);
        CloseAudioDevice();
    }

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update() {
        if (running) {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithWalls();
            CheckCollisionWithBody();
        }
    }

    void CheckCollisionWithFood() {
        if(Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            PlaySound(eat);
            score++;
            if (hiscore < score) hiscore = score;
        }
    }

    void CheckCollisionWithWalls() {
        if (snake.body[0].x >= cellCount || snake.body[0].x == -1) {
            // snake.body.pop_back();
            GameOver();
            PlaySound(wall);
        }

        if (snake.body[0].y >= cellCount || snake.body[0].y == -1) {
            // snake.body.pop_back();
            GameOver();
            PlaySound(wall);
        }
    }

    void CheckCollisionWithBody() {
        for (int i = 1; i < snake.body.size(); i++) {
            if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
                GameOver();
                PlaySound(wall);
            }
        }
    }

    void GameOver() {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
    }
};

int main() {
    // window is a square so window width and height are the same
    InitWindow(windowSize, windowSize, "Snake");
    SetTargetFPS(60);
    
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
        // press 'space' to start game
        if (IsKeyPressed(KEY_SPACE))                             game.running = true; 

        // Drawing
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{offset - 5, offset - 5, cellSize*cellCount + 10, cellSize*cellCount + 10}, 5, darkGreen);
        DrawText(TextFormat("Score: %04i", game.score), offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("HiScore: %04i", game.hiscore), offset + 480, 20, 40, darkGreen);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}