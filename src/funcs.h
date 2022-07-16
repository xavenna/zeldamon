#ifndef FUNCS_H
#define FUNCS_H

#include "charset.h"

#define SWORD_MAX_DURABILITY 5
#define BOW_MAX_DURABILITY 5

#define xv_Pause() while(os_GetCSC() != sk_Enter)
#define xv_MoveCursorAndPrint(st, y, x) \
  os_SetCursorPos(y, x);		\
  os_PutStrFull(st)

enum Direction {Up, Down, Left, Right, Null=-1};

struct Player {
  uint8_t x;
  uint8_t y;
  uint8_t hp;
  uint8_t ep;
  uint8_t sw;
  uint8_t bow;
  uint8_t key;
  uint8_t coin;
  int captures;
  enum Direction dir;
};

struct Map {
  uint8_t xmin;
  uint8_t xmax;
  uint8_t ymin;
  uint8_t ymax;
  uint8_t xpad;
  uint8_t map[10][20];
  uint8_t sub[10][20];
};
#define MAP_WIDTH 20
#define MAP_HEIGHT 10

struct Creature {
  uint8_t x;
  uint8_t y;
  uint8_t id;
};

enum Target {Random, TowardPlayer, ChasePlayerSmart, NoTarget};

struct Enemy {
  uint8_t x;
  uint8_t y;
  uint8_t ox;
  uint8_t oy;
  uint8_t id;
  enum Target targetMode;
  bool active;
};
#define ENEMY_LIMIT 5
void initPlayer(struct Player*);
void initMap(struct Map*);
void initCreature(struct Creature*);
void initEnemy(struct Enemy*);
void screenSetup();
bool canCatchCreature(struct Player*, struct Creature*, struct Map*);
void generateNewCreature(struct Player*, struct Map*, struct Creature*, struct Creature*);
void generateNewCreatureSimple(struct Player*, struct Creature*, struct Map*);
bool blockedSpace(struct Creature*, char, char);
bool eBlockedSpace(struct Enemy*, char, char);
bool enemyBlock(struct Enemy*, char, char);
bool empty(uint8_t, enum Direction);
bool enemyCanMove(uint8_t, enum Direction);
bool creatureSpawn(uint8_t);
void printNCharsOfInt(int24_t, size_t, char, char);
void mapDraw(struct Map*, bool, char, char, char);
void newMapDraw(struct Map*, bool, int*, int*);
int facing(struct Player*, struct Map*);
int fsx(struct Player*, struct Map*);
int fsy(struct Player*, struct Map*);
void handleFacingSpace(struct Player*, struct Map*, int);
#endif
