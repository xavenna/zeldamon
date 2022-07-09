#include <keypadc.h>
#include <string.h>
#include <ti/screen.h>
#include <ti/getcsc.h>  /*  Included for the loading screen  */
#include <ti/real.h>
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
  memset(m->map[0], 0, 20*10*sizeof(uint8_t));
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
  } while(!creatureSpawn(m->map[c->y][c->x]) || (p->x == c->x && p->y == c->y) || (p->x == e->x && p->y == e->y));
}

void generateNewCreatureSimple(struct Player* p, struct Creature* c, struct Map* m) {

  do {
    c->x = randInt(0, m->xmax);
    c->y = randInt(0, m->ymax);
  } while(!creatureSpawn(m->map[c->y][c->x]) || (p->x == c->x && p->y == c->y));
}

bool empty(uint8_t val) {
  /*  can a player move through here?  */
  switch(val) {
  case 0xdd:
  case 0xbe:
  case 0xbf:
  case 0xcd:
  case 0x80:
  case 0xca:
  case 0x7f:
  case 0xba:
  case 0xde:
  case 0x0c:
  case 0xf6:
  case 0xf8:
  case 0xce:
    return false;
  default:
    return true;
  }
}
bool creatureSpawn(uint8_t val) {
  /*  determines whether a creature can be spawned in on target space  */
  switch(val) {
  case '<':
  case '>':
  case '^':
  case 'v':
  case '@':
  case '-':
  case '|':
  case 0xdd:
  case 0xbe:
  case 0xbf:
  case 0xcd:
  case 0xf0:
  case 0x80:
  case 0xca:
  case 0x09:
  case 0x7f:
  case 0xba:
  case 0xde:
  case 0x0c:
  case 0xf6:
  case 0xf8:
  case 0xce:
  case 0xb8:
    return false;
  default:
    return true;
  }
}

bool blockedSpace(struct Creature* c, char y, char x) {
  return (c->x == x && c->y == y);
}

void printNCharsOfInt(int24_t num, size_t len, char y, char x) {  /*  Only prints up to 12 chars  */
  char temp[12];
  char temp2[12];
  const real_t t = os_Int24ToReal(num);
  os_RealToStr(temp, &(t), 6, 1, -1);
  strncpy(temp2, temp, len+1);
  os_SetCursorPos(y, x);
  os_PutStrLine(temp2);
}

void mapDraw(struct Map* m, bool f, char o1, char o2, char o3) {
  char mapBuf[21];
  mapBuf[20] = '\0';
  for(int i=0;i<10;i++) {
    if(!(o1 == i || o2 == i || o3 == i) && !f)
      continue;
    memcpy(mapBuf, (char*) m->map[i], 20);
    os_SetCursorPos(i, m->xpad);
    os_PutStrLine(mapBuf);
  }
}


