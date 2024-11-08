#include <xtra/raylib.h>
#include <stdio.h>

#if defined(RAYLIB_H) && defined(RAYMATH_H)

void 
DrawDashedLine(Vector2 startPos, Vector2 endPos, float thick, Color color)
{
    float length = Vector2Distance(startPos, endPos);
		
		float dashLength = 20;

    int dashes = length / dashLength;
    //dashLength -= spaceLength;

    Vector2 dashStart = startPos;
    Vector2 dashEnd;
		
		
    for(int i = 0; i < dashes; i++)
    {
        dashEnd = Vector2MoveTowards(dashStart, endPos, dashLength);
        DrawLineEx(dashStart, dashEnd, thick, color);

        dashStart = Vector2MoveTowards(dashEnd, endPos, 10);
    }
}

void 
DrawDashedLineEx(Vector2 startPos, Vector2 endPos, 
								 float spacing, float thick, Color color)
{
		// Defaults
		if(spacing <= 0)
		{
				spacing = 10;
		}
    float length = Vector2Distance(startPos, endPos);
		
		float dashLength = spacing * 2.5f;

    int dashes = length / dashLength;
    //dashLength -= spaceLength;

    Vector2 dashStart = startPos;
    Vector2 dashEnd;
		
		
    for(int i = 0; i < dashes; i++)
    {
        dashEnd = Vector2MoveTowards(dashStart, endPos, dashLength);
        DrawLineEx(dashStart, dashEnd, thick, color);

        dashStart = Vector2MoveTowards(dashEnd, endPos, spacing);
    }
}

#endif
