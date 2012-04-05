#ifndef __VISUALIZER_H__
#define __VISUALIZER_H__

#include "QGLViewer/qglviewer.h"
#include "CellModel.h"

class Viewer : public QGLViewer
{
  Q_OBJECT

public:
	Viewer(QWidget* parent)	: QGLViewer(parent){};

    void load_cellModel(CellModel* cm);

protected:
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;
	void draw_box(int x, int y, int z, float scale);
	void draw_box(int lx, int ly, int lz, int rx, int ry, int rz);

public slots:
	void aabbBoxState(int state);

private: 
	CellModel * m_cm;

	//tags
	int m_aabb;
};
  

#endif 
