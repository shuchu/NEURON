#include <algorithm>

#include "visualizer.h"
#include "cell.h"

//initialization
void Viewer::init()
{
	glEnable(GL_DEPTH_TEST);
}


//main draw functions
void Viewer::draw()
{
	glColor3f(1.0,0.0,0.0);
	//glMatrixMode(GL_PROJECTION);
	/*glPushMatrix();
	glLoadIdentity();*/

	//Point_3 center = m_cm->bbox_bottom_left();
	
	Point_3 length = m_cm->bbox_top_right() - m_cm->bbox_bottom_left();
	int max_length = length[0];
	if (max_length < length[1])
		max_length = length[1];
	if (max_length < length[2])
		max_length = length[2];

	float ss = (float) 1/ max_length;
	glScalef(ss,ss,ss);
	//glTranslatef(center[0],center[1],center[2]);

	//debug
	//std::cout << "ss: " << ss <<std::endl;

	m_cm->draw_AABB();
	m_cm->draw_nuerons();	
	//glutWireTeapot(1.0);
	
	/*glPopMatrix();*/
}

//help
QString Viewer::helpString() const
{
  QString text("<h2>NEURON Visualizer</h2>");
  return text;
}

void Viewer::load_cellModel(CellModel* cm)
{
	m_cm = cm;
};

void Viewer::draw_box(int x, int y, int z, float scale)
{
	float lx = x - scale*0.5;
	float ly = y - scale*0.5;
	float lz = z - scale*0.5;
	float rx = x + scale*0.5;
	float ry = y + scale*0.5;
	float rz = z + scale*0.5;

	this->draw_box(lx,ly,lz,rx,ry,rz);
};

void Viewer::draw_box(int lx, int ly, int lz, int rx, int ry, int rz)
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
