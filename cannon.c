#include "primlib.h"
#include <stdlib.h>
#include <time.h>

const int GOAL = 10;

const double MAKE_DEGREE = M_PI / 180.0;

const double DELTA_ANGLE = 2.0 * MAKE_DEGREE;

const int BARREL_SPEED = 8;
const int BARREL_LENGTH = 100;
const int BARREL_RADIUS = 75;

const double BULLET_RADIUS_START = 1.0;
const double BULLET_RADIUS_STOP = 10.0;
const int BULLET_SPEED = 15;

const int NUMBER_OF_ENEMIES = 3;
const int ENEMY_RADIUS = 10;
const int ENEMY_SPEED = 3;
const double HIT_DISTANCE = 50.0;

const int ENEMY_MOTHER_RADIUS = 50;

const int WHOLE_RADIUS = ENEMY_MOTHER_RADIUS + ENEMY_RADIUS;

enum scale_t { SCALE_NORMAL = 1, SCALE_2X = 2, SCALE_4X = 4, SCALE_8X = 8 };

enum direction_t { IT_GOES_LEFT, IT_GOES_RIGHT };

enum bullet_state_t { IS_NOT_SHOOTING, IS_SHOOTING, IS_HITTING };

enum enemy_state_t { ENEMY_IS_ALIVE, ENEMY_IS_DEAD };

enum angle_rotation_t { SLOW_SPEED = 4, MEDIUM_SPEED = 5, FAST_SPEED = 8 };

enum sprites_t { FROG, BOW, PIZZA, HEART };

struct barrel_t {
  int barrel_x1;
  int barrel_y1;
  double barrel_x2;
  double barrel_y2;
  double barrel_x3;
  double barrel_y3;
  enum direction_t barrel_direction;
  enum scale_t frog_scale;
  enum scale_t bow_scale;
};

struct bullet_t {
  enum bullet_state_t bullet_state;
  double bullet_radius;
  int bullet_x;
  int bullet_y;
  enum direction_t bullet_direction;
  enum scale_t bullet_scale;
};

struct enemy_mother_t {
  int enemy_mother_x;
  int enemy_mother_y;
  enum direction_t enemy_mother_direction;
};

struct enemy_t {
  int enemy_x;
  int enemy_y;
  enum enemy_state_t enemy_state;
  int enemy_angle;
  int full_rotation;
  enum scale_t enemy_scale;
};

// sprite'y
#define ENEMY_HEIGHT 7
#define ENEMY_WIDTH 9
const int frog[ENEMY_HEIGHT][ENEMY_WIDTH] = {
    {MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, BLUE, BLUE},
    {GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, BLUE, BLUE},
    {GREEN, GREEN, GREEN, GREEN, RED, RED, RED, RED, BLUE},
    {BLUE, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, BLUE},
    {BLUE, GREEN, GREEN, WHITE, BLACK, WHITE, WHITE, BLACK, GREEN},
    {BLUE, BLUE, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN},
    {BLUE, BLUE, BLUE, GREEN, GREEN, BLUE, GREEN, GREEN, BLUE},
};

#define BOW_HEIGHT 11
#define BOW_WIDTH 18
const int bow[BOW_HEIGHT][BOW_WIDTH] = {
    {BLUE, BLUE, BLACK, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLACK, BLACK, BLUE, BLUE},
    {BLUE, BLACK, RED, RED, BLACK, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLACK, BLACK, RED, RED, BLACK, BLUE},
    {BLUE, BLACK, RED, RED, RED, RED, BLACK, BLUE, BLUE, BLUE, BLUE, BLACK, RED, RED, RED, RED, BLACK, BLUE},
    {BLACK, RED, RED, RED, RED, RED, RED, BLACK, BLACK, BLACK, BLACK, RED, RED, RED, RED, RED, RED, BLACK},
    {BLACK, RED, RED, RED, RED, BLACK, RED, BLACK, RED, RED, BLACK, RED, BLACK, RED, RED, RED, RED, BLACK},
    {BLACK, RED, RED, RED, RED, RED, BLACK, BLACK, RED, RED, BLACK, BLACK, RED, RED, RED, RED, RED, BLACK},
    {BLACK, RED, RED, RED, RED, BLACK, RED, BLACK, RED, RED, BLACK, RED, BLACK, RED, RED, RED, RED, BLACK},
    {BLACK, RED, RED, RED, RED, RED, RED, BLACK, BLACK, BLACK, BLACK, RED, RED, RED, RED, RED, RED, BLACK},
    {BLUE, BLACK, RED, RED, RED, RED, BLACK, BLUE, BLUE, BLUE, BLUE, BLACK, RED, RED, RED, RED, BLACK, BLUE},
    {BLUE, BLACK, RED, RED, BLACK, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLACK, BLACK, RED, RED, BLACK, BLUE},
    {BLUE, BLUE, BLACK, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLACK, BLACK, BLUE, BLUE},
};

#define PIZZA_HEIGHT 10
#define PIZZA_WIDTH 9
const int pizza[PIZZA_HEIGHT][PIZZA_WIDTH] = {
    {BLUE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLUE},
    {BLACK, YELLOW, RED, RED, YELLOW, YELLOW, YELLOW, YELLOW, BLACK},
    {BLACK, YELLOW, RED, RED, YELLOW, YELLOW, YELLOW, YELLOW, BLACK},
    {BLUE, BLACK, YELLOW, YELLOW, YELLOW, RED, RED, BLACK, BLUE},
    {BLUE, BLACK, YELLOW, YELLOW, YELLOW, RED, RED, BLACK, BLUE},
    {BLUE, BLUE, BLACK, RED, RED, YELLOW, BLACK, BLUE, BLUE},
    {BLUE, BLUE, BLACK, RED, RED, YELLOW, BLACK, BLUE, BLUE},
    {BLUE, BLUE, BLUE, BLACK, YELLOW, BLACK, BLUE, BLUE, BLUE},
    {BLUE, BLUE, BLUE, BLACK, YELLOW, BLACK, BLUE, BLUE, BLUE},
    {BLUE, BLUE, BLUE, BLUE, BLACK, BLUE, BLUE, BLUE, BLUE},
};

#define HEART_HEIGHT 13
#define HEART_WIDTH 15
const int heart[HEART_HEIGHT][HEART_WIDTH] = {
    {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
    {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLACK, RED, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
    {BLUE, BLUE, BLUE, BLUE, BLUE, BLACK, RED, RED, RED, BLACK, BLUE, BLUE, BLUE, BLUE, BLUE},
    {BLUE, BLUE, BLUE, BLUE, BLACK, RED, RED, RED, RED, RED, BLACK, BLUE, BLUE, BLUE, BLUE},
    {BLUE, BLUE, BLUE, BLACK, RED, RED, RED, RED, RED, RED, RED, BLACK, BLUE, BLUE, BLUE},
    {BLUE, BLUE, BLACK, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLACK, BLUE, BLUE},
    {BLUE, BLACK, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLACK, BLUE},
    {BLACK, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLACK},
    {BLACK, RED, WHITE, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLACK},
    {BLACK, RED, WHITE, WHITE, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, BLACK},
    {BLACK, RED, RED, WHITE, WHITE, RED, RED, BLACK, RED, RED, RED, RED, RED, RED, BLACK},
    {BLUE, BLACK, RED, RED, RED, RED, BLACK, BLUE, BLACK, RED, RED, RED, RED, BLACK, BLUE},
    {BLUE, BLUE, BLACK, BLACK, BLACK, BLACK, BLUE, BLUE, BLUE, BLACK, BLACK, BLACK, BLACK, BLUE, BLUE},
};

// tworzenie i rysowanie elementów
void draw_sprite(int height, int width, const int sprite[height][width], int x,
                 int y, enum direction_t d, int scale) {

  int height_center = height / 2;
  int width_center = width / 2;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (sprite[i][j] != BLUE) {
        if (d == IT_GOES_LEFT) {
          gfx_filledRect((height_center * scale - scale * j + x - scale / 2),
                         (width_center * scale - scale * i + y - scale / 2),
                         (height_center * scale - scale * j + x + scale / 2),
                         (width_center * scale - scale * i + y + scale / 2),
                         sprite[i][j]);

        } else {
          gfx_filledRect(
              (height_center * scale - scale * (width - j) + x - scale / 2),
              (width_center * scale - scale * i + y - scale / 2),
              (height_center * scale - scale * (width - j) + x + scale / 2),
              (width_center * scale - scale * i + y + scale / 2), sprite[i][j]);
        }
      }
    }
  }
}

void set_barrel_props(struct barrel_t *bar) {
  bar->barrel_x1 = gfx_screenWidth() / 2;
  bar->barrel_y1 = gfx_screenHeight();
  bar->barrel_x2 =
      bar->barrel_x1 + BARREL_LENGTH * cos(90.0 * MAKE_DEGREE - DELTA_ANGLE);
  bar->barrel_y2 =
      bar->barrel_y1 - BARREL_LENGTH * sin(90.0 * MAKE_DEGREE - DELTA_ANGLE);
  bar->barrel_x3 =
      bar->barrel_x1 + BARREL_LENGTH * cos(90.0 * MAKE_DEGREE + DELTA_ANGLE);
  bar->barrel_y3 =
      bar->barrel_y1 - BARREL_LENGTH * sin(90.0 * MAKE_DEGREE + DELTA_ANGLE);

  bar->frog_scale = SCALE_8X;
  bar->bow_scale = SCALE_4X;
  bar->barrel_direction = IT_GOES_LEFT;
}

void update_and_draw_barrel_position(struct barrel_t *bar) {
  if (bar->barrel_x1 >= gfx_screenWidth() - BARREL_RADIUS &&
      bar->barrel_x1 <
          gfx_screenWidth()) // ograniczenie poruszania się z prawej strony
    bar->barrel_x1 = gfx_screenWidth() - BARREL_RADIUS;

  if (bar->barrel_x1 <= BARREL_RADIUS &&
      bar->barrel_x1 > 0) // ograniczenie poruszania się z lewej strony
    bar->barrel_x1 = BARREL_RADIUS;

  bar->barrel_x2 =
      bar->barrel_x1 + BARREL_LENGTH * cos(90.0 * MAKE_DEGREE - DELTA_ANGLE);
  bar->barrel_y2 =
      bar->barrel_y1 - BARREL_LENGTH * sin(90.0 * MAKE_DEGREE - DELTA_ANGLE);
  bar->barrel_x3 =
      bar->barrel_x1 + BARREL_LENGTH * cos(90.0 * MAKE_DEGREE + DELTA_ANGLE);
  bar->barrel_y3 =
      bar->barrel_y1 - BARREL_LENGTH * sin(90.0 * MAKE_DEGREE + DELTA_ANGLE);

  gfx_filledTriangle(bar->barrel_x1, bar->barrel_y1, bar->barrel_x2,
                     bar->barrel_y2, bar->barrel_x3, bar->barrel_y3,
                     YELLOW); // lufa działa
  gfx_filledCircle(bar->barrel_x1, bar->barrel_y1, BARREL_RADIUS,
                   YELLOW); // działo

  draw_sprite(ENEMY_HEIGHT, ENEMY_WIDTH, frog, bar->barrel_x1 + ENEMY_WIDTH,
              bar->barrel_y1 - BARREL_RADIUS + bar->bow_scale * BOW_HEIGHT - 5,
              bar->barrel_direction, bar->frog_scale); // pani pepe

  draw_sprite(BOW_HEIGHT, BOW_WIDTH, bow, bar->barrel_x1 + BOW_WIDTH,
              bar->barrel_y1 - BARREL_RADIUS, bar->barrel_direction,
              bar->bow_scale); // kokarda
}

void set_bullet_props(struct bullet_t *b) {
  b->bullet_y = gfx_screenHeight();
  b->bullet_state = IS_NOT_SHOOTING;
  b->bullet_radius = BULLET_RADIUS_START;
  b->bullet_scale = SCALE_4X;
}

void resurrect_enemy(struct enemy_mother_t *enemy_mother,
                     struct enemy_t *enemy) {
  if (enemy_mother->enemy_mother_x > gfx_screenWidth() ||
      enemy_mother->enemy_mother_x < 0) {
    if (enemy->enemy_state == ENEMY_IS_DEAD) {
      enemy_mother->enemy_mother_x =
          ((rand() % (gfx_screenWidth() - WHOLE_RADIUS)) + WHOLE_RADIUS);
      enemy_mother->enemy_mother_y =
          ((rand() % (gfx_screenWidth() / 4 - (WHOLE_RADIUS + ENEMY_HEIGHT))) +
           WHOLE_RADIUS);
      enemy->enemy_angle = (rand() % 720);
    }
    enemy->enemy_state = ENEMY_IS_ALIVE;
  }
}

void create_enemy(struct enemy_mother_t *enemy_mother, struct enemy_t *enemy,
                  int angle) {
  enemy->enemy_state = ENEMY_IS_ALIVE;
  enemy_mother->enemy_mother_x =
      ((rand() % (gfx_screenWidth() - WHOLE_RADIUS)) + WHOLE_RADIUS);
  enemy_mother->enemy_mother_y =
      ((rand() % (gfx_screenHeight() / 4 - WHOLE_RADIUS)) + WHOLE_RADIUS);
  enemy_mother->enemy_mother_direction = (rand() % 2);

  resurrect_enemy(enemy_mother, enemy);

  enemy->enemy_angle = angle;
  enemy->full_rotation = angle;
  enemy->enemy_scale = SCALE_4X;
}

void create_scoreboard(int *p, int g) {
	  char str_points[20];
    char str_goal[10];

    sprintf(str_points, "happy pepes: %d", *p); 
    gfx_textout(10, 10, str_points, WHITE);

    sprintf(str_goal, "goal: %d", g);
    gfx_textout(10, 40, str_goal, WHITE);
}

// aktualizowanie ruchu
void update_bullet_position(struct bullet_t *b, struct barrel_t bar) {

  if ((b->bullet_x < 0) || (b->bullet_y < 0) ||
      (b->bullet_x >= gfx_screenWidth()) ||
      (b->bullet_y >= gfx_screenHeight())) {
    b->bullet_state = IS_NOT_SHOOTING;
    b->bullet_y = bar.barrel_y1 - BARREL_LENGTH - BULLET_RADIUS_START;
  }
}

void check_bullet_state(struct bullet_t *b) {

  if (b->bullet_state == IS_NOT_SHOOTING) {
    b->bullet_y = gfx_screenHeight();
  }

  if (b->bullet_state == IS_SHOOTING) {
    b->bullet_y -= BULLET_SPEED;
    draw_sprite(PIZZA_HEIGHT, PIZZA_WIDTH, pizza, b->bullet_x, b->bullet_y,
                b->bullet_direction, b->bullet_scale);
  }

  if (b->bullet_state == IS_HITTING) {
    b->bullet_radius += .5;

    draw_sprite(HEART_HEIGHT, HEART_WIDTH, heart, b->bullet_x, b->bullet_y,
                b->bullet_direction, b->bullet_radius);

    if (b->bullet_radius >= BULLET_RADIUS_STOP) {
      b->bullet_state = IS_NOT_SHOOTING;
      b->bullet_radius = BULLET_RADIUS_START;
      b->bullet_y = gfx_screenHeight();
    }
  }
}

void update_enemy_position(struct enemy_mother_t *enemy_mother,
                           struct enemy_t *enemy, int a_rotation) {
  if (enemy_mother->enemy_mother_direction == IT_GOES_RIGHT)
    enemy->enemy_angle += a_rotation;
  else
    enemy->enemy_angle -= a_rotation;

  if (enemy_mother->enemy_mother_direction && enemy->enemy_angle % 720 == 0)
    enemy->enemy_angle = 0;
  if (!enemy_mother->enemy_mother_direction && !enemy->enemy_angle)
    enemy->enemy_angle = 720;

  if (enemy->enemy_x > (gfx_screenWidth() - 2 * ENEMY_RADIUS))
    enemy_mother->enemy_mother_direction = IT_GOES_LEFT;
  if (enemy->enemy_x < 2 * ENEMY_RADIUS)
    enemy_mother->enemy_mother_direction = IT_GOES_RIGHT;

  enemy->full_rotation = enemy->enemy_angle / 180;
  if (enemy->full_rotation % 4) {
    enemy->enemy_y =
        enemy_mother->enemy_mother_y +
        (ENEMY_MOTHER_RADIUS * sin(MAKE_DEGREE * enemy->enemy_angle));
    enemy->enemy_x =
        enemy_mother->enemy_mother_x +
        (ENEMY_MOTHER_RADIUS * cos(MAKE_DEGREE * enemy->enemy_angle));
  } else {
    enemy->enemy_y = enemy_mother->enemy_mother_y;
    enemy->enemy_x =
        enemy_mother->enemy_mother_x +
        (ENEMY_MOTHER_RADIUS * cos(MAKE_DEGREE * enemy->enemy_angle));
    enemy->enemy_x +=
        enemy_mother->enemy_mother_direction ? ENEMY_SPEED : -ENEMY_SPEED;
  }
}

// wykrywanie klawiszy i trafień
void detect_hit(struct enemy_t *enemy, struct bullet_t *b, int *point) {
  if (hypot((b->bullet_x - enemy->enemy_x), (b->bullet_y - enemy->enemy_y)) <=
          HIT_DISTANCE &&
      (enemy->enemy_state == ENEMY_IS_ALIVE) &&
      (b->bullet_state != IS_HITTING)) {
    enemy->enemy_state = ENEMY_IS_DEAD;
    b->bullet_state = IS_HITTING;
    *point += 1;
  }
}

void key_detection(struct bullet_t *b, struct barrel_t *bar) {
  if (gfx_isKeyDown(SDLK_SPACE)) {
    if (b->bullet_state == IS_NOT_SHOOTING) {
      b->bullet_x = bar->barrel_x1;
      b->bullet_state = IS_SHOOTING;
    }
  }

  if (gfx_isKeyDown(SDLK_RIGHT)) {
    bar->barrel_x1 += BARREL_SPEED;
    bar->barrel_direction = IT_GOES_RIGHT;
    b->bullet_direction = IT_GOES_RIGHT;
  }

  if (gfx_isKeyDown(SDLK_LEFT)) {
    bar->barrel_x1 -= BARREL_SPEED;
    bar->barrel_direction = IT_GOES_LEFT;
    b->bullet_direction = IT_GOES_LEFT;
  }
}

// mapka na koniec 
void final_pepe() {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLUE);
		
    draw_sprite(ENEMY_HEIGHT, ENEMY_WIDTH, frog,
                gfx_screenWidth()/2-ENEMY_WIDTH/2,
                gfx_screenHeight()/2-ENEMY_HEIGHT/2,
                IT_GOES_RIGHT, SCALE_8X);
}
void the_end(int *c1, int *c2) {
		int x = 0;
    final_pepe();
    gfx_updateScreen();
    SDL_Delay(2000);
		
    while(x<275) {
      x+=1;
      final_pepe();

      draw_sprite(HEART_HEIGHT, HEART_WIDTH, heart,
                  gfx_screenWidth()/2+HEART_WIDTH,
                  gfx_screenHeight()/2-HEART_HEIGHT,
                  IT_GOES_RIGHT, x);
      gfx_updateScreen();
      SDL_Delay(10);
    }
	  
  while (gfx_pollkey()!=SDLK_ESCAPE) {
	  *c1 = (rand() % 8);
	  *c2 = (rand() % 8);
	  while (1) {
		  if (*c1 == *c2)
			  *c2 = (rand() % 8);
		  else
	      break;
	  }
		
	  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, *c1);

	  draw_sprite(HEART_HEIGHT, HEART_WIDTH, heart, gfx_screenWidth() / 4,
                gfx_screenHeight() / 2, IT_GOES_RIGHT, SCALE_8X);
    draw_sprite(HEART_HEIGHT, HEART_WIDTH, heart, 3 * gfx_screenWidth() / 4,
							  gfx_screenHeight() / 2, IT_GOES_LEFT, SCALE_8X);
		
    gfx_textout(gfx_screenWidth() / 2 - 75, gfx_screenHeight() / 2,
                "All pepes are happy!", *c2);
    gfx_textout(gfx_screenWidth() / 2 - 75, gfx_screenHeight() / 2 + 30,
                "Press ESC key to exit", *c2);
		
    gfx_updateScreen();

    SDL_Delay(700);
	}
}

//------
int main() {
  if (gfx_init())
    exit(3);

  srand(time(0));

  // działo
  struct barrel_t barrel;
  set_barrel_props(&barrel);

  // pocisk
  struct bullet_t bullet;
  set_bullet_props(&bullet);

  // przeciwnicy
  enum angle_rotation_t angle_rotation = FAST_SPEED; // prędkość przeciwników
  struct enemy_mother_t
      enemies_mothers[NUMBER_OF_ENEMIES];    // tablica z przeciwnikami-matkami
  struct enemy_t enemies[NUMBER_OF_ENEMIES]; // tablica z przeciwnikami
  for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
    create_enemy(&enemies_mothers[i], &enemies[i], 90 * i);
  }

  //punkty i kolory
  int points = 0;

  int rand_col1;
  int rand_col2;

  while (points < GOAL) {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1,
                   BLUE); // tło

    update_and_draw_barrel_position(&barrel);

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
      enemies_mothers[i].enemy_mother_x +=
          enemies_mothers[i].enemy_mother_direction
              ? ENEMY_SPEED
              : -ENEMY_SPEED; // kierunek przeciwnika-matki

      resurrect_enemy(&enemies_mothers[i],
                      &enemies[i]); // pojawianie się przeciwnika w losowych
                                    // miejscach po śmierci

      if (enemies[i].enemy_state == ENEMY_IS_ALIVE) {
        update_enemy_position(&enemies_mothers[i], &enemies[i], angle_rotation);
        draw_sprite(ENEMY_HEIGHT, ENEMY_WIDTH, frog, enemies[i].enemy_x,
                    enemies[i].enemy_y,
                    enemies_mothers[i].enemy_mother_direction,
                    enemies[i].enemy_scale);
      }
      detect_hit(&enemies[i], &bullet, &points);
    }

    check_bullet_state(&bullet);
    update_bullet_position(&bullet, barrel);
		
		create_scoreboard(&points, GOAL);

    gfx_updateScreen();

		key_detection(&bullet, &barrel);

    SDL_Delay(10);
  }

	the_end(&rand_col1, &rand_col2);

  return 0;
}
