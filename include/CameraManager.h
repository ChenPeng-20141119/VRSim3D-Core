//////////////////////////////////////////////////////////////////////////
///�ļ���������ٿ���
///�����ߣ���־��
///����ʱ�䣺2013��7��17��
///����������С��������ٿ���
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
	//ͨ�����������ӿ�
	virtual void setByMatrix(const osg::Matrixd& matrix);
	//���ص�ǰ����
	virtual osg::Matrixd getMatrix() const;
	//��������� set the position of the matrix manipulator using a 4x4 Matrix.
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	//���������
	virtual osg::Matrixd getInverseMatrix() const;
	//Handle events
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
	//λ�ñ任
	bool changePosition(osg::Vec3 delta);
	//���ò���
	void setSetUp(float data_setup);
	// ���������λ�ø���
	void computeChange(float &x,float &y,float &z,ChangeBy changeby = CHANGEBY_KEY);
	//������ײģ��
	void setModle(osg::Node *node);
	//��ײ���
	bool Ishold(osg::Vec3 detla);
private:
	//�ƶ�
	void move();
private:
	osg::Vec3 m_vPosition;//��ǰλ��
	osg::Vec3 m_vRotation;//��ǰ����
	float m_vStep;//�����ƶ�����
	float m_vScollStep;//�������ƶ�����
	float m_vRotateStep;//��ת�Ƕ�
	bool m_bRightDown;
	float m_iLeftX;
	float m_iLeftY;
	osg::ref_ptr<osg::Node> m_node;
	//�Ƿ��ƶ�
	bool m_isMove;
	Key_Symbol m_KeySymbol;
};
}
#endif

