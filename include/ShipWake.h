//////////////////////////////////////////////////////////////////////////
///�ļ���������ʻˮ��β����
///�����ߣ���־��
///����ʱ�䣺2013��7��31��
///����������ʵ�ִ���ˮ����ʻ����β���ࣨ������Triton::WakeGenerator��
//////////////////////////////////////////////////////////////////////////
#ifndef SHIP_WAKE_H
#define SHIP_WAKE_H 1

#include "Export.h"

#include<Triton.h>
#include"osg/NodeCallback"
#include "osg/ComputeBoundsVisitor"

namespace osgEarth { namespace Drivers { namespace Triton
{
	class TritonContext;
}}}
namespace VRSim3D
{
class VRSim3D_EXPORT ShipWake : public virtual osg::NodeCallback
{
	
public:
    ShipWake(osgEarth::Drivers::Triton::TritonContext* triton = NULL);

   virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

    void UpdateShipPos( double x, double y, double z, double dx, double dy, double dz, double velocity );

protected:
	void update(osg::Node* node,osg::NodeVisitor* nv);
    void Setup( void );
    void Cleanup( void );

    virtual ~ShipWake();

	osgEarth::Drivers::Triton::TritonContext*  _TRITON;
    Triton::WakeGenerator  *_ship;
    bool                    _geocentric;
    osg::Vec3d              _shipPosition;
    osg::Vec3d              _shipDirection;
    float                   _shipVelocity;
	osg::BoundingBox		m_bb;

};
}
#endif
