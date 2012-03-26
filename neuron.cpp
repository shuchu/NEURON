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
	ui.plainTextEdit->setReadOnly(true); //put the text editor into Read-only

	//redirect the std::clog buffer
	logBuf.open("./log.txt");
	std::cout.rdbuf(logBuf.rdbuf());

	//create model
	cellModel = new CellModel();

	//setup the connections
	connect(ui.actionLoad,SIGNAL(triggered()),this,SLOT(open()));
}

NEURON::~NEURON()
{
	logBuf.close();
}

void NEURON::open()
{
	QString file_name = QFileDialog::getOpenFileName(this,tr("Open File"));

	if (!file_name.isNull())
	{
		cellModel->load_data_from_file(file_name.toStdString());
	}
};


