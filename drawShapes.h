#ifndef __DRAWSHAPES_H__
#define __DRAWSHAPES_H__
#include <GL/GL.h>

void drawCube(int lx, int ly, int lz, int rx, int ry, int rz)
{
	glBegin(GL_LINE_LOOP);
	glVertex3i(lx,ly,lz);
	glVertex3i(rx,ly,lz);
	glVertex3i(rx,ly,rz);
	glVertex3i(lx,ly,rz);
    glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3i(lx,ry,lz);
	glVertex3i(rx,ry,lz);
	glVertex3i(rx,ry,rz);
	glVertex3i(lx,ry,rz);
    glEnd();
	
	glBegin(GL_LINES);
	glVertex3i(lx,ly,lz);
	glVertex3i(lx,ry,lz);
	
    glVertex3i(rx,ly,lz);
	glVertex3i(rx,ry,lz);
    
    glVertex3i(rx,ly,rz);
	glVertex3i(rx,ry,rz);
	
    glVertex3i(lx,ly,rz);
	glVertex3i(lx,ry,rz);
    glEnd();
};

void drawCube(int x, int y, int z, float scale)
{
	float lx = x - scale*0.5;
	float ly = y - scale*0.5;
	float lz = z - scale*0.5;
	float rx = x + scale*0.5;
	float ry = y + scale*0.5;
	float rz = z + scale*0.5;

	drawCube(lx,ly,lz,rx,ry,rz);
};



#endif