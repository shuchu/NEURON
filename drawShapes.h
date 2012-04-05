#ifndef __DRAWSHAPES_H__
#define __DRAWSHAPES_H__
#include <GL/GL.h>
#include <GL/GLU.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

void drawCube(int lx, int ly, int lz, int rx, int ry, int rz)
{
	  glBegin(GL_QUADS);
	  glVertex3i(lx,ly,ly);
      glVertex3i(lx,ly,rz);
      glVertex3i(rx,ly,rz);
      glVertex3i(rx,ly,lz);

      glVertex3i(lx,ry,lz);
      glVertex3i(rx,ry,lz);
      glVertex3i(rx,ry,rz);
      glVertex3i(lx,ry,rz);

      glVertex3i(lx,ly,lz);
      glVertex3i(rx,ly,lz);
      glVertex3i(rx,ry,lz);
      glVertex3i(lx,ry,lz);
      
      glVertex3i(rx,ly,lz);
      glVertex3i(rx,ly,lz);
      glVertex3i(rx,ry,rz);
      glVertex3i(rx,ry,lz);
      
      glVertex3i(rx,ly,rz);
      glVertex3i(lx,ly,rz);
      glVertex3i(lx,ry,rz);
      glVertex3i(rx,ry,rz);
      
      glVertex3i(lx,ly,rz);
      glVertex3i(lx,ly,lz);
      glVertex3i(lx,ry,lz);
      glVertex3i(lx,ry,rz);
	  glEnd();

	//glBegin(GL_LINE_LOOP);
	//glVertex3i(lx,ly,lz);
	//glVertex3i(rx,ly,lz);
	//glVertex3i(rx,ly,rz);
	//glVertex3i(lx,ly,rz);
 //   glEnd();

	//glBegin(GL_LINE_LOOP);
	//glVertex3i(lx,ry,lz);
	//glVertex3i(rx,ry,lz);
	//glVertex3i(rx,ry,rz);
	//glVertex3i(lx,ry,rz);
 //   glEnd();
	//
	//glBegin(GL_LINES);
	//glVertex3i(lx,ly,lz);
	//glVertex3i(lx,ry,lz);
	//
 //   glVertex3i(rx,ly,lz);
	//glVertex3i(rx,ry,lz);
 //   
 //   glVertex3i(rx,ly,rz);
	//glVertex3i(rx,ry,rz);
	//
 //   glVertex3i(lx,ly,rz);
	//glVertex3i(lx,ry,rz);
 //   glEnd();
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

static void
doughnut(GLint x, GLint y, GLint z, GLfloat r, GLfloat R, GLint nsides, GLint rings)
{
  int i, j;
  GLfloat theta, phi, theta1;
  GLfloat cosTheta, sinTheta;
  GLfloat cosTheta1, sinTheta1;
  GLfloat ringDelta, sideDelta;

  ringDelta = 2.0 * M_PI / rings;
  sideDelta = 2.0 * M_PI / nsides;

  theta = 0.0;
  cosTheta = 1.0;
  sinTheta = 0.0;
  for (i = rings - 1; i >= 0; i--) {
    theta1 = theta + ringDelta;
    cosTheta1 = cos(theta1);
    sinTheta1 = sin(theta1);
    glBegin(GL_QUAD_STRIP);
    phi = 0.0;
    for (j = nsides; j >= 0; j--) {
      GLfloat cosPhi, sinPhi, dist;

      phi += sideDelta;
      cosPhi = cos(phi);
      sinPhi = sin(phi);
      dist = R + r * cosPhi;

	  glNormal3f(x+cosTheta1 * cosPhi, y-sinTheta1 * cosPhi, z+sinPhi);
      glVertex3f(x+cosTheta1 * dist, y-sinTheta1 * dist, z + r * sinPhi);
      glNormal3f(x+cosTheta * cosPhi, y-sinTheta * cosPhi, z + sinPhi);
      glVertex3f(x+cosTheta * dist, y-sinTheta * dist,  z + r * sinPhi);

     /* glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
      glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
      glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
      glVertex3f(cosTheta * dist, -sinTheta * dist,  r * sinPhi);*/
    }
    glEnd();
    theta = theta1;
    cosTheta = cosTheta1;
    sinTheta = sinTheta1;
  }
}



#endif