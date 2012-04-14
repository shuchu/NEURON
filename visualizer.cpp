#include <algorithm>
#include <bitset>
#include <QMouseEvent>

#include "visualizer.h"
#include "cell.h"

Viewer::Viewer(QWidget* parent)	: QGLViewer(parent)
{ 

  std::cout << "Welcome to the NUERON Viewer!" << std::endl;

  m_aabb = 0;
  m_den = 0;
  m_axon = 0;
  m_axis = true;
  m_selected = false;

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

  //bounding box
  aabb_low[0] = aabb_low[1] = aabb_low[2] = 0.0;
  aabb_high[0] = aabb_high[1] = aabb_high[2] = 1.0;

  m_ortho = false;
  m_clip_show = false;
  m_clip_x[0] = m_clip_x[1] = m_clip_x[2] = m_clip_x[3] = 0.0;
  m_clip_y[0] = m_clip_y[1] = m_clip_y[2] = m_clip_y[3] = 0.0;
  m_clip_z[0] = m_clip_z[1] = m_clip_z[2] = m_clip_z[3] = 0.0;
  m_clip_x[0] = 1.0;
  m_clip_y[1] = 1.0;
  m_clip_z[2] = 1.0;

  m_cm = NULL;
};

//initialization
void Viewer::init()
{
  //restore satte
  std::cout << "State File name: " << QString(stateFileName()).toStdString() << std::endl;
  restoreStateFromFile();

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);                 // The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  //antialiasing
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
  glLineWidth(1.0);

  //lighting
  GLfloat pos[4] = {1000.0,1000.0,1000.0,0.0};
  GLfloat ka[] = {0.2f,0.2f,0.2f,1.0};
  GLfloat kd[] = {1.0,1.0,1.0,1.0};
  GLfloat ks[] = {1.0,1.0,1.0,1.0};
  glLightfv(GL_LIGHT0,GL_AMBIENT,ka);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,kd);
  glLightfv(GL_LIGHT0,GL_SPECULAR,ks);
  glLightfv(GL_LIGHT0,GL_POSITION,pos);
  glEnable(GL_LIGHT0);

  //texture
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glEnable(GL_TEXTURE_2D);

  //clip
  setManipulatedFrame(new qglviewer::ManipulatedFrame());
 // manipulatedFrame()->setConstraint(new qglviewer::ManipulatedFrameSetConstraint());
  //setBackgroundColor(QColor(0,71,125));
  setTextIsEnabled(false);

  load_texture();
};

int Viewer::set_tex_id(int& type)
{
  int tex_id = 0;
  switch (type) {
    case 1:
      tex_id = 0;break;
    case 2:
      tex_id = 1;break;
    case 4:
      tex_id = 2;break;
    case 8:
      tex_id = 3;break;
    case 16:
      tex_id = 4;break;
    case 32:
      tex_id = 5;break;
    case 64:
      tex_id = 6;break;
    case 128:
      tex_id = 7;break;
    case 256:
      tex_id = 8;break;
    case 512:
      tex_id = 9;break;
    case 1024:
      tex_id = 10;break;
    case 2048:
      tex_id = 11;break;
    default:
      tex_id = 0; break;
  } //switch

  return tex_id;
}

//main draw functions
void Viewer::draw()
{
  //draw axis
  if (m_axis) {
    glPushMatrix();
    glMultMatrixd(manipulatedFrame()->matrix());
    QGLViewer::drawAxis(sceneRadius() / 80.0); // Or any scale
    glPopMatrix();
  }

  //draw AABB 
  if( m_aabb != 0 ){
    glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glMultMatrixd(manipulatedFrame()->matrix());
    m_cm->draw_AABB();
    glPopMatrix();
  }

  //set clipping plane
  glPushMatrix();
  glMultMatrixd(manipulatedFrame()->matrix());
  glClipPlane(GL_CLIP_PLANE0, m_clip_x);
  glClipPlane(GL_CLIP_PLANE1, m_clip_y);
  glClipPlane(GL_CLIP_PLANE2, m_clip_z);
  //draw clip planes
  //

  //debug
  //glBindTexture(GL_TEXTURE_2D,m_texture[0]);
  //drawCubef(0.0,0.0,0.0,1.0,1.0,1.0);

  if (m_selected) {

    for (QList<int>::const_iterator it=selection_.begin(), end=selection_.end(); it != end; ++it){
      Nueron* n_ptr = objects_.at(*it);
      int type = (1 << n_ptr->type());
      if (m_magic_number & type) {
        int tex_id = set_tex_id(type);
        //draw text
        //glColor4f(1.0,1.0,1.0,1.0);
        //qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(m_frames[n_ptr->id()].position());
        //drawText((int)screenPos[0],(int)screenPos[1],(const QString)m_cm->nueron_type(n_ptr->type()));
        /*Point_3 p = n_ptr->soma()->get_position();
          QGLWidget::renderText(p[0],p[1],p[2], QString(n_ptr->id()));*/

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,m_texture[tex_id]);
        n_ptr->draw_soma();
        glDisable(GL_TEXTURE_2D);

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
      } //if
    }//for

  } else {

    //show different Cells
    for (CellModel::Nueron_iterator ni = m_cm->nuerons_begin();
        ni != m_cm->nuerons_end(); ni++)
    {
      Nueron* n_ptr = *ni;
      int type = (1 << n_ptr->type());
      if (m_magic_number & type) {
        int tex_id = set_tex_id(type);
        
        //draw text
        //glColor4f(1.0,1.0,1.0,1.0);
        //qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(m_frames[n_ptr->id()].position());
        //drawText((int)screenPos[0],(int)screenPos[1],(const QString)m_cm->nueron_type(n_ptr->type()));
        /*Point_3 p = n_ptr->soma()->get_position();
          QGLWidget::renderText(p[0],p[1],p[2], QString(n_ptr->id()));*/

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,m_texture[tex_id]);
        n_ptr->draw_soma();
        glDisable(GL_TEXTURE_2D);

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
  }//else


  glPopMatrix();

  // Draws rectangular selection area. Could be done in postDraw() instead.
  if (selectionMode_ != NONE)
    drawSelectionRectangle();  

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

    //scene radius
    setSceneRadius(double(radius));
    std::cout << "scene radius: " << sceneRadius() << std::endl;

    //bounding box
    //qglviewer::Vec b_min( (double)low[0],(double)low[1],(double)low[2]);
    //qglviewer::Vec b_max( (double)high[0],(double)high[1],(double)high[2]);
    //setSceneBoundingBox(b_min,b_max);

    //update scene center
    //qglviewer::Vec center(double(high[0]-low[0]),double(high[1]-low[1]),double(high[2]-low[2]));
    //setSceneCenter(center);

    showEntireScene();
  }
};

void Viewer::update_scene()
{
  //this->build_frames();
  this->update_scene_radius();
  this->update_aabb();
  this->build_select_objects();
  showEntireScene();
};

void Viewer::show_syn(int state)
{
  m_syn = state;
  updateGL();
}; 

void Viewer::show_syn_via(bool state)
{
  m_syn_via = state;updateGL();
}; 

void Viewer::show_syn_in(int state)
{
  m_syn_in = state;
  updateGL();
}

void Viewer::show_syn_out(int state)
{
  m_syn_out = state;
  updateGL();
};

void Viewer::load_status()
{
  std::cout << "reload the parameters of the scence" << std::endl;

  // / Add a manipulated frame to the viewer.
  setManipulatedFrame(new qglviewer::ManipulatedFrame());
  if (!restoreStateFromFile())
    showEntireScene(); // Previous state cannot be restored: fit camera to scene.
};

//load 12 textures from "images/*.*"
void Viewer::load_texture()
{
  //assume the textures are located at "images/*.*"
  std::cout << "I assume the textures are located at './images/'" << std::endl;
  std::cout << "start loading...... " << std::endl;

  // P 
  m_texture[0] = bindTexture(QPixmap(QString("images/P.png")),GL_TEXTURE_2D);
  m_texture[1] = bindTexture(QPixmap(QString("images/N.png")),GL_TEXTURE_2D);
  m_texture[2] = bindTexture(QPixmap(QString("images/G.png")),GL_TEXTURE_2D);
  m_texture[3] = bindTexture(QPixmap(QString("images/B.png")),GL_TEXTURE_2D);
  m_texture[4] = bindTexture(QPixmap(QString("images/A.png")),GL_TEXTURE_2D);

  m_texture[5] = bindTexture(QPixmap(QString("images/S.png")),GL_TEXTURE_2D);
  m_texture[6] = bindTexture(QPixmap(QString("images/T.png")),GL_TEXTURE_2D);
  m_texture[7] = bindTexture(QPixmap(QString("images/I.png")),GL_TEXTURE_2D);
  m_texture[8] = bindTexture(QPixmap(QString("images/C.png")),GL_TEXTURE_2D);
  m_texture[9] = bindTexture(QPixmap(QString("images/M.png")),GL_TEXTURE_2D);

  m_texture[10] = bindTexture(QPixmap(QString("images/R.png")),GL_TEXTURE_2D);
  m_texture[11] = bindTexture(QPixmap(QString("images/D.png")),GL_TEXTURE_2D);

  std::cout << "finish loading textures. " << std::endl;

  /*for (int i = 0; i < 12; ++i) {
    std::cout << "tex: "<< i << "value " << m_texture[i] << std::endl;
    }*/
};

void Viewer::show_ortho(bool state)
{
  m_ortho = state;

  //ok, orthogonal view of X
  if (m_ortho) {
    //set view direction
    qglviewer::Vec v = camera()->viewDirection();
    std::cout << "current view direction: " << v.x <<" "<< v.y << " " << v.z << std::endl;
    camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);
  } else {
    camera()->setType(qglviewer::Camera::PERSPECTIVE );
  }

};

void Viewer::set_clip_x(int value)
{
  m_clip_x[3] = aabb_low[0] + 0.001*value*(aabb_high[0] - aabb_low[0]);
  m_clip_x[3] -= 0.1f;
  m_clip_x[3] *= -1.0;
  std::cout << "Clipping plane X: "
    << m_clip_x[0] << " "
    << m_clip_x[1] << " "
    << m_clip_x[2] << " "
    << m_clip_x[3] << " "
    <<std::endl;
}

void Viewer::set_clip_y(int value)
{
  m_clip_y[3] = aabb_low[1] + 0.001*value*(aabb_high[1] - aabb_low[1]);
  m_clip_y[3] -= 0.1f;
  m_clip_y[3] *= -1.0;
  std::cout << "Clipping plane Y: "
    << m_clip_y[0] << " "
    << m_clip_y[1] << " "
    << m_clip_y[2] << " "
    << m_clip_y[3] << " "
    <<std::endl;
}

void Viewer::set_clip_z(int value)
{
  m_clip_z[3] = aabb_low[2] + 0.001*value*(aabb_high[2] - aabb_low[2]);
  m_clip_z[3] += 0.1f;
  m_clip_z[3] *= -1.0;
  std::cout << "Clipping plane Z: "
    << m_clip_z[0] << " "
    << m_clip_z[1] << " "
    << m_clip_z[2] << " "
    << m_clip_z[3] << " "
    <<std::endl;
}

//update the AABB
void Viewer::update_aabb()
{
  if (m_cm != NULL){
    Point_3 low = m_cm->bbox_bottom_left();
    Point_3 high = m_cm->bbox_top_right();

    for (int i = 0; i < 3; ++i){
      aabb_low[i] = (double) low[i];
      aabb_high[i] = (double) high[i];
    }

    m_clip_x[3] = low[0];
    m_clip_y[3] = low[1];
    m_clip_z[3] = low[2];
  }
}


void Viewer::enable_clip_plane(bool flag)
{
  if (flag) {
    glEnable(GL_CLIP_PLANE0);
    glEnable(GL_CLIP_PLANE1);
    glEnable(GL_CLIP_PLANE2);
  } else {
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
  }
};

void Viewer::show_clip_plane(bool flag)
{
  m_clip_show = flag;
}

void Viewer::draw_corner_axis()
{
  int viewport[4];
  int scissor[4];

  // The viewport and the scissor are changed to fit the lower left
  // corner. Original values are saved.
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetIntegerv(GL_SCISSOR_BOX, scissor);

  // Axis viewport size, in pixels
  const int size = 150;
  glViewport(0,0,size,size);
  glScissor(0,0,size,size);

  // The Z-buffer is cleared to make the axis appear over the
  // original image.
  glClear(GL_DEPTH_BUFFER_BIT);

  // Tune for best line rendering
  glDisable(GL_LIGHTING);
  glLineWidth(1.0);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMultMatrixd(camera()->orientation().inverse().matrix());


  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0);

  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 1.0);
  glEnd();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glEnable(GL_LIGHTING);

  // The viewport and the scissor are restored.
  glScissor(scissor[0],scissor[1],scissor[2],scissor[3]);
  glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

};

//void Viewer::postDraw()
//{
//  QGLViewer::postDraw();
//  //draw_corner_axis();
//};

void Viewer::show_axis(bool flag)
{
  m_axis = flag;
};

//set rotatation scale (according to X axis)
void Viewer::set_rot_x(int value)
{
  m_radius_x = (double) value * 2.0* M_PI / 360.0;
}

//set rotatation scale (according to Y axis)
void Viewer::set_rot_y(int value)
{
  m_radius_y = (double) value * 2.0* M_PI / 360.0;
}

//set rotatation scale (according to Z axis)
void Viewer::set_rot_z(int value)
{
  m_radius_z = (double) value * 2.0* M_PI / 360.0;
}

void Viewer::rotate_according_x()
{
  manipulatedFrame()->rotate(qglviewer::Quaternion(qglviewer::Vec(1.0,0.0,0.0),m_radius_x));
  updateGL();
};
void Viewer::rotate_according_y()
{
  manipulatedFrame()->rotate(qglviewer::Quaternion(qglviewer::Vec(0.0,1.0,0.0),m_radius_y));
  updateGL();
};
void Viewer::rotate_according_z()
{
  manipulatedFrame()->rotate(qglviewer::Quaternion(qglviewer::Vec(0.0,0.0,1.0),m_radius_z));
  updateGL();
};


// Customized mouse events 
void Viewer::mousePressEvent(QMouseEvent* e)
{
  // Start selection. Mode is ADD with Shift key and TOGGLE with Alt key.
  rectangle_ = QRect(e->pos(), e->pos());

  if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::ShiftModifier))
    selectionMode_ = ADD;
  else
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
      selectionMode_ = REMOVE;
    else
    {
      /*if (e->modifiers() == Qt::ControlModifier)      
        startManipulation();*/
      QGLViewer::mousePressEvent(e);
    }
}

void Viewer::mouseMoveEvent(QMouseEvent* e)
{
  if (selectionMode_ != NONE)
  {
    // Updates rectangle_ coordinates and redraws rectangle
    rectangle_.setBottomRight(e->pos());
    updateGL();
  }
  else
    QGLViewer::mouseMoveEvent(e);
}

void Viewer::mouseReleaseEvent(QMouseEvent* e)
{
  if (selectionMode_ != NONE)
  {
    // Actual selection on the rectangular area.
    // Possibly swap left/right and top/bottom to make rectangle_ valid.
    rectangle_ = rectangle_.normalized();
    // Define selection window dimensions
    setSelectRegionWidth(rectangle_.width());
    setSelectRegionHeight(rectangle_.height());
    // Compute rectangle center and perform selection
    select(rectangle_.center());
    // Update display to show new selected objects
    updateGL();
  }
  else
    QGLViewer::mouseReleaseEvent(e);
}


// processing
void Viewer::drawWithNames()
{
  for (int i=0; i<int(objects_.size()); i++)
  {
    glPushName(i);
    objects_.at(i)->draw();
    glPopName();
  }
}

void Viewer::endSelection(const QPoint&)
{
  // Flush GL buffers
  glFlush();

  // Get the number of objects that were seen through the pick matrix frustum. Reset GL_RENDER mode.
  GLint nbHits = glRenderMode(GL_RENDER);

  if (nbHits > 0)
  {
    // Interpret results : each object created 4 values in the selectBuffer().
    // (selectBuffer())[4*i+3] is the id pushed on the stack.
    for (int i=0; i<nbHits; ++i)
      switch (selectionMode_)
      {
        case ADD    : addIdToSelection((selectBuffer())[4*i+3]); break;
        case REMOVE : removeIdFromSelection((selectBuffer())[4*i+3]);  break;
        default : break;
      }
  }
  selectionMode_ = NONE;
}

void Viewer::startManipulation()
{
  /*Vec averagePosition;
  ManipulatedFrameSetConstraint* mfsc = (ManipulatedFrameSetConstraint*)(manipulatedFrame()->constraint());
  mfsc->clearSet();

  for (QList<int>::const_iterator it=selection_.begin(), end=selection_.end(); it != end; ++it)
  {
    mfsc->addObjectToSet(objects_[*it]);
    averagePosition += objects_[*it]->frame.position();
  }

  if (selection_.size() > 0)
    manipulatedFrame()->setPosition(averagePosition / selection_.size());*/
}


//tools 

void Viewer::addIdToSelection(int id)
{
  if (!selection_.contains(id))
    selection_.push_back(id);
}

void Viewer::removeIdFromSelection(int id)
{
  selection_.removeAll(id);
}

void Viewer::drawSelectionRectangle() const
{
  startScreenCoordinatesSystem();
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);

  glColor4f(0.0, 0.0, 0.3f, 0.3f);
  glBegin(GL_QUADS);
  glVertex2i(rectangle_.left(),  rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(),  rectangle_.bottom());
  glEnd();

  glLineWidth(2.0);
  glColor4f(0.4f, 0.4f, 0.5f, 0.5f);
  glBegin(GL_LINE_LOOP);
  glVertex2i(rectangle_.left(),  rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(),  rectangle_.bottom());
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  stopScreenCoordinatesSystem();
}

void Viewer::build_select_objects()
{
  std::cout << "starting build selected object buffer.... " << std::endl;
  if (m_cm != NULL) {
    objects_.reserve(m_cm->size_of_nuerons()); //reserve spave
    for (CellModel::Nueron_iterator ni = m_cm->nuerons_begin();
        ni != m_cm->nuerons_end(); ++ni){
      Nueron* n_ptr = *ni;
      objects_.insert(n_ptr->id(),n_ptr);
    }
  }
  std::cout << "finish the building of selected buffer" << std::endl;
}

void Viewer::show_selected(bool flag)
{
  m_selected = flag; 
}


