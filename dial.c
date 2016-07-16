#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <math.h>

/* Program to draw a dial.

Opicks up data from environment variables.
infile	- fileto use as base
outfile - Otherwise stdout
x y location of center
thetamin - angle corresponding to minimum value - default 225
thetamax - angle coressponding to maximmu value - default -45
min - min value - default 0
max - maximum value	- default 2500
value - value to be displayed
inner - inner radius of needle
outer - outer radius of needle - default 75
width - width at base of needle - default 5
colour - rgb colour of needle. Either decimal or 0xRRGGBB hex.
*/

#ifdef linux
#define EXISTING "/root/dial-2500.gif"
#define OUTPUT "/tmp/dial.gif"
#else
#define EXISTING "/Users/Martin/Documents/Naturalwatt/Dashboard/Dial2500.gif"
#define OUTPUT "/tmp/dial.gif"
#endif
#define PI 3.14158265358979
#define R(x) ((x&0x00ff0000) >> 16)
#define B(x) ((x&0x0000FF00) >> 8)
#define G(x) (x) &0xFF
#define DEBUG if(debug)

int debug  = 0;
int main (int argc, const char * argv[]) {
    // insert code here...
	float start = 225.0;	// angle corresponding to minumum value
	float end = -45.0;		// angle corresponding to maximum value
	float min = 0.0;		// minimum value to be displayed
	float max = 2500.0;		// maximum value
	float value = 0.0;		// value to be displayed
	float angle;
	float inner = 17.0;			// inner radius of needle
	float outer = 95.0;			// outer radius of needle
	float width = 5.0;			// width of base of needle
	int colour = 0xff0000;	// colour of needle
	int col;
	int px = 122;			// location of center
	int py = 122;
	char * input = EXISTING;	// name of existing GIF
	char * output = OUTPUT;			// name of outputfile
	FILE * infile, * outfile;
	gdImagePtr im;
	gdPoint points[3], needle[3];		// the needle triangle
	float c,s;			// cached cos, sin
	char * cp;
	
	// Process command line options
	
	if (cp = getenv("infile"))	input = cp;
	if (cp = getenv("outfile"))	output = cp;
	if (cp = getenv("x"))	px = atoi(cp);
	if (cp = getenv("y"))	py = atoi(cp);
	if (cp = getenv("thetamin"))	start = atoi(cp);
	if (cp = getenv("thetamax"))	end = atoi(cp);
	if (cp = getenv("min"))	min = atoi(cp);
	if (cp = getenv("max"))	max = atoi(cp);
	if (cp = getenv("value"))	value = atoi(cp);
	if (cp = getenv("inner"))	inner = atoi(cp);
	if (cp = getenv("outer"))	outer = atoi(cp);
	if (cp = getenv("width"))	width = atoi(cp);
	if (cp = getenv("colour"))	colour = strtol(cp, NULL, 0);
	if (cp = getenv("debug"))	debug = atoi(cp);

	
	if(!(infile = fopen(input, "r"))) {
		perror(input);
		return 1;
	}
	if (!(im = gdImageCreateFromGif(infile))) {
		fprintf(stderr, "Not a valid GIF");
		return 2;
	}
	fclose(infile);
	// Calculate rotation
	if (value < min) value = min;
	if (value > max) value = max;
	if (min == max) {
		fprintf(stderr, "ERROR min = max\n");	return 3;
	}
	if (start == end) {
		fprintf(stderr, "ERROR start = end\n");		return 4;
	}
	angle = ((value - min) / (max - min) * (end - start) + start) * PI / 180;
	// Draw needle
	needle[0].x = inner;		needle[0].y = width / 2;
	needle[1].x = inner;		needle[1].y = - width / 2;
	needle[2].x = outer;		needle[2].y = 0;			// pointy apex
	c = cos(angle);
	s = sin(angle);
	points[0].x = px + c * needle[0].x + s * needle[0].y;	points[0].y = py - s * needle[0].x + c * needle[0].y; 
	points[1].x = px + c * needle[1].x + s * needle[1].y;	points[1].y = py - s * needle[1].x + c * needle[1].y; 
	points[2].x = px + c * needle[2].x + s * needle[2].y;	points[2].y = py - s * needle[2].x + c * needle[2].y; 
	col = gdImageColorAllocate(im, R(colour), G(colour), B(colour));
	gdImageSetAntiAliased(im, col);
	DEBUG {
		fprintf(stderr, "Red 0x%x Blue 0x%x Green 0x%x\n",  R(colour), B(colour), G(colour));
		fprintf(stderr, "Infile %s Outfile %s Value=%f Min=%f Max=%f 0min=%f 0max=%f", input, output, value, min, max,
			start, end);
		fprintf(stderr, "Needle inner=%f out=%f width=%f\n", inner, outer, width);
	}
	if (col < 0) {
		fprintf(stderr, "Failed to allocate a colour for needle");
	}
	gdImageFilledPolygon(im, points, 3, gdAntiAliased);

	if (output) {
		if (!(outfile = fopen(output, "wb"))) {
			perror(output);
			return 3;
		}
		gdImageGif(im, outfile);
		fclose(outfile);
	}
	else
		gdImageGif(im, stdout);
		
	gdImageDestroy(im);	// free up stuff
    return 0;
	
}
