#include "DefaultIncludes.h"

#pragma once

/*

Enemy Mothership


     ******
   **********
  ************
 ** ** ** ** **
****************
  ***      ***
   *        *

*/

#define EnemyMothership_Width 16
#define EnemyMothership_Height 7

global_variable color EnemyMothership_Bitmask[EnemyMothership_Width * EnemyMothership_Height] = {
	Blank, Blank, Blank, Blank, Blank, White, White, White, White, White, White, Blank, Blank, Blank, Blank, Blank,
	Blank, Blank, Blank, White, White, White, White, White, White, White, White, White,	White, Blank, Blank, Blank,
	Blank, Blank, White, White, White, White, White, White, White, White, White, White,	White, White, Blank, Blank,
	Blank, White, White, Blank, White, White, Blank, White, White, Blank, White, White,	Blank, White, White, Blank,
	White, White, White, White, White, White, White, White, White, White, White, White,	White, White, White, White,
	Blank, Blank, White, White, White, Blank, Blank, Blank, Blank, Blank, Blank, White,	White, White, Blank, Blank,
	Blank, Blank, Blank, White, Blank, Blank, Blank, Blank, Blank, Blank, Blank, Blank, White, Blank, Blank, Blank };

global_variable bitmap EnemyMothership_Bitmap = { (u32*)EnemyMothership_Bitmask, EnemyMothership_Width, EnemyMothership_Height };

/*

A0

    ****    
 ********** 
************
************
***  **  ***
  ***  ***  
 **  **  ** 
  **    **  

*/

#define EnemyA0_Width 12
#define EnemyA0_Height 8

global_variable color EnemyA0_Bitmask[EnemyA0_Width * EnemyA0_Height] = {
				Blank, Blank, Blank, Blank, White, White, White, White, Blank, Blank, Blank, Blank,
				Blank, White, White, White, White, White, White, White, White, White, White, Blank,
				White, White, White, White, White, White, White, White, White, White, White, White,
				White, White, White, White, White, White, White, White, White, White, White, White,
				White, White, White, Blank, Blank, White, White, Blank, Blank, White, White, White,
				Blank, Blank, White, White, White, Blank, Blank, White, White, White, Blank, Blank,
				Blank, White, White, Blank, Blank, White, White, Blank, Blank, White, White, Blank,
				Blank, Blank, White, White, Blank, Blank, Blank, Blank, White, White, Blank, Blank };

global_variable bitmap EnemyA0_Bitmap = { (u32*)EnemyA0_Bitmask, EnemyA0_Width, EnemyA0_Height };

/*

A1

    ****
 **********
************
************
***  **  ***
   **  **
  ** ** **
**        **

*/

#define EnemyA1_Width 12
#define EnemyA1_Height 8

global_variable color EnemyA1_Bitmask[EnemyA1_Width * EnemyA1_Height] = {
	Blank, Blank, Blank, Blank, White, White, White, White, Blank, Blank, Blank, Blank,
	Blank, White, White, White, White, White, White, White, White, White, White, Blank,
	White, White, White, White, White, White, White, White, White, White, White, White,
	White, White, White, White, White, White, White, White, White, White, White, White,
	White, White, White, Blank, Blank, White, White, Blank, Blank, White, White, White,
	Blank, Blank, Blank, White, White, Blank, Blank, White, White, Blank, Blank, Blank,
	Blank, Blank, White, White, Blank, White, White, Blank, White, White, Blank, Blank,
	White, White, Blank, Blank, Blank, Blank, Blank, Blank, Blank, Blank, White, White };

global_variable bitmap EnemyA1_Bitmap = { (u32*)EnemyA1_Bitmask, EnemyA1_Width, EnemyA1_Height };

/*

B0

  *     *
   *   *
  *******
 ** *** **
***********
* ******* *
* *     * *
  *** ***

*/

#define EnemyB0_Width 11
#define EnemyB0_Height 8

global_variable color EnemyB0_Bitmask[EnemyB0_Width * EnemyB0_Height] = {
	Blank, Blank, White, Blank, Blank, Blank, Blank, Blank, White, Blank, Blank,
	Blank, Blank, Blank, White, Blank, Blank, Blank, White, Blank, Blank, Blank,
	Blank, Blank, White, White, White, White, White, White, White, Blank, Blank,
	Blank, White, White, Blank, White, White, White, Blank, White, White, Blank,
	White, White, White, White, White, White, White, White, White, White, White,
	White, Blank, White, White, White, White, White, White, White, Blank, White,
	White, Blank, White, Blank, Blank, Blank, Blank, Blank, White, Blank, White,
	Blank, Blank, White, White, White, Blank, White, White, White, Blank, Blank };

global_variable bitmap EnemyB0_Bitmap = { (u32*)EnemyB0_Bitmask, EnemyB0_Width, EnemyB0_Height };

/*

B1

  *     *
*  *   *  *
* ******* *
*** *** ***
***********
 *********
  *     *
 *       *

*/

#define EnemyB1_Width 11
#define EnemyB1_Height 8

global_variable color EnemyB1_Bitmask[EnemyB1_Width * EnemyB1_Height] = {
	Blank, Blank, White, Blank, Blank, Blank, Blank, Blank, White, Blank, Blank,
	Blank, Blank, Blank, White, Blank, Blank, Blank, White, Blank, Blank, Blank,
	White, Blank, White, White, White, White, White, White, White, Blank, White,
	White, White, White, Blank, White, White, White, Blank, White, White, White,
	White, White, White, White, White, White, White, White, White, White, White,
	Blank, Blank, White, White, White, White, White, White, White, Blank, Blank,
	Blank, Blank, White, Blank, Blank, Blank, Blank, Blank, White, Blank, Blank,
	Blank, White, Blank, Blank, Blank, Blank, Blank, Blank, Blank, White, Blank };

global_variable bitmap EnemyB1_Bitmap = { (u32*)EnemyB1_Bitmask, EnemyB1_Width, EnemyB1_Height };


/*

C0

  ****
 ******
** ** **
********
 * ** *
*      *
 *    *

*/

#define EnemyC0_Width 8
#define EnemyC0_Height 8

global_variable color EnemyC0_Bitmask[EnemyC0_Width * EnemyC0_Height] = {
	Blank, Blank, Blank, White, White, Blank, Blank, Blank,
	Blank, Blank, White, White, White, White, Blank, Blank,
	Blank, White, White, White, White, White, White, Blank,
	White, White, Blank, White, White, Blank, White, White,
	White, White, White, White, White, White, White, White,
	Blank, White, Blank, White, White, Blank, White, Blank,
	White, Blank, Blank, Blank, Blank, Blank, Blank, White,
	Blank, White, Blank, Blank, Blank, Blank, White, Blank };

global_variable bitmap EnemyC0_Bitmap = { (u32*)EnemyC0_Bitmask, EnemyC0_Width, EnemyC0_Height };


/*

C1

  ****
 ******
** ** **
********
  *  *
 * ** *
* *  * *

*/

#define EnemyC1_Width 8
#define EnemyC1_Height 8

global_variable color EnemyC1_Bitmask[EnemyC1_Width * EnemyC1_Height] = {
	Blank, Blank, Blank, White, White, Blank, Blank, Blank,
	Blank, Blank, White, White, White, White, Blank, Blank,
	Blank, White, White, White, White, White, White, Blank,
	White, White, Blank, White, White, Blank, White, White,
	White, White, White, White, White, White, White, White,
	Blank, Blank, White, Blank, Blank, White, Blank, Blank,
	Blank, White, Blank, White, White, Blank, White, Blank,
	White, Blank, White, Blank, Blank, White, Blank, White };

global_variable bitmap EnemyC1_Bitmap = { (u32*)EnemyC1_Bitmask, EnemyC1_Width, EnemyC1_Height };

GAME_UPDATE_AND_RENDER(UpdateAndRender_SpaceInvaders)
{
	game_state* GameState = (game_state*)GameMemory->Permament.Memory;

	if (!GameMemory->Initialized)
	{
		//TODO(Jeremy): Do something with the audio a little more sensible
		GameState->ToneHz = 512;
		GameState->tSine = 0.0f;


		GameState->PlayerX = 100;
		GameState->PlayerY = 100;

		GameMemory->Permament.Used += sizeof(game_state);
		GameMemory->Initialized = true;
	}

	r32 PlayerSpeed = 8.0f;

	GameState->PlayerX += (s32)(PlayerInput->Controllers[CONTROLLER_ONE].LeftStick.X * PlayerSpeed);
	GameState->PlayerY += (s32)(PlayerInput->Controllers[CONTROLLER_ONE].LeftStick.Y * -PlayerSpeed);

	ClearScreen(Buffer, ColorRGB(255, 0, 255, 255));

	for (u32 i = 0, j = 0; j < 500; i += 20)
	{
		RenderBitmap(Buffer, EnemyB0_Bitmap, GameState->PlayerX + (i * 20), GameState->PlayerY + (j * 20), 12 + (u32)(sin(i + j) * 10));
		if (i >= 200)
		{
			j += 20;
			i = 0;
		}
	}
	RenderText(Buffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n01234566789", 0, 0, 10, ColorRGB(255, 255, 255, 255));
	//RenderText(Buffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n01234566789", GameState->PlayerX, GameState->PlayerY, 10, ColorRGB(0, 255, 255, 255));
	//RenderText(Buffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n01234566789", GameState->PlayerX, GameState->PlayerY, 8, ColorRGB(255, 0, 0, 1), 8);
}