#include <fileioc.h>


#include <string.h>

#include "funcs.h"
#include "level.h"

uint8_t loadLevelFromAppVar(struct Map* m, const char* name) {
  const uint8_t handle = ti_Open(name, "r");
  if(handle == 0) {
    ti_Close(handle);
    return 2;
  }

  //char buffer[MAP_WIDTH * MAP_HEIGHT];
  const size_t read = ti_Read(m->map[0], 20*sizeof(uint8_t), 10, handle);

  //memcpy(m->map, buffer, sizeof(buffer));

  ti_Close(handle);
  if(read == 1) {
    return 0;
  }
  else if(read == 0){
    return 1;
  }
  else {
    return 4;
  }
}
