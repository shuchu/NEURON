#include <QtGui/QApplication>
#include "neuron.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NEURON w;
	w.show();
	return a.exec();
}
