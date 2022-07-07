#include <keypadc.h>
#include <string.h>
#include <ti/screen.h>
#include <ti/getcsc.h>  /*  Included for the loading screen  */
#include <sys/timers.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/util.h>
#include "funcs.h"

void initPlayer(struct Player *p) {
  p->x = 0;
  p->y = 0;
  p->hp = 5;
  p->ep = 5;
  p->dir = Up;
  p->captures = 0;
}
void initMap(struct Map *m) {
  m->xmin = 0;
  m->ymin = 0;
  m->xmax = 19;
  m->ymax = 9;
  m->xpad = 6;
  memset(m->map[0], 0, 20*10*sizeof(int));
}

void initCreature(struct Creature *c) {
  c->x = 0;
  c->y = 0;
  c->id = 0;
}

void screenSetup() {
  for(int i=0;i<10;i++) {
    os_SetCursorPos(i, 0);
    os_PutStrLine("     I");
  }
  os_SetCursorPos(0, 0);
  os_PutStrLine("HP:  ");
  os_SetCursorPos(1, 0);
  os_PutStrLine("EP:  ");
  os_SetCursorPos(2, 0);
  os_PutStrLine("Capt:");
}

bool canCatchCreature(struct Player* p, struct Creature* c, struct Map *m) {
  switch(p->dir) {
  case Up:
    return ((p->y != 0) && p->x == c->x && p->y-1 == c->y);

  case Down:
    return ((p->y != m->ymax) && p->x == c->x && p->y+1 == c->y);

  case Left:
    return ((p->x != 0) && p->y == c->y && p->x-1 == c->x);

  case Right:
    return ((p->x != m->xmax) && p->y == c->y && p->x+1 == c->x);

  default:
    return false;
  }
}

void generateNewCreature(struct Player* p, struct Map* m, struct Creature* c, struct Creature* e) {
  do {
    c->x = randInt(0, m->xmax);
    c->y = randInt(0, m->ymax);
  } while(!empty(m->map[c->y][c->x]) || (p->x == c->x && p->y == c->y) || (p->x == e->x && p->y == e->y));
}

void generateNewCreatureSimple(struct Player* p, struct Creature* c, struct Map* m) {

  do {
    c->x = randInt(0, m->xmax);
    c->y = randInt(0, m->ymax);
  } while(!empty(m->map[c->y][c->x]) || (p->x == c->x && p->y == c->y));
}

bool empty(int val) {
  switch(val) {
  case 0:  /*  all values that are considered empty go here */
    return true;
  default:
    return false;
  }
}

bool blockedSpace(struct Creature* c, char y, char x) {
  return (c->x == x && c->y == y);
}
