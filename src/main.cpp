// -*- coding: utf-8 -*-
#include <iostream>
#include <deque>
#include <random>
#include <conio.h>      // kbhit, getch
#include <windows.h>    // Sleep
#include <algorithm>

struct Coord { int x{}, y{}; };
enum class Dir { Stop, Up, Down, Left, Right };

static const int WIDTH  = 32;
static const int HEIGHT = 18;

class SnakeGame {
public:
    SnakeGame() { reset(); }

    void reset() {
        gameOver = false; paused = false; score = 0; speedMs = 150;
        snake.clear();
        dir = Dir::Right;
        int cx = WIDTH / 2, cy = HEIGHT / 2;
        snake.push_back({cx, cy});
        snake.push_back({cx-1, cy});
        snake.push_back({cx-2, cy});
        placeFruit();
    }

    void run() {
        while (true) {
            draw();
            if (gameOver) {
                std::cout << "\nGAME OVER! Score: " << score
                          << "\nPress R to restart or Q to quit.\n";
                while (true) {
                    int ch = waitKey();
                    if (ch == 'q' || ch == 'Q') return;
                    if (ch == 'r' || ch == 'R') { reset(); break; }
                }
            } else {
                input();
                if (!paused) { logic(); Sleep(speedMs); }
                else { Sleep(30); }
            }
        }
    }

private:
    std::deque<Coord> snake;
    Coord fruit{};
    Dir dir{Dir::Right};
    bool gameOver{false};
    bool paused{false};
    int score{0};
    int speedMs{150};

    static int waitKey() {
        while (!_kbhit()) Sleep(10);
        return _getch();
    }

    void draw() {
        // clear console
        system("cls");
        // top border
        for (int i = 0; i < WIDTH + 2; ++i) std::cout << '#';
        std::cout << '\n';
        for (int y = 0; y < HEIGHT; ++y) {
            std::cout << '#';
            for (int x = 0; x < WIDTH; ++x) {
                char c = ' ';
                if (x == fruit.x && y == fruit.y) c = '*';
                for (size_t i = 0; i < snake.size(); ++i) {
                    if (snake[i].x == x && snake[i].y == y) {
                        c = (i == 0) ? 'O' : 'o';
                        break;
                    }
                }
                std::cout << c;
            }
            std::cout << "#\n";
        }
        for (int i = 0; i < WIDTH + 2; ++i) std::cout << '#';
        std::cout << "\nScore: " << score
                  << "   Speed: " << (200 - speedMs)
                  << (paused ? "   [PAUSED]" : "")
                  << "\nControls: arrows/WASD = move, P = pause, Q = quit\n";
    }

    bool cellOccupied(int x, int y, size_t upto) const {
        for (size_t i = 0; i < upto; ++i)
            if (snake[i].x == x && snake[i].y == y) return true;
        return false;
    }

    void placeFruit() {
        static std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dx(0, WIDTH-1);
        std::uniform_int_distribution<int> dy(0, HEIGHT-1);
        do { fruit = {dx(rng), dy(rng)}; } while (cellOccupied(fruit.x, fruit.y, snake.size()));
    }

    void input() {
        if (!_kbhit()) return;
        int ch = _getch();
        if (ch == 224 || ch == 0) { // arrow keys
            ch = _getch();
            switch (ch) {
                case 72: if (dir != Dir::Down)  dir = Dir::Up;    break;
                case 80: if (dir != Dir::Up)    dir = Dir::Down;  break;
                case 75: if (dir != Dir::Right) dir = Dir::Left;  break;
                case 77: if (dir != Dir::Left)  dir = Dir::Right; break;
            }
        } else {
            switch (ch) {
                case 'w': case 'W': if (dir != Dir::Down)  dir = Dir::Up;    break;
                case 's': case 'S': if (dir != Dir::Up)    dir = Dir::Down;  break;
                case 'a': case 'A': if (dir != Dir::Right) dir = Dir::Left;  break;
                case 'd': case 'D': if (dir != Dir::Left)  dir = Dir::Right; break;
                case 'p': case 'P': paused = !paused; break;
                case 'q': case 'Q': gameOver = true; break;
            }
        }
    }

    void logic() {
        Coord head = snake.front();
        switch (dir) {
            case Dir::Up:    head.y--; break;
            case Dir::Down:  head.y++; break;
            case Dir::Left:  head.x--; break;
            case Dir::Right: head.x++; break;
            default: break;
        }
        // walls
        if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
            gameOver = true; return;
        }
        // will we grow?
        bool willGrow = (head.x == fruit.x && head.y == fruit.y);
        // self collision (ignore tail if not growing)
        size_t upto = snake.size() - (willGrow ? 0 : 1);
        if (cellOccupied(head.x, head.y, upto)) { gameOver = true; return; }

        // move
        snake.push_front(head);
        if (willGrow) {
            score += 10;
            if (speedMs > 60) speedMs -= 5;
            placeFruit();
        } else {
            snake.pop_back();
        }
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
