#ifndef NEURON_H
#define NEURON_H

#include <QtGui/QMainWindow>
#include "ui_neuron.h"

class NEURON : public QMainWindow
{
	Q_OBJECT

public:
	NEURON(QWidget *parent = 0, Qt::WFlags flags = 0);
	~NEURON();

private:
	Ui::NEURONClass ui;
};

#endif // NEURON_H
