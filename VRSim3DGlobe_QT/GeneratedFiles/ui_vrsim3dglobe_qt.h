/********************************************************************************
** Form generated from reading UI file 'vrsim3dglobe_qt.ui'
**
** Created: Sun Aug 10 22:16:41 2014
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VRSIM3DGLOBE_QT_H
#define UI_VRSIM3DGLOBE_QT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VRSim3DGlobe_QTClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VRSim3DGlobe_QTClass)
    {
        if (VRSim3DGlobe_QTClass->objectName().isEmpty())
            VRSim3DGlobe_QTClass->setObjectName(QString::fromUtf8("VRSim3DGlobe_QTClass"));
        VRSim3DGlobe_QTClass->resize(600, 400);
        menuBar = new QMenuBar(VRSim3DGlobe_QTClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        VRSim3DGlobe_QTClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VRSim3DGlobe_QTClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        VRSim3DGlobe_QTClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(VRSim3DGlobe_QTClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        VRSim3DGlobe_QTClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(VRSim3DGlobe_QTClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        VRSim3DGlobe_QTClass->setStatusBar(statusBar);

        retranslateUi(VRSim3DGlobe_QTClass);

        QMetaObject::connectSlotsByName(VRSim3DGlobe_QTClass);
    } // setupUi

    void retranslateUi(QMainWindow *VRSim3DGlobe_QTClass)
    {
        VRSim3DGlobe_QTClass->setWindowTitle(QApplication::translate("VRSim3DGlobe_QTClass", "VRSim3DGlobe_QT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VRSim3DGlobe_QTClass: public Ui_VRSim3DGlobe_QTClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VRSIM3DGLOBE_QT_H
