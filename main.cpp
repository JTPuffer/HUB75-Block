#include <math.h>
#include "pico/stdlib.h"
#include <stdio.h>

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
            graphics.set_pixel(Point(px, py));
        }
}

Point convertPoint(int row){
	float carC= vertices[row+2]+1.1;

	float carA = (vertices[row]/carC)+1;
	float carB = (vertices[row+1]/carC)+1;
	// convert to cartesian
	carA *= WIDTH/2;
	carB *=HEIGHT/2;
	printf("%f %f \n",carA,carB);
	return Point(carA,carB);
}

int main() {
	stdio_init_all();
	hub75.start(dma_complete);



	Point text_location(0, 0);
	Pen BG = graphics.create_pen(0, 0, 0);
	Pen red = graphics.create_pen(125, 0, 0);

	while(true) {
		graphics.set_pen(BG);
		graphics.clear();
		for(int i =0 ;i < 36 ;i+=3){

			graphics.set_pen(red);
			// for some reason the graphics display starts at -1?

			//graphics.triangle(convertPoint(i *8),convertPoint((i+1) * 8),convertPoint((i+2) *8));
			// graphics.set_pixel(convertPoint((i+1) * 8));
			// graphics.set_pixel(convertPoint(i *8));
			// graphics.set_pixel(convertPoint((i+2) *8));

			draw_line(convertPoint((i+1) * 8),convertPoint(i *8));
			draw_line(convertPoint(i *8),convertPoint((i+2) *8));
			draw_line(convertPoint((i+2) * 8),convertPoint((i+1) * 8));
			hub75.update(&graphics);
			sleep_ms(1000 );
			

		}
//Point(((vertices[i*24] / vertices[i*24 +2]) *WIDTH /2 ) + WIDTH/2,((vertices[i*24 +1] / vertices[i*24 +2]) *HEIGHT /2) + HEIGHT/2)
		// update screen
		hub75.update(&graphics);
		sleep_ms(1000 / 30);
	}

	return 0;
}