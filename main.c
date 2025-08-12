#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_LENGTH 100
#define INITIAL_SPEED 200

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

struct Point {
    int x, y;
};

struct Snake snake;
struct Point food, powerup;

int score = 0, highscore = 0;
int game_over = 0;
int speed = INITIAL_SPEED;
int powerup_timer = 0;

void load_highscore() {
    FILE *file = fopen("Highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highscore);
        fclose(file);
    } else {
        printf("There was an error opening the file.");
        return;
    }
}

void save_highscore() {
    FILE *file = fopen("Highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highscore);
        fclose(file);
    } else {
        printf("There was an error opening the file.");
        return;
    }
}
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE; 
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void clearScreen() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

void draw(){
  system("cls");
  for(int y=0; y <= HEIGHT; y++){
    for(int x=0; x<= WIDTH; x++){
      if(x==0 || x==WIDTH || y==0 || y==HEIGHT){
        printf("#");
      }
      else if(x == food.x && y == food.y){
        printf("F");
      }
      else if(x == powerup.x && y == powerup.y){
        printf("P");
      }
      else{
        int is_snake = 0;
        for(int i = 0; i < snake.length; i++){
          if(snake.body[i].x == x && snake.body[i].y == y){
            if(i==0) printf("O");
            else printf("o");
            is_snake = 1;
            break;
          }
        }
        if(is_snake == 0){
          printf(" ");
        }
      }
    }
    printf("\n"); // Moved outside the inner loop
  }
  printf("Score: %d  Highscore: %d\n", score, highscore);
}


void input(gamestate* game){
    if(_kbhit()){
        switch(_getch()){
        case 'w': if(game->dir!=DOWN) game->dir=UP; break;
        case 'a': if(game->dir!=RIGHT) game->dir=LEFT; break;
        case 's': if(game->dir!=UP) game->dir=DOWN; break;
        case 'd': if(game->dir!=LEFT) game->dir=RIGHT; break;
        }
    }
}

void logic(gamestate* game){
    for (int i=game->length-1;i>0;i--){
        game->body[i] = game->body[i-1];
    }
    if (game->length>0){game->body[0]=game->head;}

    switch(game->dir){
    case UP: game->head.y--; break;
    case LEFT: game->head.x--; break;
    case DOWN: game->head.y++; break;
    case RIGHT: game->head.x++; break;}

    if(game->head.x <= 0 || game->head.x >= WIDTH-1 || game->head.y <0 || game->head.y >= HEIGHT){
        game->gameover = true;
    }

}

int main() {
    gamestate game;
    setup(&game);

    while (!game.gameover) {
        draw(&game);
        input(&game);
        logic(&game);
        Sleep(100);
    }

    return 0;
}
