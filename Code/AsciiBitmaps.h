#include "DefaultIncludes.h"

#pragma once

#define White ColorRGB(255, 255, 255, 255)
#define Blank ColorRGB(0, 0, 0, 0)

/*

0

***
* *
* *
* *
***


*/

#define Number0_Width 3
#define Number0_Height 5

global_variable color Number0_Bitmask[Number0_Width * Number0_Height] = {
	White, White, White,
	White, Blank, White,
	White, Blank, White,
	White, Blank, White,
	White, White, White };

global_variable bitmap Number0_Bitmap = { (u32*)Number0_Bitmask, Number0_Width, Number0_Height };


/*

1

 *
**
 *
 *
***

*/

#define Number1_Width 3
#define Number1_Height 5

global_variable color Number1_Bitmask[Number1_Width * Number1_Height] = {
	Blank, White, Blank,
	White, White, Blank,
	Blank, White, Blank,
	Blank, White, Blank,
	White, White, White };

global_variable bitmap Number1_Bitmap = { (u32*)Number1_Bitmask, Number1_Width, Number1_Height };


/*

2

***
  *
***
*
***

*/

#define Number2_Width 3
#define Number2_Height 5

global_variable color Number2_Bitmask[Number2_Width * Number2_Height] = {
	White, White, White,
	Blank, Blank, White,
	White, White, White,
	White, Blank, Blank,
	White, White, White };

global_variable bitmap Number2_Bitmap = { (u32*)Number2_Bitmask, Number2_Width, Number2_Height };


/*

3

***
  *
***
  *
***

*/

#define Number3_Width 3
#define Number3_Height 5

global_variable color Number3_Bitmask[Number3_Width * Number3_Height] = {
	White, White, White,
	Blank, Blank, White,
	White, White, White,
	Blank, Blank, White,
	White, White, White };

global_variable bitmap Number3_Bitmap = { (u32*)Number3_Bitmask, Number3_Width, Number3_Height };



/*

4

* *
* *
***
  *
  *

*/

#define Number4_Width 3
#define Number4_Height 5

global_variable color Number4_Bitmask[Number4_Width * Number4_Height] = {
	White, Blank, White,
	White, Blank, White,
	White, White, White,
	Blank, Blank, White,
	Blank, Blank, White };

global_variable bitmap Number4_Bitmap = { (u32*)Number4_Bitmask, Number4_Width, Number4_Height };


/*

5

***
*
***
  *
***

*/

#define Number5_Width 3
#define Number5_Height 5

global_variable color Number5_Bitmask[Number5_Width * Number5_Height] = {
	White, White, White,
	White, Blank, Blank,
	White, White, White,
	Blank, Blank, White,
	White, White, White };

global_variable bitmap Number5_Bitmap = { (u32*)Number5_Bitmask, Number5_Width, Number5_Height };


/*

6

***
*
***
* *
***

*/

#define Number6_Width 3
#define Number6_Height 5

global_variable color Number6_Bitmask[Number6_Width * Number6_Height] = {
	White, White, White,
	White, Blank, Blank,
	White, White, White,
	White, Blank, White,
	White, White, White };

global_variable bitmap Number6_Bitmap = { (u32*)Number6_Bitmask, Number6_Width, Number6_Height };


/*

7

***
  *
  *
  *
  *

*/

#define Number7_Width 3
#define Number7_Height 5

global_variable color Number7_Bitmask[Number7_Width * Number7_Height] = {
	White, White, White,
	Blank, Blank, White,
	Blank, Blank, White,
	Blank, Blank, White,
	Blank, Blank, White };

global_variable bitmap Number7_Bitmap = { (u32*)Number7_Bitmask, Number7_Width, Number7_Height };


/*

8

***
* *
***
* *
***


*/

#define Number8_Width 3
#define Number8_Height 5

global_variable color Number8_Bitmask[Number8_Width * Number8_Height] = {
	White, White, White,
	White, Blank, White,
	White, White, White,
	White, Blank, White,
	White, White, White };

global_variable bitmap Number8_Bitmap = { (u32*)Number8_Bitmask, Number8_Width, Number8_Height };


/*

9

***
* *
***
  *
***


*/

#define Number9_Width 3
#define Number9_Height 5

global_variable color Number9_Bitmask[Number9_Width * Number9_Height] = {
	White, White, White,
	White, Blank, White,
	White, White, White,
	Blank, Blank, White,
	White, White, White };

global_variable bitmap Number9_Bitmap = { (u32*)Number9_Bitmask, Number9_Width, Number9_Height };


/*

A


***
* *
***
* *
* *

*/

#define LetterA_Width 3
#define LetterA_Height 5

global_variable color LetterA_Bitmask[LetterA_Width * LetterA_Height] = {
	White, White, White,
	White, Blank, White,
	White, White, White,
	White, Blank, White,
	White, Blank, White };

global_variable bitmap LetterA_Bitmap = { (u32*)LetterA_Bitmask, LetterA_Width, LetterA_Height };


/*

B


**
* *
***
* *
**

*/

#define LetterB_Width 3
#define LetterB_Height 5

global_variable color LetterB_Bitmask[LetterB_Width * LetterB_Height] = {
	White, White, Blank,
	White, Blank, White,
	White, White, White,
	White, Blank, White,
	White, White, Blank };

global_variable bitmap LetterB_Bitmap = { (u32*)LetterB_Bitmask, LetterB_Width, LetterB_Height };


/*

C

***
*
*
*
*
***

*/

#define LetterC_Width 3
#define LetterC_Height 5

global_variable color LetterC_Bitmask[LetterC_Width * LetterC_Height] = {
	White, White, White,
	White, Blank, Blank,
	White, Blank, Blank,
	White, Blank, Blank,
	White, White, White };

global_variable bitmap LetterC_Bitmap = { (u32*)LetterC_Bitmask, LetterC_Width, LetterC_Height };


/*

D

**
* *
* *
* *
**

*/

#define LetterD_Width 3
#define LetterD_Height 5

global_variable color LetterD_Bitmask[LetterD_Width * LetterD_Height] = {
	White, White, Blank,
	White, Blank, White,
	White, Blank, White,
	White, Blank, White,
	White, White, Blank };

global_variable bitmap LetterD_Bitmap = { (u32*)LetterD_Bitmask, LetterD_Width, LetterD_Height };


/*

E

***
*
***
*
***

*/

#define LetterE_Width 3
#define LetterE_Height 5

global_variable color LetterE_Bitmask[LetterE_Width * LetterE_Height] = {
	White, White, White,
	White, Blank, Blank,
	White, White, White,
	White, Blank, Blank,
	White, White, White };

global_variable bitmap LetterE_Bitmap = { (u32*)LetterE_Bitmask, LetterE_Width, LetterE_Height };


/*

F

***
*
***
*
*

*/

#define LetterF_Width 3
#define LetterF_Height 5

global_variable color LetterF_Bitmask[LetterF_Width * LetterF_Height] = {
	White, White, White,
	White, Blank, Blank,
	White, White, White,
	White, Blank, Blank,
	White, Blank, Blank };

global_variable bitmap LetterF_Bitmap = { (u32*)LetterF_Bitmask, LetterF_Width, LetterF_Height };


/*

G

***
*
* *
* *
***

*/

#define LetterG_Width 3
#define LetterG_Height 5

global_variable color LetterG_Bitmask[LetterG_Width * LetterG_Height] = {
	White, White, White,
	White, Blank, Blank,
	White, Blank, White,
	White, Blank, White,
	White, White, White };

global_variable bitmap LetterG_Bitmap = { (u32*)LetterG_Bitmask, LetterG_Width, LetterG_Height };


/*

H

* *
* *
***
* *
* *

*/

#define LetterH_Width 3
#define LetterH_Height 5

global_variable color LetterH_Bitmask[LetterH_Width * LetterH_Height] = {
	White, Blank, White,
	White, Blank, White,
	White, White, White,
	White, Blank, White,
	White, Blank, White };

global_variable bitmap LetterH_Bitmap = { (u32*)LetterH_Bitmask, LetterH_Width, LetterH_Height };


/*

I

***
 *
 *
 *
***


*/

#define LetterI_Width 3
#define LetterI_Height 5

global_variable color LetterI_Bitmask[LetterI_Width * LetterI_Height] = {
	White, White, White,
	Blank, White, Blank,
	Blank, White, Blank,
	Blank, White, Blank,
	White, White, White };

global_variable bitmap LetterI_Bitmap = { (u32*)LetterI_Bitmask, LetterI_Width, LetterI_Height };


/*

J

***
 *
 *
 *
**


*/

#define LetterJ_Width 3
#define LetterJ_Height 5

global_variable color LetterJ_Bitmask[LetterJ_Width * LetterJ_Height] = {
	White, White, White,
	Blank, White, Blank,
	Blank, White, Blank,
	Blank, White, Blank,
	White, White, Blank };

global_variable bitmap LetterJ_Bitmap = { (u32*)LetterJ_Bitmask, LetterJ_Width, LetterJ_Height };


/*

K

* *
* *
**
* *
* *


*/

#define LetterK_Width 3
#define LetterK_Height 5

global_variable color LetterK_Bitmask[LetterK_Width * LetterK_Height] = {
	White, Blank, White,
	White, Blank, White,
	White, White, Blank,
	White, Blank, White,
	White, Blank, White };

global_variable bitmap LetterK_Bitmap = { (u32*)LetterK_Bitmask, LetterK_Width, LetterK_Height };



/*

L

*
*
*
*
***


*/

#define LetterL_Width 3
#define LetterL_Height 5

global_variable color LetterL_Bitmask[LetterL_Width * LetterL_Height] = {
	White, Blank, Blank,
	White, Blank, Blank,
	White, Blank, Blank,
	White, Blank, Blank,
	White, White, White };

global_variable bitmap LetterL_Bitmap = { (u32*)LetterL_Bitmask, LetterL_Width, LetterL_Height };


/*

M

** **
* * *
* * *


*/

#define LetterM_Width 5
#define LetterM_Height 5

global_variable color LetterM_Bitmask[LetterM_Width * LetterM_Height] = {
	White, White, Blank, White, White,
	White, Blank, White, Blank, White,
	White, Blank, White, Blank, White,
	White, Blank, White, Blank, White,
	White, Blank, White, Blank, White };

global_variable bitmap LetterM_Bitmap = { (u32*)LetterM_Bitmask, LetterM_Width, LetterM_Height };


/*

L

***
* *
* *
* *
* *


*/

#define LetterN_Width 3
#define LetterN_Height 5

global_variable color LetterN_Bitmask[LetterN_Width * LetterN_Height] = {
	White, White, White,
	White, Blank, White,
	White, Blank, White,
	White, Blank, White,
	White, Blank, White };

global_variable bitmap LetterN_Bitmap = { (u32*)LetterN_Bitmask, LetterN_Width, LetterN_Height };


/*

O

**
*  *
*  *
*  *
**


*/

#define LetterO_Width 4
#define LetterO_Height 5

global_variable color LetterO_Bitmask[LetterO_Width * LetterO_Height] = {
	Blank, White, White, Blank,
	White, Blank, Blank, White,
	White, Blank, Blank, White,
	White, Blank, Blank, White,
	Blank, White, White, Blank };

global_variable bitmap LetterO_Bitmap = { (u32*)LetterO_Bitmask, LetterO_Width, LetterO_Height };


/*

P

***
* *
***
*
*


*/

#define LetterP_Width 3
#define LetterP_Height 5

global_variable color LetterP_Bitmask[LetterP_Width * LetterP_Height] = {
	White, White, White,
	White, Blank, White,
	White, White, White,
	White, Blank, Blank,
	White, Blank, Blank };

global_variable bitmap LetterP_Bitmap = { (u32*)LetterP_Bitmask, LetterP_Width, LetterP_Height };


/*

Q

***
* *
* *
* *
** *


*/

#define LetterQ_Width 4
#define LetterQ_Height 5

global_variable color LetterQ_Bitmask[LetterQ_Width * LetterQ_Height] = {
	White, White, White, Blank,
	White, Blank, White, Blank,
	White, Blank, White, Blank,
	White, Blank, White, Blank,
	White, White, Blank, White };

global_variable bitmap LetterQ_Bitmap = { (u32*)LetterQ_Bitmask, LetterQ_Width, LetterQ_Height };


/*

R

***
* *
***
* *
*  *


*/

#define LetterR_Width 3
#define LetterR_Height 5

global_variable color LetterR_Bitmask[LetterR_Width * LetterR_Height] = {
	White, White, White,
	White, Blank, White,
	White, White, Blank,
	White, Blank, White,
	White, Blank, White };

global_variable bitmap LetterR_Bitmap = { (u32*)LetterR_Bitmask, LetterR_Width, LetterR_Height };



/*

S

 **
*
***
  *
**


*/

#define LetterS_Width 4
#define LetterS_Height 5

global_variable color LetterS_Bitmask[LetterS_Width * LetterS_Height] = {
	Blank, White, White, White,
	White, Blank, Blank, Blank,
	White, White, White, Blank,
	Blank, Blank, Blank, White,
	White, White, White, Blank };

global_variable bitmap LetterS_Bitmap = { (u32*)LetterS_Bitmask, LetterS_Width, LetterS_Height };


/*

T

***
 *
 *
 *
 *


*/

#define LetterT_Width 3
#define LetterT_Height 5

global_variable color LetterT_Bitmask[LetterT_Width * LetterT_Height] = {
	White, White, White,
	Blank, White, Blank,
	Blank, White, Blank,
	Blank, White, Blank,
	Blank, White, Blank };

global_variable bitmap LetterT_Bitmap = { (u32*)LetterT_Bitmask, LetterT_Width, LetterT_Height };


/*

U

* *
* *
* *
* *
***


*/

#define LetterU_Width 3
#define LetterU_Height 5

global_variable color LetterU_Bitmask[LetterU_Width * LetterU_Height] = {
	White, Blank, White,
	White, Blank, White,
	White, Blank, White,
	White, Blank, White,
	White, White, White };

global_variable bitmap LetterU_Bitmap = { (u32*)LetterU_Bitmask, LetterU_Width, LetterU_Height };


/*

V


* *
* *
* *
* *
 *


*/

#define LetterV_Width 3
#define LetterV_Height 5

global_variable color LetterV_Bitmask[LetterV_Width * LetterV_Height] = {
	White, Blank, White,
	White, Blank, White,
	White, Blank, White,
	White, Blank, White,
	Blank, White, Blank };

global_variable bitmap LetterV_Bitmap = { (u32*)LetterV_Bitmask, LetterV_Width, LetterV_Height };


/*

W


* * *
* * *
** **


*/

#define LetterW_Width 5
#define LetterW_Height 5

global_variable color LetterW_Bitmask[LetterW_Width * LetterW_Height] = {
	White, Blank, White, Blank, White,
	White, Blank, White, Blank, White,
	White, Blank, White, Blank, White,
	White, Blank, White, Blank, White,
	White, White, Blank, White, White };

global_variable bitmap LetterW_Bitmap = { (u32*)LetterW_Bitmask, LetterW_Width, LetterW_Height };


/*

X


* *
* *
 *
* *
* *


*/

#define LetterX_Width 3
#define LetterX_Height 5

global_variable color LetterX_Bitmask[LetterX_Width * LetterX_Height] = {
	White, Blank, White,
	White, Blank, White,
	Blank, White, Blank,
	White, Blank, White,
	White, Blank, White };

global_variable bitmap LetterX_Bitmap = { (u32*)LetterX_Bitmask, LetterX_Width, LetterX_Height };


/*

Y


* *
* *
 *
 *
 *


*/

#define LetterY_Width 3
#define LetterY_Height 5

global_variable color LetterY_Bitmask[LetterY_Width * LetterY_Height] = {
	White, Blank, White,
	White, Blank, White,
	Blank, White, Blank,
	Blank, White, Blank,
	Blank, White, Blank };

global_variable bitmap LetterY_Bitmap = { (u32*)LetterY_Bitmask, LetterY_Width, LetterY_Height };


/*

Z

***
  *
 *
*
***


*/

#define LetterZ_Width 3
#define LetterZ_Height 5

global_variable color LetterZ_Bitmask[LetterZ_Width * LetterZ_Height] = {
	White, White, White,
	Blank, Blank, White,
	Blank, White, Blank,
	White, Blank, Blank,
	White, White, White };

global_variable bitmap LetterZ_Bitmap = { (u32*)LetterZ_Bitmask, LetterZ_Width, LetterZ_Height };



global_variable bitmap Letter_Bitmaps[26] = { LetterA_Bitmap, LetterB_Bitmap, LetterC_Bitmap, LetterD_Bitmap,
									   LetterE_Bitmap, LetterF_Bitmap, LetterG_Bitmap, LetterH_Bitmap,
									   LetterI_Bitmap, LetterJ_Bitmap, LetterK_Bitmap, LetterL_Bitmap,
									   LetterM_Bitmap, LetterN_Bitmap, LetterO_Bitmap, LetterP_Bitmap,
									   LetterQ_Bitmap, LetterR_Bitmap, LetterS_Bitmap, LetterT_Bitmap,
									   LetterU_Bitmap, LetterV_Bitmap, LetterW_Bitmap, LetterX_Bitmap,
									   LetterY_Bitmap, LetterZ_Bitmap };

global_variable bitmap Number_Bitmaps[10] = { Number0_Bitmap, Number1_Bitmap, Number2_Bitmap,
									   Number3_Bitmap, Number4_Bitmap, Number5_Bitmap,
									   Number6_Bitmap, Number7_Bitmap, Number8_Bitmap,
									   Number9_Bitmap };