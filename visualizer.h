#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include "QGLViewer/qglviewer.h"
#include "CellModel.h"

class Viewer : public QGLViewer
{
  Q_OBJECT

public:
    void set_cellModel(CellModel* cm);

  protected:
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;

private: 
	CellModel* m_cm;
};
  

#endif 
