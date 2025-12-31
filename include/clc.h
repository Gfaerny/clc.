/// no need for pragma once :>
// 
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <sys/types.h>
#include <string>
#include <printf.h>

#define GLYPH_MINIMAL
#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL
extern "C" 
{
#include "./RGFW/RGFW.h"
#include "./glyph/glyph.h"
};
#include <GL/gl.h>

/// this easy to use function just writed by IshaqKassam
/// so i define it here for now(2025:dec:6)

inline void drawCircle(float cx, float cy, float r, int num_segments)
{
    float theta = 3.1415926 * 2 / float(num_segments);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float x = r;//we start at angle = 0 

    float y = 0;
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);//output vertex 

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -y;
        float ty = x;

        //add the tangential vector 

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        //correct using the radial factor 

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}
