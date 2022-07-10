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
  p->sw = 0;
  p->bow = 0;
  p->key = 0;
  p->coin = 0;
  p->dir = Up;
  p->captures = 0;
}
void initMap(struct Map *m) {
  m->xmin = 0;
  m->ymin = 0;
  m->xmax = 19;
  m->ymax = 9;
  m->xpad = 6;
  //memset(m->map[0], 0, 20*10*sizeof(uint8_t)); //this isn't needed b/c it's loaded from appvar
  //memset(m->sub[0], 0, 20*10*sizeof(uint8_t));
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
  os_PutStrLine("GP:  ");
  os_SetCursorPos(3, 0);
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

bool empty(uint8_t val, enum Direction dir) {
  /*  can a player move through here?  */
  switch(val) {
  case i_plotSquare:
  case i_cursEq:
  case i_small0:
  case i_upExcl:
  case i_deltaCap:
  case i_delta:
  case i_omega:
  case i_xSuper:
  case i_ellipsis:
  case i_boldN:
  case i_doubleCloseParen:
  case i_cursDotOut:
  case i_plotDot:
  case '@':
    return false;
  case '<':
    return dir != Right;
  case '>':
    return dir != Left;
  case '^':
    return dir != Down;
  case 'v':
    return dir != Up;
  case '-':
    return dir == Left || dir == Right;
  case '|':
    return dir == Up || dir == Down;
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
  case i_boldN:
  case i_deltaCap:
  case i_delta:
  case i_xSuper:
  case i_bigArrDown:
  case i_small0:
  case i_omega:
  case i_wideX:
  case i_cursEq:
  case i_upExcl:
  case i_doubleCloseParen:
  case i_plotSquare:
  case i_cursDotOut:
  case i_plotDot:
  case i_ellipsis:
  case i_diuresis:
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
  temp2[len] = 0;
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

void newMapDraw(struct Map* m, bool f, int* y, int* x) {
  if(f) {
    mapDraw(m, true, 0, 0, 0);
    return;
  }
  char buf[2] = " ";
  for(int i=0;;i++) {
    if(x[i] == -2 || y[i] == -2)
      break;
    if(x[i] == -1 || y[i] == -1)
      continue;
    os_SetCursorPos(y[i], m->xpad + x[i]);
    buf[0] = m->map[y[i]][x[i]];
    os_PutStrLine(buf);
  }
}

int facing(struct Player* p, struct Map* m) {
  switch(p->dir) {
  case Up:
    if(p->y == 0)
      return -1;
    return m->map[p->y-1][p->x];
  case Down:
    if(p->y == m->ymax)
      return -1;
    return m->map[p->y+1][p->x];
  case Left:
    if(p->x == 0)
      return -1;
    return m->map[p->y][p->x-1];
  case Right:
    if(p->x == m->xmax)
      return -1;
    return m->map[p->y][p->x+1];
  default:
    return -1;
  }
}

int fsx(struct Player* p, struct Map* m) {
  switch(p->dir) {
  case Up:
    if(p->y == 0)
      return -1;
    return p->x;
  case Down:
    if(p->y == m->ymax)
      return -1;
    return p->x;
  case Left:
    if(p->x == 0)
      return -1;
    return p->x-1;
  case Right:
    if(p->x == m->xmax)
      return -1;
    return p->x+1;
  default:
    return -1;
  }
}
int fsy(struct Player* p, struct Map* m) {
  switch(p->dir) {
  case Up:
    if(p->y == 0)
      return -1;
    return p->y-1;
  case Down:
    if(p->y == m->ymax)
      return -1;
    return p->y+1;
  case Left:
    if(p->x == 0)
      return -1;
    return p->y;
  case Right:
    if(p->x == m->xmax)
      return -1;
    return p->y;
  default:
    return -1;
  }
}
void handleFacingSpace(struct Player* p, struct Map* m, int fs) { /*  this handles interactions  */
  /*   if this routine is entered, that means that facingSpace is on the map   */
  const int fx = fsx(p, m);
  const int fy = fsy(p, m);
  switch(fs) {
  case i_cursEq:
    /*  crate  */

    m->map[fy][fx] = m->sub[fy][fx];
    break;
  case i_omega:
    /* key */
    p->key++;
    m->map[fy][fx] = ' ';
    break;
  case i_upExcl:
    /*  sword  */
    p->sw = SWORD_MAX_DURABILITY;
    m->map[fy][fx] = ' ';
    break;
  case i_small0:
    /*  coin  */
    p->coin++;
    m->map[fy][fx] = ' ';
    break;
  case i_delta:
    /*  flask  */
    break;
  case i_deltaCap:
    /*  energy up  */
    p->ep++;
    m->map[fy][fx] = ' ';
    break;
  case '@':
    /*  breakable wall  */
    p->sw--;
    m->map[fy][fx] = ' ';
    break;
  case i_cursDotOut:
    /*  locked door  */
    if(p->key > 0) {
      p->key--;
      m->map[fy][fx] = ' ';
    }
    break;
  default:
      break;
  }
}
