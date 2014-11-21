//////////////////////////////////////////////////////////////////////////
///�ļ���������ϴ����
///�����ߣ���־��
///����ʱ�䣺2014��8��2��
///��������������ϴ����ˮ���Ӱ��ģ�⣨������Triton::RotorWashHandler��
//////////////////////////////////////////////////////////////////////////
#ifndef  ROTOR_WASH_HANDLER_H
#define ROTOR_WASH_HANDLER_H 1

#include "Export.h"
#include "MyRotorWash.h"
#include<Triton.h>
#include"osg/NodeCallback"
#include "osg/ComputeBoundsVisitor"

namespace osgEarth { namespace Drivers { namespace Triton
{
	class TritonContext;
}}}

namespace VRSim3D
{
class VRSim3D_EXPORT RotorWashHandler : public virtual osg::NodeCallback
{
public:
    RotorWashHandler(osgEarth::Drivers::Triton::TritonContext* triton = NULL);

   virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

    void UpdateShipPos( double x, double y, double z, double dx, double dy, double dz, double velocity );

	///zqgao ���÷���
	void setwindVelocity(const double &velocity);

protected:
	void update(osg::Node* node,osg::NodeVisitor* nv);
    void Setup( void );
    void Cleanup( void );
	///zqgao �������ģ��ɻ���������ֱ����淽��
	osg::Vec3d computeWindDir();

    virtual ~RotorWashHandler();

	osgEarth::Drivers::Triton::TritonContext*  m_TRITON;
    VRSim3D::MyRotorWash			*m_wash;
    bool								m_geocentric;
    osg::Vec3d					m_rotorPosition;
    osg::Vec3d					m_windDirection;
    float								m_windVelocity;
	osg::BoundingBox			m_bb;

};
}
#endif
