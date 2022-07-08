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
#include "level.h"

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
  bool run = true;
  bool dispUpd = true;
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

  /*  Load level  */
  const uint8_t stat = loadLevelFromAppVar(&mainMap, "ZML1");
  char msg[9];
  if(stat == 0) {
    strcpy(msg, "load suc");
  }
  else if(stat == 1){
    strcpy(msg, "inv load");
  }
  else if(stat == 2) {
    strcpy(msg, "inv name");
  }
  else if(stat == 4) {
    strcpy(msg, "uh oh...");
  }
  else {
    strcpy(msg, "inv msg.");
  }
  xv_MoveCursorAndPrint(msg, 4, 0);
      

  os_SetCursorPos(3, 0);
  os_PutStrFull("Press enter to start...   ");

  xv_Pause();
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


    if(kb_Data[7] & kb_Down) {
      if(player.y < mainMap.ymax && empty(mainMap.map[player.y+1][player.x]) && !(blockedSpace(&creature, player.y+1, player.x) || blockedSpace(&energy, player.y+1, player.x))) {
	dispUpd = true;
	player.y++;
      }
      player.dir = Down;
    }
    if(kb_Data[7] & kb_Left) {
      if(player.x > mainMap.xmin && empty(mainMap.map[player.y][player.x-1]) && !(blockedSpace(&creature, player.y, player.x-1) || blockedSpace(&energy, player.y, player.x-1))) {
	dispUpd = true;
	player.x--;
      }
      player.dir = Left;
    }
    if(kb_Data[7] & kb_Right) {
      if(player.x < mainMap.xmax && empty(mainMap.map[player.y][player.x+1]) && !(blockedSpace(&creature, player.y, player.x+1) || blockedSpace(&energy, player.y, player.x+1))) {
	dispUpd = true;
	player.x++;
      }
      player.dir = Right;
    }
    if(kb_Data[7] & kb_Up) {
      if(player.y > mainMap.ymin && empty(mainMap.map[player.y-1][player.x]) && !(blockedSpace(&creature, player.y-1, player.x) || blockedSpace(&energy, player.y-1, player.x))) {
	dispUpd = true;
	player.y--;
      }
      player.dir = Up;
    }
    if(kb_Data[1] & kb_2nd) {
      /*  pausa  */


      xv_MoveCursorAndPrint("Game Paused. Press", 0, 6);
      xv_MoveCursorAndPrint("Enter to resume   ", 1, 6);
      xv_Pause();
      xv_MoveCursorAndPrint("                  ", 0, 6);
      xv_MoveCursorAndPrint("                  ", 1, 6);
      dispUpd = true;
    }
    if(kb_Data[1] & kb_Yequ) {
      /* capture */
      if(player.ep > 0 && canCatchCreature(&player, &creature, &mainMap)) {
	player.captures++;
	player.ep--;
	generateNewCreature(&player, &mainMap, &creature, &energy);
	dispUpd = true;
      }
      else if(canCatchCreature(&player, &energy, &mainMap)) {
	if(randInt(0, 5) > 1) {
	  player.ep++;
	}
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

      mapDraw(&mainMap);

      os_SetCursorPos(oldY, oldX + mainMap.xpad);
      os_PutStrLine(" ");
      os_SetCursorPos(player.y, player.x + mainMap.xpad);
      os_PutStrLine(c_theta);
      os_SetCursorPos(oldCrY, oldCrX + mainMap.xpad);
      os_PutStrLine(" ");
      os_SetCursorPos(creature.y, creature.x + mainMap.xpad);
      os_PutStrLine(c_plotPlus);
      os_SetCursorPos(oldEnY, oldEnX + mainMap.xpad);
      os_PutStrLine(" ");
      os_SetCursorPos(energy.y, energy.x + mainMap.xpad);
      os_PutStrLine(c_plotBox);

      printNCharsOfInt(player.hp, 2, 0, 3);
      printNCharsOfInt(player.ep, 2, 1, 3);
      printNCharsOfInt(player.captures, 5, 3, 0);

    }
  } while(run);
  kb_DisableOnLatch();
  return 0;
}


