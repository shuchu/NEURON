#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <fstream>

#include <QtGui/QMainWindow>
#include "ui_neuron.h"
#include "CellModel.h"

class NEURON : public QMainWindow
{
	Q_OBJECT

public:
	NEURON(QWidget *parent = 0, Qt::WFlags flags = 0);
	~NEURON();

	void show_log();


private slots:
	void open();
	//void about();

private:
	Ui::NEURONClass ui;
	CellModel* cellModel;
	std::ofstream logBuf; //buffer for log
};

#endif // NEURON_H
