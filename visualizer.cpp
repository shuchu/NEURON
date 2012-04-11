#include <algorithm>
#include <bitset>

#include "visualizer.h"
#include "cell.h"

Viewer::Viewer(QWidget* parent)	: QGLViewer(parent)
{ 

  std::cout << "Welcome to the NUERON Viewer!" << std::endl;

  m_aabb = 0;
  m_den = 0;
  m_axon = 0;

  m_magic_number = 0;
  m_P = 0; // 000000000001
  m_N = 0; // 000000000010
  m_G = 0; // 000000000100
  m_B = 0; // 000000001000
  m_A = 0; // 000000010000

  m_S = 0; // 000000100000
  m_T = 0; // 000001000000
  m_I = 0; // 000010000000
  m_C = 0; // 000100000000
  m_M = 0; // 001000000000

  m_R = 0; // 010000000000
  m_D = 0; // 100000000000

  m_cm = NULL;
};

//initialization
void Viewer::init()
{
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
  glEnable(GL_CULL_FACE);

  //antialiasing
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
  glLineWidth(1.5);

  setBackgroundColor(QColor(0,71,125));
  setTextIsEnabled(false);
};


//main draw functions
void Viewer::draw()
{
  //Point_3 length = m_cm->bbox_top_right() - m_cm->bbox_bottom_left();
  //int max_length = length[0];
  //if (max_length < length[1])
  //  max_length = length[1];
  //if (max_length < length[2])
  //  max_length = length[2];

  //float ss = (float) 1/ max_length;
  ////glScalef(ss,ss,ss);

 
  //show AABB 
  if( m_aabb != 0 ){
    glColor3f(1.0,1.0,1.0);
    m_cm->draw_AABB();
  }

  //show different Cells
 
  for (CellModel::Nueron_iterator ni = m_cm->nuerons_begin();
      ni != m_cm->nuerons_end(); ni++)
  {
    Nueron* n_ptr = *ni;
    int type = (1 << n_ptr->type());
    if (m_magic_number & type) {
		switch (type) {
		case 1:
			glColor4f(0.2,0.3,0.5,1.0); break;
		case 2:
			glColor4f(0.4,0.1,0.8,1.0); break;
		case 4:
			glColor4f(0.7,0.3,0.4,1.0); break;
		case 8:
			glColor4f(0.3,0.4,0.3,1.0); break;
		case 16:
			glColor4f(0.8,0.9,0.6,1.0); break;
		case 32:
			glColor4f(0.9,0.8,0.9,1.0); break;
		case 64:
			glColor4f(0.2,0.1,0.4,1.0); break;
		case 128:
			glColor4f(0.5,0.2,0.6,1.0); break;
		case 256:
			glColor4f(0.6,0.3,0.3,1.0); break;
		case 512:
			glColor4f(0.7,0.4,0.7,1.0); break;
		case 1024:
			glColor4f(0.1,0.5,0.4,1.0); break;
		case 2048:
			glColor4f(0.0,0.3,0.9,1.0); break;
		default:
			glColor4f(1.0,1.0,1.0,1.0); break;
		}
      //draw text
	  //glColor4f(1.0,1.0,1.0,1.0);
      qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(m_frames[n_ptr->id()].position());
      drawText((int)screenPos[0],(int)screenPos[1],(const QString)m_cm->nueron_type(n_ptr->type()));

      n_ptr->draw_soma();

	  if (m_den){
		  glColor4f(0.0,1.0,0.0,1.0);
		  n_ptr->draw_dendrites();
	  }

	  if (m_axon){
		  glColor4f(0.0,0.0,1.0,1.0);
		  n_ptr->draw_axons();
	  }

	  if (m_syn_in){
		  glColor4f(0.0,0.5,0.5,1.0);
		 n_ptr->draw_input_synapse(m_syn_via);
	  };

	  if (m_syn_out){
		  glColor4f(0.5,0.5,0.0,1.0);
		  n_ptr->draw_output_synapse(m_syn_via);
	  }

    }
  }

}


//main draw functions
void Viewer::fastDraw()
{
  //show AABB 
  if( m_aabb != 0 ){
    glColor3f(1.0,1.0,1.0);
    m_cm->draw_AABB();
  }

  //show different Cells
 
  for (CellModel::Nueron_iterator ni = m_cm->nuerons_begin();
      ni != m_cm->nuerons_end(); ni++)
  {
    Nueron* n_ptr = *ni;
    int type = (1 << n_ptr->type());
    if (m_magic_number & type) {
      //draw text
	  //glColor4f(1.0,1.0,1.0,1.0);
      //qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(m_frames[n_ptr->id()].position());
      //drawText((int)screenPos[0],(int)screenPos[1],(const QString)m_cm->nueron_type(n_ptr->type()));

      n_ptr->draw_soma();

	  if (m_den){
		  glColor4f(0.0,1.0,0.0,1.0);
		  n_ptr->draw_dendrites();
	  }

	  if (m_axon){
		  glColor4f(0.0,0.0,1.0,1.0);
		  n_ptr->draw_axons();
	  }

	  if (m_syn_in){
		  glColor4f(0.0,0.5,0.5,1.0);
		 n_ptr->draw_input_synapse(m_syn_via);
	  };

	  if (m_syn_out){
		  glColor4f(0.5,0.5,0.0,1.0);
		  n_ptr->draw_output_synapse(m_syn_via);
	  }

    }
  }

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

void Viewer::build_frames()
{
  //build frames
  if (m_cm != NULL)
  {
    m_frames = new qglviewer::Frame[m_cm->size_of_nuerons()];
    
    for (CellModel::Nueron_iterator ni = m_cm->nuerons_begin();
         ni != m_cm->nuerons_end(); ni++)
    {
      Nueron* n_ptr = *ni;
      Soma* s_ptr = n_ptr->soma();
      Point_3 pos = s_ptr->get_position();
      m_frames[n_ptr->id()].setPosition((float)pos[0],(float)pos[1],(float)pos[2]);
    }   
  }

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

void Viewer::aabbBoxState(int state)
{
  m_aabb = state;
};


// fixed values for calculating Magic_Number
//  int m_P; // 000000000001 1
//	int m_N; // 000000000010 2
//	int m_G; // 000000000100 4
//	int m_B; // 000000001000 8
//	int m_A; // 000000010000 16
//
//	int m_S; // 000000100000 32
//	int m_T; // 000001000000 64
//	int m_I; // 000010000000 128
//  int m_C; // 000100000000 256
//	int m_M; // 001000000000 512
//
//	int m_R; // 010000000000 1024
//	int m_D; // 100000000000 2048
void Viewer::update_magic_number()
{
  m_magic_number = 0;
  if ( m_P != 0 )
    m_magic_number |= 1;

  if ( m_N != 0 )
    m_magic_number |= 2;

  if ( m_G != 0 )
    m_magic_number |= 4;

  if ( m_B != 0 )
    m_magic_number |= 8;

  if ( m_A != 0 )
    m_magic_number |= 16;

  if ( m_S != 0 )
    m_magic_number |= 32;

  if ( m_T != 0 )
    m_magic_number |= 64;

  if ( m_I != 0 )
    m_magic_number |= 128;

  if ( m_C != 0 )
    m_magic_number |= 256;

  if ( m_M != 0 )
    m_magic_number |= 512;

  if ( m_R != 0 )
    m_magic_number |= 1024;

  if ( m_D != 0 )
    m_magic_number |= 2048;

  //debug
  //std::cout << "magic number is: " << m_magic_number << std::endl;
};

void Viewer::show_P(int state)
{
  m_P = state;
  this->update_magic_number();
};

void Viewer::show_N(int state)
{
  m_N = state; 
  this->update_magic_number();
};

void Viewer::show_G(int state)
{
  m_G = state;
  this->update_magic_number();
};

void Viewer::show_B(int state)
{
  m_B = state;
  this->update_magic_number();
};

void Viewer::show_A(int state)
{
  m_A = state;
  this->update_magic_number();
};

void Viewer::show_S(int state)
{
  m_S = state;
  this->update_magic_number();
};

void Viewer::show_T(int state)
{
  m_T = state;
  this->update_magic_number();
};

void Viewer::show_I(int state)
{
  m_I = state;
  this->update_magic_number();
};

void Viewer::show_C(int state)
{
  m_C = state;
  this->update_magic_number();
};

void Viewer::show_M(int state)
{
  m_M = state;
  this->update_magic_number();
};

void Viewer::show_R(int state)
{
  m_R = state;
  this->update_magic_number();
};

void Viewer::show_D(int state)
{
  m_D = state;
  this->update_magic_number();
};
 
void Viewer::show_den(int state)
{
	m_den = state;
};
void Viewer::show_axon(int state)
{
	m_axon = state;
};

void Viewer::update_scene_radius()
{
  if (m_cm != NULL) {
	Point_3 low = m_cm->bbox_bottom_left();
  	Point_3 high = m_cm->bbox_top_right();

  	int radius = high[0] - low[0];
  	if (radius < (high[1] - low[1]))
  		radius = high[1] - low[1];
  	if (radius < (high[2] - low[2]))
  	    radius = high[2] - low[2];
  	setSceneRadius(2.0*double(radius));
	qglviewer::Vec camera_pos((double)(high[0]+10),(double)(high[1]+10),(double)(high[2]+10));
	camera()->setPosition(camera_pos);

	std::cout << "scene radius: " << sceneRadius() << std::endl;
	std::cout << "camera position: " << camera_pos.x << " " << camera_pos.y << " " << camera_pos.z  << std::endl;
  }
};

void Viewer::update_scene()
{
	this->build_frames();
	this->update_scene_radius();
};

void Viewer::show_syn(int state)
{
	m_syn = state;
}; 
	
void Viewer::show_syn_via(bool state)
{
	m_syn_via = state;
}; 

void Viewer::show_syn_in(int state)
{
	m_syn_in = state;
}

void Viewer::show_syn_out(int state)
{
	m_syn_out = state;
};

void Viewer::load_status()
{
		std::cout << "reload the parameters of the scence" << std::endl;

	// / Add a manipulated frame to the viewer.
	setManipulatedFrame(new qglviewer::ManipulatedFrame());
	if (!restoreStateFromFile())
		showEntireScene(); // Previous state cannot be restored: fit camera to scene.

};