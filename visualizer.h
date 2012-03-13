#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include "QGLViewer/qglviewer.h"

class Viewer : public QGLViewer
{
  protected:
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;
};
  

#endif 
