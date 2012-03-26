#include "cell.h"

void Synapse::draw()
{
      glBegin(GL_LINE_STRIP);

      // FROM to SYNAPSE
      Point_3 from_center = m_from_nueron->soma()->get_position();
      glVertex3i(from_center[0],from_center[1],from_center[2]);
      glVertex3i(m_pos[0],m_pos[1],m_pos[2]);  

      // SYNAPSE to VIA (if any)
      if(m_via_point){
        glVertex3i(m_via[0],m_via[1],m_via[2]);
      }
      
      // to TO
      Point_3 to_center = m_to_nueron->soma()->get_position();
      glVertex3i(to_center[0],to_center[1],to_center[2]);
      glEnd();  
};