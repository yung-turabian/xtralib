/**
 * @file raylib.h
 * @author yung-turabian
 * @date 4 19 2024
 * @brief Some extras for Raylib programs.
 * 
 * @copyright BSD-3-Clause
 */
#ifndef __REISLIB_RAYLIB_H__
#define __REISLIB_RAYLIB_H__

#include <reis.h>

#if defined __has_include
#	if __has_include (<raylib.h>)
#		include <raylib.h>
#	else
#		pragma message ("Raylib wasn't discovered, most likely it's not being linked")
#	endif
#	if __has_include (<raymath.h>)
#		include <raymath.h>
#	else
#		pragma message ("Raymath wasn't discovered, most likely it's not being linked")
#	endif
#endif



#if defined(RAYLIB_H) && defined(RAYMATH_H)
/** 
 * @todo Kinda janky still, consider making dashes all the same length.
 *
 */
void DrawDashedLineEx(Vector2 startPos, Vector2 endPos, 
										  float spacing, float thick, Color color);

#endif

#endif

