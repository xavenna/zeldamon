#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <keypadc.h>

#include <ti/screen.h>
#include <ti/getcsc.h>  /*  Included for the loading screen  */
#include <ti/real.h>

#include <sys/timers.h>
#include <sys/util.h>

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
  uint8_t programCounter = 0;
  bool run = true;
  bool dispUpd = true;
  bool statUpd = true;
  bool action = false;
  bool move = false;

  int xlist[240];
  int ylist[240];
  for(int i=0;i<240;i++) {
    xlist[i] = -1;
    ylist[i] = -1;
  }
  struct Player player;
  struct Map mainMap;
  struct Creature creature;
  struct Creature energy;
  struct Enemy enemy[ENEMY_LIMIT];  //enemy limit is the number of enemies that can be active at once
  initPlayer(&player);
  initMap(&mainMap);
  initCreature(&creature);
  initCreature(&energy);
  for(int i=0;i<ENEMY_LIMIT;i++) {
    initEnemy(&(enemy[i]));
  }
  enemy[0].active = true;
  enemy[0].y = 7;
  enemy[0].x = 15;
  enemy[0].targetMode = Random;

  /* SET UP SCREEN */
  
  os_ClrHome(); 
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
  else if(stat == 5) {
    strcpy(msg, "old lvl.");
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
  mapDraw(&mainMap, true, -1, -1, -1);  //force full map draw
  do {  /* Main program loop */

    for(int i=0;i<240;i++) {
      ylist[i] = -1;
      xlist[i] = -1;
    }
    for(int i=0;i<ENEMY_LIMIT;i++) {
      if(!enemy[i].active)
	continue;
      enemy[i].oy = enemy[i].y;
      enemy[i].ox = enemy[i].x;
    }
    oldY = player.y;
    oldX = player.x;
    oldCrY = creature.y;
    oldCrX = creature.x;
    oldEnY = energy.y;
    oldEnX = energy.x;

    kb_Scan();  /* get keys pressed */

    /*  only check key presses every 5 frames  */
    if(programCounter % 5 == 0) {
      if(kb_Data[7] & kb_Down) {
	player.dir = Down;
	move = true;
	if(player.y < mainMap.ymax) {
	  if(empty(mainMap.map[player.y+1][player.x], Down) && !(blockedSpace(&creature, player.y+1, player.x) || blockedSpace(&energy, player.y+1, player.x) || enemyBlock(enemy, player.y+1, player.x))) {
	    dispUpd = true;
	    player.y++;
	  }
	  else if(facing(&player, &mainMap) == i_xSuper && programCounter % 25 == 0) {
	    /*  only check damage every 25 frames  */
	    player.hp--;
	    statUpd = true;
	  }
	}
      }
      if(kb_Data[7] & kb_Left) {
	player.dir = Left;
	move = true;
	if(player.x > mainMap.xmin) {
	  if(empty(mainMap.map[player.y][player.x-1], Left) && !(blockedSpace(&creature, player.y, player.x-1) || blockedSpace(&energy, player.y, player.x-1) || enemyBlock(enemy, player.y, player.x-1))) {
	    dispUpd = true;
	    player.x--;
	  }
	  else if(facing(&player, &mainMap) == i_xSuper && programCounter % 25 == 0) {
	    player.hp--;
	    statUpd = true;
	    /*  ADD INVINCIBILITY FRAME  */
	  }
	}
      }
      if(kb_Data[7] & kb_Right) {
	player.dir = Right;
	move = true;
	if(player.x < mainMap.xmax) {
	  if(empty(mainMap.map[player.y][player.x+1], Right) && !(blockedSpace(&creature, player.y, player.x+1) || blockedSpace(&energy, player.y, player.x+1) || enemyBlock(enemy, player.y, player.x+1))) {
	    dispUpd = true;
	    player.x++;
	  }
	  else if(facing(&player, &mainMap) == i_xSuper && programCounter % 25 == 0) {
	    player.hp--;
	    statUpd = true;
	  }
	}
      }
      if(kb_Data[7] & kb_Up) {
	player.dir = Up;
	move = true;
	if(player.y > mainMap.ymin) {
	  if(empty(mainMap.map[player.y-1][player.x], Up) && !(blockedSpace(&creature, player.y-1, player.x) || blockedSpace(&energy, player.y-1, player.x) || enemyBlock(enemy, player.y-1, player.x))) {
	    dispUpd = true;
	    player.y--;
	  }
	  else if(facing(&player, &mainMap) == i_xSuper && programCounter % 25 == 0) {
	    player.hp--;
	    statUpd = true;
	  }
	}
      }
      
      if(kb_Data[1] & kb_2nd) {
	/*  pause  */
	
	
	xv_MoveCursorAndPrint("Game Paused. Press", 0, 6);
	xv_MoveCursorAndPrint("Enter to resume   ", 1, 6);
	xv_Pause();
	xv_MoveCursorAndPrint("                  ", 0, 6);
	xv_MoveCursorAndPrint("                  ", 1, 6);
	dispUpd = true;
	
      }
      if(kb_Data[1] & kb_Yequ) {
	/* capture */
	move = true;
	if(player.ep > 0 && canCatchCreature(&player, &creature, &mainMap)) {
	  player.captures++;
	  player.ep--;
	  generateNewCreature(&player, &mainMap, &creature, &energy);
	  dispUpd = true;
	  action = true;
	}
	else if(canCatchCreature(&player, &energy, &mainMap)) {
	  if(randInt(0, 5) > 1) {
	    player.ep++;
	  }
	  generateNewCreature(&player, &mainMap, &energy, &creature);
	  dispUpd = true;
	  action = true;
	}
	int facingSpace = facing(&player, &mainMap);
	if(facingSpace != -1) {  /*  -1 means that player is at the edge of the map  */
	  handleFacingSpace(&player, &mainMap, facingSpace);
	  dispUpd = true;
	  action = true;
	}
      }
    }
    if(kb_On) {  /*  the on check can happen every frame  */
      /* break */
      run = false;
    }

    /*   handle enemies   */
    if(move) {
      for(int i=0;i<ENEMY_LIMIT;i++) {
	if(enemy[i].active) {
	  uint8_t direc;
	  switch(enemy[i].targetMode) {
	  case Random:
	    direc = randInt(0,4);  /*  up, clockwise.   4 means stay still  */
	    switch(direc) {
	    case 0: /*  move up  */
	      if(enemy[i].y != 0 && enemyCanMove(mainMap.map[enemy[i].y-1][enemy[i].x], Up) && !(blockedSpace(&creature, enemy[i].y-1, enemy[i].x) || blockedSpace(&energy, enemy[i].y-1, enemy[i].x)) && !(player.y == enemy[i].y-1 && player.x == enemy[i].x)) {
		enemy[i].y = enemy[i].y - 1;
	      }
	      break;
	    case 1: /*  move right  */
	      if(enemy[i].x != mainMap.xmax && enemyCanMove(mainMap.map[enemy[i].y][enemy[i].x+1], Right) && !(blockedSpace(&creature, enemy[i].y, enemy[i].x+1) || blockedSpace(&energy, enemy[i].y, enemy[i].x+1)) && !(player.y == enemy[i].y && player.x == enemy[i].x+1)) {
		enemy[i].x = enemy[i].x + 1;
	      }
	    break;
	    case 2: /*  move down  */
	      if(enemy[i].y != mainMap.ymax && enemyCanMove(mainMap.map[enemy[i].y+1][enemy[i].x], Down) && !(blockedSpace(&creature, enemy[i].y+1, enemy[i].x) || blockedSpace(&energy, enemy[i].y+1, enemy[i].x)) && !(player.y == enemy[i].y+1 && player.x == enemy[i].x)) {
		enemy[i].y = enemy[i].y + 1;
	      }
	      break;
	    case 3: /*  move left  */
	      if(enemy[i].x != 0 && enemyCanMove(mainMap.map[enemy[i].y][enemy[i].x-1], Left) && !(blockedSpace(&creature, enemy[i].y, enemy[i].x-1) || blockedSpace(&energy, enemy[i].y, enemy[i].x-1)) && !(player.y == enemy[i].y && player.x == enemy[i].x-1)) {
		enemy[i].x = enemy[i].x - 1;
	      }
	      break;
	    case 4: /*  don't move  */
	      break;
	    }
	    break;
	  default:
	    break;
	  }
	}
      }
      dispUpd = true;
    }
    
    if(oldX != player.x || oldY != player.y) {
      xlist[0] = oldX;
      xlist[1] = player.x;
      ylist[0] = oldY;
      ylist[1] = player.y;
    }
    if(oldEnX != energy.x || oldEnY != energy.y) {
      xlist[2] = oldEnX;
      xlist[3] = energy.x;
      ylist[2] = oldEnY;
      ylist[3] = energy.y;
    }
    if(oldCrX != creature.x || oldCrY != creature.y) {
      xlist[4] = oldCrX;
      xlist[5] = creature.x;
      ylist[4] = oldCrY;
      ylist[5] = creature.y;
    }
    if(action) {
      xlist[6] = fsx(&player, &mainMap);
      ylist[6] = fsy(&player, &mainMap);
    }
    for(int i=0;i<ENEMY_LIMIT;i++) {  /*  update places where enemies spawn  */
      if(!enemy[i].active) {
	xlist[7+2*i] = -1;
	xlist[8+2*i] = -1;
	ylist[7+2*i] = -1;
	ylist[8+2*i] = -1;
      }
      else if(enemy[i].ox != enemy[i].x || enemy[i].oy != enemy[i].y) {
	xlist[7+2*i] = enemy[i].ox;
	xlist[8+2*i] = enemy[i].x;
	ylist[7+2*i] = enemy[i].oy;
	ylist[8+2*i] = enemy[i].y;
      }
    }
    xlist[7+2*ENEMY_LIMIT] = -2;  /*  Terminate  */
    ylist[7+2*ENEMY_LIMIT] = -2;

    /*  refresh display */
    if(dispUpd || statUpd) {
      printNCharsOfInt(player.hp, 2, 0, 3);
      printNCharsOfInt(player.ep, 2, 1, 3);
      printNCharsOfInt(player.coin, 2, 2, 3);
      printNCharsOfInt(player.captures, 6, 4, 0);
    }
    if(dispUpd) {
      /* add delay because asm is much faster than BASIC */
      delay(40);

      if(oldX != player.x || oldY != player.y) {
	os_SetCursorPos(oldY, oldX + mainMap.xpad);
	os_PutStrLine(" ");
      }
      if(oldCrX != creature.x || oldCrY != creature.y) {
	os_SetCursorPos(oldCrY, oldCrX + mainMap.xpad);
	os_PutStrLine(" ");
      }
      if(oldEnX != energy.x || oldEnY != energy.y) {
	os_SetCursorPos(oldEnY, oldEnX + mainMap.xpad);
	os_PutStrLine(" ");
      }
      for(int i=0;i<ENEMY_LIMIT;i++) {  /*  update places where enemies spawn  */
	if(enemy[i].active && (enemy[i].ox != enemy[i].x || enemy[i].oy != enemy[i].y)) {
	  os_SetCursorPos(enemy[i].oy, enemy[i].ox + mainMap.xpad);
	  os_PutStrLine(" ");
	}
      }

      newMapDraw(&mainMap, false, ylist, xlist);

      os_SetCursorPos(player.y, player.x + mainMap.xpad);
      os_PutStrLine(c_theta);
      os_SetCursorPos(creature.y, creature.x + mainMap.xpad);
      os_PutStrLine(c_plotPlus);
      os_SetCursorPos(energy.y, energy.x + mainMap.xpad);
      os_PutStrLine(c_plotBox);
      for(int i=0;i<ENEMY_LIMIT;i++) {
	if(enemy[i].active) {
	  os_SetCursorPos(enemy[i].y, enemy[i].x + mainMap.xpad);
	  os_PutStrLine(c_chi);
	}
      }
    }
    dispUpd = false;
    statUpd = false;
    action = false;
    move = false;
    programCounter += (programCounter==249? -249 : 1);
  } while(run);
  kb_DisableOnLatch();
  return 0;
}


