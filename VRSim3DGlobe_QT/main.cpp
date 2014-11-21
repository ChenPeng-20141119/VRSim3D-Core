#include "vrsim3dglobe_qt.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VRSim3DGlobe_QT w;
	w.show();
	return a.exec();
}
