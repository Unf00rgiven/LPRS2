
///////////////////////////////////////////////////////////////////////////////
// Headers.

#include <stdint.h>
#include "system.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "sprites_rgb333.h"

///////////////////////////////////////////////////////////////////////////////
// HW stuff.

#define WAIT_UNITL_0(x) while(x != 0){}
#define WAIT_UNITL_1(x) while(x != 1){}

#define SCREEN_IDX1_W 640
#define SCREEN_IDX1_H 480
#define SCREEN_IDX4_W 320
#define SCREEN_IDX4_H 240
#define SCREEN_RGB333_W 160
#define SCREEN_RGB333_H 120

#define NUMFOOD 5

#define SCREEN_IDX4_W8 (SCREEN_IDX4_W/8)

#define gpu_p32 ((volatile uint32_t*)LPRS2_GPU_BASE)
#define palette_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x1000))
#define unpack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x400000))
#define pack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x600000))
#define unpack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x800000))
#define pack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xa00000))
#define unpack_rgb333_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xc00000))
#define joypad_p32 ((volatile uint32_t*)LPRS2_JOYPAD_BASE)

typedef struct {
	unsigned a      : 1;
	unsigned b      : 1;
	unsigned z      : 1;
	unsigned start  : 1;
	unsigned up     : 1;
	unsigned down   : 1;
	unsigned left   : 1;
	unsigned right  : 1;
} bf_joypad;
#define joypad (*((volatile bf_joypad*)LPRS2_JOYPAD_BASE))

typedef struct {
	uint32_t m[SCREEN_IDX1_H][SCREEN_IDX1_W];
} bf_unpack_idx1;
#define unpack_idx1 (*((volatile bf_unpack_idx1*)unpack_idx1_p32))



///////////////////////////////////////////////////////////////////////////////
// Game config.

#define STEP 1


#define PACMAN_ANIM_DELAY 3


///////////////////////////////////////////////////////////////////////////////
// Game data structures.

typedef struct {
	uint16_t x;
	uint16_t y;
} point_t;



typedef enum {
	PACMAN_IDLE,
	PACMAN_OPENING_MOUTH,
	PACMAN_WITH_OPEN_MOUTH,
	PACMAN_CLOSING_MOUTH,
	PACMAN_WITH_CLOSED_MOUTH
} pacman_anim_states_t;

typedef enum{
	LEFT,
	RIGHT,
	DOWN,
	UP
} direction;

typedef struct {
	pacman_anim_states_t state;
	uint8_t delay_cnt;
} pacman_anim_t;



typedef struct {
	point_t pos;
	pacman_anim_t anim;
	uint16_t eaten;
} pacman_t;



typedef struct {
	point_t pos[NUMFOOD];
	uint16_t eaten[NUMFOOD];
} food_t;

typedef struct {
	point_t pos;
} ghost_t;


typedef struct {
	pacman_t pacman;
	food_t food;
	ghost_t ghost;
} game_state_t;







void draw_sprite_from_atlas(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {
	
	
	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx = 
				(src_y+y)*Pacman_Sprite_Map__w +
				(src_x+x);
			uint32_t dst_idx = 
				(dst_y+y)*SCREEN_RGB333_W +
				(dst_x+x);
			uint16_t pixel = Pacman_Sprite_Map__p[src_idx];
			unpack_rgb333_p32[dst_idx] = pixel;
		}
	}
	
	
}



///////////////////////////////////////////////////////////////////////////////
// Game code.

int main(void) {
	
	// Setup.
	gpu_p32[0] = 3; // RGB333 mode.
	gpu_p32[0x800] = 0x00ff00ff; // Magenta for HUD.
	srand(time(NULL));


	// Game state.
	game_state_t gs;
	gs.pacman.pos.x = 32;
	gs.pacman.pos.y = 32;
	gs.pacman.anim.state = PACMAN_IDLE;
	gs.pacman.anim.delay_cnt = 0;
	gs.ghost.pos.x = 76;
	gs.ghost.pos.y = 24;
	gs.pacman.eaten = 1;
	
	for(uint16_t i=0; i < NUMFOOD; i++)
	{
		gs.food.pos[i].x = rand();
		if(gs.food.pos[i].x > SCREEN_RGB333_W - 16) gs.food.pos[i].x = (gs.food.pos[i].x % SCREEN_RGB333_W);
		if(gs.food.pos[i].x > SCREEN_RGB333_W - 16); gs.food.pos[i].x -=16;

		gs.food.pos[i].y = rand();
		if(gs.food.pos[i].y > SCREEN_RGB333_H - 16) gs.food.pos[i].y = (gs.food.pos[i].y % SCREEN_RGB333_H);
		if(gs.food.pos[i].y > SCREEN_RGB333_H - 16); gs.food.pos[i].y -=16;

		gs.food.eaten[i] = 1;
	}
	
	while(1){
		
		direction dir;
		/////////////////////////////////////
		// Poll controls.
		
		int mov_x = 0;
		int mov_y = 0;
		if(joypad.right == 1 && joypad.down == 0 && joypad.up == 0 && joypad.left == 0){
			if(gs.pacman.pos.x + 16 < SCREEN_RGB333_W) 
			{
				mov_x = +1;
				dir = RIGHT;
			}
		}
		if(joypad.right == 0 && joypad.down == 0 && joypad.up == 0 && joypad.left == 1){
			if(gs.pacman.pos.x > 0)
			{ 
				mov_x = -1;
				dir = LEFT;
			}
		}
		if(joypad.right == 0 && joypad.down == 0 && joypad.up == 1 && joypad.left == 0){
			if(gs.pacman.pos.y > 0)
			{
				mov_y = -1;
				dir = UP;
			}
		}
		if(joypad.right == 0 && joypad.down == 1 && joypad.up == 0 && joypad.left == 0){
			if(gs.pacman.pos.y + 16 < SCREEN_RGB333_H)
			{
				mov_y = +1;
				dir = DOWN;
			}
		}
		
		/////////////////////////////////////
		// Gameplay.
		
		
		gs.pacman.pos.x += mov_x*STEP;
		gs.pacman.pos.y += mov_y*STEP;
		
		for(uint16_t i=0; i < NUMFOOD; i++)
		{
			if((gs.pacman.pos.x > gs.food.pos[i].x - 3)  && (gs.pacman.pos.y > gs.food.pos[i].y - 3) && (gs.pacman.pos.x < gs.food.pos[i].x + 3)  && (gs.pacman.pos.y < gs.food.pos[i].y + 3)) 
			gs.food.eaten[i] = 0;
		}

		if((gs.pacman.pos.x > gs.ghost.pos.x - 3)  && (gs.pacman.pos.y > gs.ghost.pos.y - 3) && (gs.pacman.pos.x < gs.ghost.pos.x + 3)  && (gs.pacman.pos.y < gs.ghost.pos.y + 3))
		{
			gs.pacman.eaten = 0;
		}


		switch(gs.pacman.anim.state){
		case PACMAN_IDLE:
			if(mov_x != 0 || mov_y != 0){
				gs.pacman.anim.delay_cnt = PACMAN_ANIM_DELAY;
				gs.pacman.anim.state = PACMAN_WITH_OPEN_MOUTH;
			}
			break;
		case PACMAN_OPENING_MOUTH:
			if(gs.pacman.anim.delay_cnt != 0){
					gs.pacman.anim.delay_cnt--;
			}else{
				gs.pacman.anim.delay_cnt = PACMAN_ANIM_DELAY;
				gs.pacman.anim.state = PACMAN_WITH_OPEN_MOUTH;
			}
			break;
		case PACMAN_WITH_OPEN_MOUTH:
			if(gs.pacman.anim.delay_cnt != 0){
					gs.pacman.anim.delay_cnt--;
			}else{
				if(mov_x != 0 || mov_y != 0){
					gs.pacman.anim.delay_cnt = PACMAN_ANIM_DELAY;
					gs.pacman.anim.state = PACMAN_CLOSING_MOUTH;
				}else{
					gs.pacman.anim.state = PACMAN_IDLE;
				}
			}
			break;
		case PACMAN_CLOSING_MOUTH:
			if(gs.pacman.anim.delay_cnt != 0){
					gs.pacman.anim.delay_cnt--;
			}else{
				gs.pacman.anim.delay_cnt = PACMAN_ANIM_DELAY;
				gs.pacman.anim.state = PACMAN_WITH_CLOSED_MOUTH;
			}
			break;
		case PACMAN_WITH_CLOSED_MOUTH:
			if(gs.pacman.anim.delay_cnt != 0){
					gs.pacman.anim.delay_cnt--;
			}else{
				if(mov_x != 0 || mov_y != 0){
					gs.pacman.anim.delay_cnt = PACMAN_ANIM_DELAY;
					gs.pacman.anim.state = PACMAN_OPENING_MOUTH;
				}else{
					gs.pacman.anim.state = PACMAN_IDLE;
				}
			}
			break;
		}
		
		
		
		/////////////////////////////////////
		// Drawing.
		
		
		// Detecting rising edge of VSync.
		WAIT_UNITL_0(gpu_p32[2]);
		WAIT_UNITL_1(gpu_p32[2]);
		// Draw in buffer while it is in VSync.
		
		
		
		
		// Black background.
		for(uint16_t r = 0; r < SCREEN_RGB333_H; r++){
			for(uint16_t c = 0; c < SCREEN_RGB333_W; c++){
				unpack_rgb333_p32[r*SCREEN_RGB333_W + c] = 0000;
			}
		}


		// DRAW FOOD
		for(uint16_t i=0; i<NUMFOOD; i++)
		{
			if(gs.food.eaten[i])
			draw_sprite_from_atlas(
				16*i, 176, 16, 16, gs.food.pos[i].x, gs.food.pos[i].y
			);
		}
		
		// DRAW GHOST		
		draw_sprite_from_atlas(
				0, 64, 16, 16, gs.ghost.pos.x, gs.ghost.pos.y
		);
		
		// Draw pacman.
		if(gs.pacman.eaten){
			switch(gs.pacman.anim.state){
			case PACMAN_IDLE:
				if(dir == RIGHT)
				draw_sprite_from_atlas(
					16, 0, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				if(dir == LEFT)
				draw_sprite_from_atlas(
					16, 16, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				if(dir == UP)
				draw_sprite_from_atlas(
					16, 32, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				if(dir == DOWN)
				draw_sprite_from_atlas(
					16, 48, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				break;
			case PACMAN_OPENING_MOUTH:
			case PACMAN_CLOSING_MOUTH:
				// Half open mouth.
				if(joypad.right)
				draw_sprite_from_atlas(
					16, 0, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				else if(joypad.left)
				draw_sprite_from_atlas(
					16, 16, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				else if(joypad.up)
				draw_sprite_from_atlas(
					16, 32, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				else if(joypad.down)
				draw_sprite_from_atlas(
					16, 48, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				break;
			case PACMAN_WITH_OPEN_MOUTH:
				// Full open mouth.
				if(joypad.right)
				draw_sprite_from_atlas(
					0, 0, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				else if(joypad.left)
				draw_sprite_from_atlas(
					0, 16, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				else if(joypad.up)
				draw_sprite_from_atlas(
					0, 32, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				else if(joypad.down)
				draw_sprite_from_atlas(
					0, 48, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				break;
			case PACMAN_WITH_CLOSED_MOUTH:
				// Close mouth.
				draw_sprite_from_atlas(
					32, 0, 16, 16, gs.pacman.pos.x, gs.pacman.pos.y
				);
				break;
			}
		}
		else break;
				
		
		
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
