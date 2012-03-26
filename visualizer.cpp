#include "visualizer.h"
//initialization
void Viewer::init()
{


}


//main draw functions
void Viewer::draw()
{
	m_cm->draw_nuerons();	
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
