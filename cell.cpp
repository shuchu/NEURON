#include "cell.h"

//assume the order of lines are fixed.
void Synapse::draw(bool via_point)
{
	Point_3& nFrom = m_from_nueron->position();
	Point_3& nTo = m_to_nueron->position();
	// from nueron to syna
	glBegin(GL_LINE_STRIP);
	glVertex3i(nFrom[0],nFrom[1],nFrom[2]);
	glVertex3i(m_pos[0],m_pos[1],m_pos[2]);

	if(via_point){
		glVertex3i(m_via[0],m_via[1],m_via[2]);
	}
	glVertex3i(nTo[0],nTo[1],nTo[2]);
	glEnd();

	//draw Synapse object
	drawCube(m_pos[0],m_pos[1],m_pos[2],1.0);
      if (via_point)
        drawCube(m_via[0],m_via[1],m_via[2],1.0);
	
};
