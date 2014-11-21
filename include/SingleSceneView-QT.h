#ifdef _QTWIND
#ifndef SINGLE_SCENE_VIEW__QT_H
#define SINGLE_SCENE_VIEW__QT_H 1

#include "Export.h"
#include "SingleSceneView.h"

#include "osgGA/KeySwitchMatrixManipulator"

namespace VRSim3D
{

class VRSim3D_EXPORT SingleSceneViewQT :
	public QGraphicsScene,
	public VRSim3D::SingleSceneView
{
public:
	SingleSceneViewQT(int width = 0, int height = 0);
	~SingleSceneViewQT(void);
	///zqgao 创建场景，返回true创建成功
	///\param samples(unsigned int)
	// 抗锯齿倍数，请根据使用机显卡设置
	virtual	bool CreateScene(HWND hwnd,const float &width = 800.0f,const float &height = 600.0f,const unsigned int &samples = 16);
private:
	QTimer _timer;
private:
	virtual void keyPressEvent( QKeyEvent* event );
	virtual void keyReleaseEvent( QKeyEvent* event );
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent * event);
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

	virtual void drawBackground(QPainter *painter, const QRectF &rect);
public:
	void resize(int width, int height);
};
}
#endif
#endif
