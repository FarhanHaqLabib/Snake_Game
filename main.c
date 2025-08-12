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
int x,y;
};

struct Snake {
struct Point body[MAX_LENGTH];
int length;
Direction dir;
};

struct Snake snake;
struct Point food,powerup;

int score = 0, highscore = 0;
int game_over = 0;
int speed = INITIAL_SPEED;
int powerup_timer = 0;

void load_highscore() {
  FILE *file = fopen("Highscore.txt", "r");
  if (file != NULL){
    fscanf(file, "%d", &highscore);
    fclose(file);
  }
  else{
    printf("There was an error opening the file.");
    return;
  }
}

void save_highscore(){
  FILE *file = fopen("Highscore.txt", "w");
  if (file != NULL){
    fprintf(file, "%d", highscore);
    fclose(file);
  }
  else{
    printf("There was an error opening the file.");
    return;
  }
}
void clear_screen() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

/*---------------------------------------------------------------------------------*/

void setup(){
  clear_screen();
  snake.length = 3;
  snake.body[0].x = WIDTH / 2;
  snake.body[0].y = HEIGHT / 2;
  snake.dir = RIGHT;

  for(int i = 1; i < snake.length; i++){
    snake.body[i].x = snake.body[0].x - i;
    snake.body[i].y = snake.body[0].y;
  }

  food.x = rand() % WIDTH;
  food.y = rand() % HEIGHT;

  powerup.x = rand() % WIDTH;
  powerup.y = rand() % HEIGHT;

  load_highscore();
}

void input(){
  if(_kbhit()){
    char key = _getch();
    switch(key){

      case 'w':
      case 'W':
      if(snake.dir != DOWN){
        snake.dir = UP;
      }
      break;

      case 'a':
      case 'A':
      if(snake.dir != RIGHT){
        snake.dir = LEFT;
      }
      break;

      case 'd':
      case 'D':
      if(snake.dir != LEFT){
        snake.dir = RIGHT;
      }
      break;

      case 's':
      case 'S':
      if(snake.dir != UP){
        snake.dir = DOWN;
      }
      break;
    }
  }
}

/*---------------------------------------------------------------------------------*/

void logic(){
  struct Point prev = snake.body[0];
  struct Point temp;

  switch(snake.dir){
    case UP:
    snake.body[0].y--;
    break;

    case LEFT:
    snake.body[0].x--;
    break;

    case RIGHT:
    snake.body[0].x++;
    break;

    case DOWN:
    snake.body[0].y++;
    break;
  }

  for(int i = 1; i < snake.length; i++){
    temp = snake.body[i];
    snake.body[i] = prev;
    prev = temp;
  }

  if(snake.body[0].x <= 0 || snake.body[0].x >= WIDTH || snake.body[0].y <= 0 || snake.body[0].y >= HEIGHT){ 
    game_over = 1;
    return;
  }

  for(int i = 1; i < snake.length; i++){
    if(snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y){
      game_over = 1;
      return;
    }
  }

  if(snake.body[0].x == food.x && snake.body[0].y == food.y){ 
    score += 10;
    snake.length++;

    if(snake.length < MAX_LENGTH){
      snake.body[snake.length-1] = snake.body[snake.length-2];
    }

    food.x = rand() % (WIDTH-2) + 1; 
    food.y = rand() % (HEIGHT-2) + 1;

    if(speed > 50){
      speed -= 5;
    }
  }

    if(snake.body[0].x == powerup.x && snake.body[0].y == powerup.y){
      int type = rand() % 2;
      if(type == 0){
        speed -= 20;
      }
      else{
        speed += 20;
      }

      powerup_timer = 30;
      powerup.x = rand() % (WIDTH - 2) + 1;
      powerup.y = rand() % (HEIGHT - 2) + 1;

      if(powerup_timer > 0){
        powerup_timer--;
      }
    }
}

/*---------------------------------------------------------------------------------*/

void draw(){
  clear_screen();
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
    printf("\n");
  }
  printf("Score: %d  Highscore: %d\n", score, highscore);
}

/*---------------------------------------------------------------------------------*/

int main() {
    srand(time(NULL));
    hideCursor(); 
    setup();



  while(game_over == 0){
    draw();
    input();
    logic();
    Sleep(speed);
  }
  if (score > highscore) {
        highscore = score;
        save_highscore();
    }

    printf("\nGame Over! Final Score: %d\n", score);
    printf("Press any key to exit...\n");
    _getch();
    return 0;
}
