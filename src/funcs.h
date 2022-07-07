#ifndef FUNCS_H
#define FUNCS_H

enum Direction {Up, Down, Left, Right, Null=-1};
struct Player {
  uint8_t x;
  uint8_t y;
  uint8_t hp;
  uint8_t ep;
  int captures;
  enum Direction dir;
};

struct Map {
  uint8_t xmin;
  uint8_t xmax;
  uint8_t ymin;
  uint8_t ymax;
  uint8_t xpad;
  int map[10][20];
};

struct Creature {
  uint8_t x;
  uint8_t y;
  uint8_t id;
};
void initPlayer(struct Player *p);
void initMap(struct Map *m);
void initCreature(struct Creature *c);
void screenSetup();
bool canCatchCreature(struct Player*, struct Creature*, struct Map*);
void generateNewCreature(struct Player*, struct Map*, struct Creature*, struct Creature*);
void generateNewCreatureSimple(struct Player*, struct Creature*, struct Map*);
bool blockedSpace(struct Creature*, char, char);
bool empty(int);
#endif
