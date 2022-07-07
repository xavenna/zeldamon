/*#include <sys/lcd.h> */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <keypadc.h>

#include <ti/screen.h>
#include <ti/getcsc.h>  /*  Included for the loading screen  */
#include <ti/real.h>

#include <sys/timers.h>
#include "funcs.h"

/* This is zeldamon, a game for the ti-84+ce
 * The original version was written in TI-BASIC, but I decided to rewrite it in c

 * This uses (y,x) screen addressing, because it seems the OS functions use it
 */

int main(void) {

  /*  DEFINE VARIABLES  */
  
  char oldX;
  char oldY;
  char oldCrX;
  char oldCrY;
  char oldEnX;
  char oldEnY;
  char temp[16];
  char temp3[3];
  char temp5[5];
  bool run = true;
  bool dispUpd;
  struct Player player;
  struct Map mainMap;
  struct Creature creature;
  struct Creature energy;
  initPlayer(&player);
  initMap(&mainMap);
  initCreature(&creature);
  initCreature(&energy);

  /* SET UP SCREEN */
  os_ClrHome();  /* clear screen */
  os_SetCursorPos(0, 0);
  os_PutStrFull("Zeldamon v0.0.0a          Created by xavenna        ");
  os_SetCursorPos(3,0);
  os_PutStrFull("Loading...                \x5b...\x0b");

  kb_EnableOnLatch();
  kb_ClearOnLatch();


  os_SetCursorPos(3, 0);
  os_PutStrFull("Press enter to start...   ");

  while (!os_GetCSC());
  os_ClrHome();
  generateNewCreatureSimple(&player, &creature, &mainMap);
  generateNewCreature(&player, &mainMap, &energy, &creature);

  /*  Setup Status Screen  */
  screenSetup();
  do {  /* Main program loop */

    dispUpd = false;
    oldX = player.x;
    oldY = player.y;
    oldCrX = creature.x;
    oldCrY = creature.y;
    oldEnX = energy.x;
    oldEnY = energy.y;

    kb_Scan();  /* get keys pressed */

    

    if(kb_Data[7] & kb_Down && player.y < mainMap.ymax && empty(mainMap.map[player.y+1][player.x])) {
      if(!(blockedSpace(&creature, player.y+1, player.x) || blockedSpace(&energy, player.y+1, player.x))) {
	dispUpd = true;
	player.y++;
	player.dir = Down;
      }
    }
    if(kb_Data[7] & kb_Left && player.x > mainMap.xmin && empty(mainMap.map[player.y][player.x-1])) {
      if(!(blockedSpace(&creature, player.y, player.x-1) || blockedSpace(&energy, player.y, player.x-1))) {
	dispUpd = true;
	player.x--;
	player.dir = Left;
      }
    }
    if(kb_Data[7] & kb_Right && player.x < mainMap.xmax && empty(mainMap.map[player.y][player.x+1])) {
      if(!(blockedSpace(&creature, player.y, player.x+1) || blockedSpace(&energy, player.y, player.x+1))) {
	dispUpd = true;
	player.x++;
	player.dir = Right;
      }
    }
    if(kb_Data[7] & kb_Up && player.y > mainMap.ymin && empty(mainMap.map[player.y-1][player.x])) {
      if(!(blockedSpace(&creature, player.y-1, player.x) || blockedSpace(&energy, player.y-1, player.x))) {
	dispUpd = true;
	player.y--;
	player.dir = Up;
      }
    }
    if(kb_Data[1] & kb_2nd) {
      /* capture */
      if(player.ep > 0 && canCatchCreature(&player, &creature, &mainMap)) {
	player.captures++;
	player.ep--;
	generateNewCreature(&player, &mainMap, &creature, &energy);
	dispUpd = true;
      }
      else if(canCatchCreature(&player, &energy, &mainMap)) {
	player.ep++;
	generateNewCreature(&player, &mainMap, &energy, &creature);
	dispUpd = true;
      }
    }
    if(kb_On) {
      /* break */
      run = false;
    }
    
    /*  refresh display */
    if(dispUpd) {
      /* add delay because asm is much faster than BASIC */
      delay(100);
      os_SetCursorPos(oldY, oldX + mainMap.xpad);
      os_PutStrLine(" ");
      os_SetCursorPos(player.y, player.x + mainMap.xpad);
      os_PutStrLine("\x5b");
      os_SetCursorPos(oldCrY, oldCrX + mainMap.xpad);
      os_PutStrLine(" ");
      os_SetCursorPos(creature.y, creature.x + mainMap.xpad);
      os_PutStrLine("\x0b");
      os_SetCursorPos(oldEnY, oldEnX + mainMap.xpad);
      os_PutStrLine(" ");
      os_SetCursorPos(energy.y, energy.x + mainMap.xpad);
      os_PutStrLine("\x0a");

      real_t t = os_Int24ToReal((int24_t) player.ep);
      os_RealToStr(temp, &(t), 6, 1, -1);
      memcpy(temp3, temp, 3);
      os_SetCursorPos(1, 3);
      os_PutStrLine(temp3);
      /*  Maybe make this series of function calls and casts into a external function to make this less convoluteed */
      t = os_Int24ToReal((int24_t) player.hp);
      os_RealToStr(temp, &(t), 6, 1, -1);
      memcpy(temp3, temp, 3);
      os_SetCursorPos(0, 3);
      os_PutStrLine(temp3);

      t = os_Int24ToReal((int24_t) player.captures);
      os_RealToStr(temp, &(t), 6, 1, -1);
      memcpy(temp5, temp, 3);
      os_SetCursorPos(3, 0);
      os_PutStrLine(temp5);

    }
  } while(run);
  kb_DisableOnLatch();
  return 0;
}


