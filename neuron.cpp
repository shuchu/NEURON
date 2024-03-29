#include <iostream>
#include <fstream>

#include <QString>
#include <QFileDialog>


#include "neuron.h"


NEURON::NEURON(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//setup ui
	ui.setupUi(this);
	ui.textEdit->setReadOnly(true);
	qout = new QDebugStream(std::cout,ui.textEdit);

	//create model
	cellModel = new CellModel();
	ui.viewer->load_cellModel(cellModel);

	//ui
        connect(ui.actionQuit,SIGNAL(triggered()),this,SLOT(quit()));
        //connect(ui.pushButton_snapshot,SIGNAL(clicked()),ui.viewer,SLOT(saveSnapshot()));
        connect(ui.actionSave_Status,SIGNAL(triggered()),ui.viewer,SLOT(saveStateToFile()));
        connect(ui.actionLoad_status,SIGNAL(triggered()),ui.viewer,SLOT(load_status()));

        //setup the connections
        connect(ui.actionLoad,SIGNAL(triggered()),this,SLOT(open())); //file load
        connect(ui.checkBox_AABB,SIGNAL(stateChanged(int)),ui.viewer,SLOT(aabbBoxState(int))); //aabb 
        connect(ui.checkBox_P, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_P(int))); // show P
        connect(ui.checkBox_N, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_N(int))); // show N
        connect(ui.checkBox_G, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_G(int))); // show G
        connect(ui.checkBox_B, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_B(int))); // show B
        connect(ui.checkBox_A, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_A(int))); // show A
        connect(ui.checkBox_S, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_S(int))); // show S
        connect(ui.checkBox_T, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_T(int))); // show T
        connect(ui.checkBox_I, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_I(int))); // show I
        connect(ui.checkBox_C, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_C(int))); // show C
        connect(ui.checkBox_M, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_M(int))); // show M
        connect(ui.checkBox_R, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_R(int))); // show R
        connect(ui.checkBox_D, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_D(int))); // show D
        connect(ui.checkBox_DEN, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_den(int))); //dendrites
        connect(ui.checkBox_AXON, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_axon(int))); //axons
        connect(ui.checkBox_axis, SIGNAL(clicked(bool)),ui.viewer,SLOT(show_axis(int))); //axis
        connect(ui.checkBox_VIA, SIGNAL(clicked(bool)),ui.viewer,SLOT(show_syn_via(bool))); //via point
        connect(ui.checkBox_IN, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_syn_in(int))); //synapse IN
        connect(ui.checkBox_OUT, SIGNAL(stateChanged(int)),ui.viewer,SLOT(show_syn_out(int))); //synapse OUT
        connect(ui.checkBox_ShowLetters,SIGNAL(clicked(bool)),ui.viewer,SLOT(setTextIsEnabled(bool))); //text
        connect(ui.checkBox_ortho,SIGNAL(clicked(bool)),ui.viewer,SLOT(show_ortho(bool))); //orthogonal viewerx
        connect(ui.groupBox_3, SIGNAL(toggled(bool)),ui.viewer,SLOT(enable_clip_plane(bool))); //enable clipping planes
        connect(ui.verticalSlider_x, SIGNAL(sliderMoved(int)), ui.viewer, SLOT(set_clip_x(int))); // clip X
        connect(ui.verticalSlider_y, SIGNAL(sliderMoved(int)), ui.viewer, SLOT(set_clip_y(int))); // clip X
        connect(ui.verticalSlider_z, SIGNAL(sliderMoved(int)), ui.viewer, SLOT(set_clip_z(int))); // clip X
        connect(ui.spinBox,SIGNAL(valueChanged(int)),ui.viewer,SLOT(set_clip_x(int))); //clip
        connect(ui.spinBox_2,SIGNAL(valueChanged(int)),ui.viewer,SLOT(set_clip_y(int)));
        connect(ui.spinBox_3,SIGNAL(valueChanged(int)),ui.viewer,SLOT(set_clip_z(int)));
        connect(ui.spinBox_4,SIGNAL(valueChanged(int)),ui.viewer,SLOT(set_rot_x(int))); //rotate scale
        connect(ui.spinBox_5,SIGNAL(valueChanged(int)),ui.viewer,SLOT(set_rot_y(int)));
        connect(ui.spinBox_6,SIGNAL(valueChanged(int)),ui.viewer,SLOT(set_rot_z(int)));
        connect(ui.pushButton_rot_x,SIGNAL(clicked()),ui.viewer,SLOT(rotate_according_x())); //rotate action
        connect(ui.pushButton_rot_y,SIGNAL(clicked()),ui.viewer,SLOT(rotate_according_y()));
        connect(ui.pushButton_rot_z,SIGNAL(clicked()),ui.viewer,SLOT(rotate_according_z()));
        connect(ui.checkBox_select,SIGNAL(clicked(bool)),ui.viewer, SLOT(show_selected(bool))); //selection option 
}

NEURON::~NEURON()
{
  //logBuf.close();
  delete qout;
  delete cellModel;
}

void NEURON::open()
{
  QString file_name = QFileDialog::getOpenFileName(this,tr("Open File"));

  if (!file_name.isNull())
  {
    cellModel->load_data_from_file(file_name.toStdString());
    cellModel->compute_bounding_box(); //computer the bounding box
    ui.viewer->update_scene(); //update the scene of viewer as new data coming.
    //setTextIsEnabled(false);

  }
};



