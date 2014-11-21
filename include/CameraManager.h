//////////////////////////////////////////////////////////////////////////
///文件名：相机操控器
///创建者：高志清
///创建时间：2013年7月17日
///内容描述：小场景相机操控器
//////////////////////////////////////////////////////////////////////////
#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H 1

#include "Export.h"
#include "osgGA/CameraManipulator"
#include "osgViewer/Viewer"

namespace VRSim3D
{

class VRSim3D_EXPORT CameraManager :
	public osgGA::CameraManipulator
{
public:
	CameraManager(void);
	~CameraManager(void);
	struct Key_Symbol
	{
		bool KEY_FRONT;
		bool KEY_BACK;
		bool KEY_LEFT;
		bool KEY_RIGHT;
	};
	enum ChangeBy
	{
		CHANGEBY_KEY,
		CHANGEBY_MOUSE
	};
	//通过矩阵设置视口
	virtual void setByMatrix(const osg::Matrixd& matrix);
	//返回当前矩阵
	virtual osg::Matrixd getMatrix() const;
	//设置逆矩阵 set the position of the matrix manipulator using a 4x4 Matrix.
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	//返回逆矩阵
	virtual osg::Matrixd getInverseMatrix() const;
	//Handle events
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
	//位置变换
	bool changePosition(osg::Vec3 delta);
	//设置步长
	void setSetUp(float data_setup);
	// 计算各方向位置更改
	void computeChange(float &x,float &y,float &z,ChangeBy changeby = CHANGEBY_KEY);
	//设置碰撞模型
	void setModle(osg::Node *node);
	//碰撞检测
	bool Ishold(osg::Vec3 detla);
private:
	//移动
	void move();
private:
	osg::Vec3 m_vPosition;//当前位置
	osg::Vec3 m_vRotation;//当前朝向
	float m_vStep;//按键移动步长
	float m_vScollStep;//鼠标滚轴移动步长
	float m_vRotateStep;//旋转角度
	bool m_bRightDown;
	float m_iLeftX;
	float m_iLeftY;
	osg::ref_ptr<osg::Node> m_node;
	//是否移动
	bool m_isMove;
	Key_Symbol m_KeySymbol;
};
}
#endif

