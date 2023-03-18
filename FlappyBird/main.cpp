#include <graphics.h>
#include <time.h>
#include <conio.h>
#include <iostream>
#include <mmsystem.h>
#pragma comment (lib, "Winmm.lib")
using namespace std;

#define WIDTH 700
#define HEIGHT 800
#define G 0.6

int score;
float obstacle_height = 260;

void initialize_game();
void drawMap();
void bird_movement();
void obstacle_movement();
void judge();
//create a bird's structure
struct bird {

    float x, y, y_velocity;
    float r = 40;

};
bird bird1;

//create obstacle's structure
struct obstacles {

    float x1, x2;
    float y1, y2;
    float x_velocity;

};
obstacles obstacle_upper;
obstacles obstacle_lower;


int main() {

    initgraph(WIDTH, HEIGHT);
    initialize_game();
    while (1) {
        
        bird_movement();
        obstacle_movement();
        cleardevice();
        drawMap();
        judge();
        Sleep(10); //let the bird stay for 10ms or else the while loop will instanty refreshes it

    }
    closegraph();

}

void initialize_game() {

    bird1.x = WIDTH / 4;
    bird1.y = HEIGHT / 2; //let the ball starts at y = 0
    bird1.y_velocity = 0;

    obstacle_upper.x1 = WIDTH * 3 / 4;
    obstacle_upper.y1 = 0; //the obstacle is at the top of the window
    obstacle_upper.x2 = obstacle_upper.x1 + 60; //meaning the width is 30 pixel
    obstacle_upper.y2 = obstacle_upper.y1 + obstacle_height; //obstacle height set to 130 pixel

    obstacle_lower.x1 = WIDTH * 3 / 4;
    obstacle_lower.y1 = HEIGHT - obstacle_height;
    obstacle_lower.x2 = obstacle_lower.x1 + 60;
    obstacle_lower.y2 = HEIGHT; //the lower obstacle stick to the ground

    //initial velocity of the movement of obstacles
    obstacle_upper.x_velocity = obstacle_lower.x_velocity = -3;

}

void drawMap() {

    fillcircle(bird1.x, bird1.y, bird1.r);
    fillrectangle(obstacle_upper.x1, obstacle_upper.y1, obstacle_upper.x2, obstacle_upper.y2);
    fillrectangle(obstacle_lower.x1, obstacle_lower.y1, obstacle_lower.x2, obstacle_lower.y2);

}

void bird_movement() {
    //when a key is hit it detects if its spacebar to trigger the bird to jump
    if (_kbhit()) {
        char input = _getch();
        if (input == ' ') {
            PlaySound(TEXT("wing.wav"), NULL, SND_ASYNC);
            bird1.y_velocity = -17;
        }
    }

    bird1.y_velocity += G;
    bird1.y += bird1.y_velocity; //changes bird's vertical position according to velocity + gravity
}

void obstacle_movement() {
    obstacle_upper.x1 += obstacle_upper.x_velocity;
    obstacle_upper.x2 += obstacle_upper.x_velocity;
    obstacle_lower.x1 += obstacle_lower.x_velocity;
    obstacle_lower.x2 += obstacle_lower.x_velocity;

    //when the obstacle touches the left most of the screen
    if (obstacle_upper.x1 <= 0) { // we reset their position back to the right and score counte +1
        obstacle_upper.x1 = obstacle_lower.x1 = WIDTH - 60;
        obstacle_upper.x2 = obstacle_lower.x2 = WIDTH;
        PlaySound(TEXT("point.wav"), NULL, SND_ASYNC);
        score += 1;
        srand(time(NULL));
        obstacle_height = rand() % int(HEIGHT / 8) + HEIGHT/4 ; //randomize the gap between the obstacles and maintain at 1/4 of the height
        obstacle_upper.y2 = obstacle_height;
        obstacle_lower.y1 = HEIGHT - obstacle_height;
    }
    if (score > 0) {
        obstacle_upper.x_velocity = obstacle_lower.x_velocity -= 0.01; //increases the speed of the movmenet of obstacles
    }
}

void judge() {

    if ((bird1.x + bird1.r >= obstacle_upper.x1 && bird1.y - bird1.r <= obstacle_height && bird1.x - bird1.r <= obstacle_upper.x2) //touch the upper block
        || bird1.x + bird1.r >= obstacle_lower.x1 && bird1.y + bird1.r >= HEIGHT - obstacle_height && bird1.x - bird1.r  <= obstacle_lower.x2 ) {
        PlaySound(TEXT("hit.wav"), NULL, SND_ASYNC);
        Sleep(100); //provide a deathrecap
        score = 0;
        MessageBox(GetHWnd(), L"You failed the challenge!", L"Failed", MB_OK);
        exit(0);
    }
    else if (bird1.y >= HEIGHT - bird1.r) { //if the bird hits the ground
        PlaySound(TEXT("die.wav"), NULL, SND_ASYNC);
        bird1.y_velocity = 0;
        bird1.y = HEIGHT - bird1.r;
        MessageBox(GetHWnd(), L"You failed the challenge!", L"Failed", MB_OK);
        exit(0);

    }

    if (score == 8) {
        MessageBox(GetHWnd(), L"You completed the challenge!", L"Victory", MB_OK);
        exit(0);
    }

}