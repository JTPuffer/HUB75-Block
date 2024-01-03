#include "math.hpp"
#include <math.h>
#include "pico/stdlib.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "drivers/hub75/hub75.hpp"


#define WIDTH 64
#define HEIGHT 32

#define NUM_OF_DATA 8


float vertices[] = { // positions   // normals // texture coords
   	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f, 
     0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 	1.0f, 0.0f,
     0.5f, 	0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 	1.0f, 1.0f,
     0.5f, 	0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 	1.0f, 1.0f,
   	-0.5f, 	0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 	0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f, 	0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f, 	0.0f, 0.0f, 
	 0.5f, -0.5f,  0.5f, 	0.0f,  0.0f,  1.0f, 	1.0f, 0.0f, 
	 0.5f, 	0.5f,  0.5f, 	0.0f,  0.0f,  1.0f, 	1.0f, 1.0f,
	 0.5f, 	0.5f,  0.5f, 	0.0f,  0.0f,  1.0f, 	1.0f, 1.0f, 
	-0.5f, 	0.5f,  0.5f, 	0.0f,  0.0f,  1.0f, 	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,
    -0.5f, 	0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 	1.0f, 0.0f, 
	-0.5f, 	0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 	0.0f, 1.0f, 
	-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,	 	0.0f, 0.0f,
    -0.5f, 	0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 	1.0f, 0.0f,
	 0.5f, 	0.5f,  0.5f, 	1.0f,  0.0f,  0.0f, 	1.0f, 0.0f, 
	 0.5f, 	0.5f, -0.5f, 	1.0f,  0.0f,  0.0f, 	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 	1.0f,  0.0f,  0.0f, 	0.0f, 1.0f, 
	 0.5f, -0.5f, -0.5f, 	1.0f,  0.0f,  0.0f, 	0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 	1.0f,  0.0f,  0.0f, 	0.0f, 0.0f,
	 0.5f, 	0.5f,  0.5f, 	1.0f,  0.0f,  0.0f, 	1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 	0.0f, -1.0f,  0.0f, 	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 	0.0f, -1.0f,  0.0f, 	1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 	0.0f, -1.0f,  0.0f, 	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 	0.0f, -1.0f,  0.0f, 	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 	0.0f, -1.0f,  0.0f, 	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 	0.0f, -1.0f,  0.0f, 	0.0f, 1.0f,
    -0.5f, 	0.5f, -0.5f, 	0.0f,  1.0f,  0.0f, 	0.0f, 1.0f,
	 0.5f, 	0.5f, -0.5f, 	0.0f,  1.0f,  0.0f, 	1.0f, 1.0f, 
	 0.5f, 	0.5f,  0.5f, 	0.0f,  1.0f,  0.0f, 	1.0f, 0.0f, 
	 0.5f, 	0.5f,  0.5f, 	0.0f,  1.0f,  0.0f, 	1.0f, 0.0f, 
	-0.5f, 	0.5f,  0.5f, 	0.0f,  1.0f,  0.0f, 	0.0f, 0.0f, 
	-0.5f, 	0.5f, -0.5f, 	0.0f,  1.0f,  0.0f, 	0.0f, 1.0f
};

using namespace pimoroni;

const uint8_t QTY_BALLS = 13;

Hub75 hub75(WIDTH, HEIGHT, nullptr, PANEL_FM6126A, false);

PicoGraphics_PenRGB888 graphics(hub75.width, hub75.height, nullptr);

// Callback for the dma interrupt (required)
void __isr dma_complete() {
  	hub75.dma_complete();
}

double distance(const Point &p1, const Point &p2) {
    int32_t dx = p2.x - p1.x;
    int32_t dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

    // Function to draw a line between two points using pixel_span
void draw_line(const Point &p1, const Point &p2) {
	    double len = distance(p1, p2);

        int32_t dx = p2.x - p1.x;
        int32_t dy = p2.y - p1.y;

        double unitX = dx / len;
        double unitY = dy / len;

        for (int32_t i = 0; i <= len; ++i) {
            int32_t px = static_cast<int32_t>(p1.x + unitX * i);
            int32_t py = static_cast<int32_t>(p1.y + unitY * i);

            // Draw pixel at the current point
			if(px < WIDTH && py < HEIGHT)
            graphics.set_pixel(Point(px, py));
        }
}
bob::Vector4<float> convertPoint(int row, bob::Matrix4<float> & theta){
	bob::Vector4<float> inputVector(vertices[row],vertices[row+1],vertices[row+2],0);
	bob::Vector4<float> rotatedVectorY =  theta *inputVector;

	rotatedVectorY.z= rotatedVectorY.z+1.1;

	rotatedVectorY.x = ((rotatedVectorY.x * HEIGHT/WIDTH)/rotatedVectorY.z)+1;
	rotatedVectorY.y = (rotatedVectorY.y/rotatedVectorY.z)+1;
	// convert to cartesian
	rotatedVectorY.x *= WIDTH/2;
	rotatedVectorY.y *=HEIGHT/2;
	return rotatedVectorY;
}

struct triangle_struct
{
    bob::Vector4<float> one;
    bob::Vector4<float> two;
    bob::Vector4<float> three;
    float depth;
	Pen colour;
};


int main() {
	stdio_init_all();
	hub75.start(dma_complete);

	Point text_location(0, 0);
	Pen BG = graphics.create_pen(0, 0, 0);
    Pen pens[12] = {
        graphics.create_pen(255, 255, 255),    // White (Background)
        graphics.create_pen(255, 0, 0),         // Red
        graphics.create_pen(0, 255, 0),         // Lime Green
        graphics.create_pen(0, 0, 255),         // Blue
        graphics.create_pen(255, 255, 0),       // Yellow
        graphics.create_pen(255, 0, 255),       // Magenta
        graphics.create_pen(0, 255, 255),       // Cyan
        graphics.create_pen(128, 0, 128),       // Purple
        graphics.create_pen(0, 128, 128),       // Teal
        graphics.create_pen(255, 182, 193),     // Light Pink
        graphics.create_pen(0, 128, 0),         // Green
        graphics.create_pen(128, 128, 0)        // Olive
        // Add more pens as needed
    };

    std::vector<triangle_struct> triangles;

	double theta =0.01;
	while(true) {
		graphics.set_pen(BG);
		graphics.clear();
		theta +=0.001;
	    bob::Matrix4<float> rotationMatrixX = bob::Matrix4<float>::rotateX(theta);
	    bob::Matrix4<float> rotationMatrixY = bob::Matrix4<float>::rotateY(theta *0.5);
	    bob::Matrix4<float> rotateed =  rotationMatrixY * rotationMatrixX;
		triangles.clear();
		for(int i =0 ;i < 36 ;i+=3){
			triangle_struct t ;
			t.one = convertPoint(i *8,rotateed);
			t.two = convertPoint((i+1) * 8,rotateed);
			t.three = convertPoint((i+2) *8,rotateed);
			t.depth = (t.one.z +t.two.z +t.three.z)/3;
			t.colour = pens[i/3];
			triangles.push_back(t);
		}
        std::sort(triangles.begin(), triangles.end(), [](const triangle_struct& a, const triangle_struct& b) {
            return a.depth > b.depth; // Sort in descending order
        });
		int i =0;
		for(triangle_struct t : triangles){
			graphics.set_pen(t.colour);
			graphics.triangle(Point(t.one.x,t.one.y),Point(t.two.x,t.two.y),Point(t.three.x,t.three.y));
			i+=3;
		}
		// update screen
		hub75.update(&graphics);

	}

	return 0;
}
