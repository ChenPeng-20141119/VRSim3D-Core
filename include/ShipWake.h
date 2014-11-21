//////////////////////////////////////////////////////////////////////////
///文件名：船行驶水面尾迹类
///创建者：高志清
///创建时间：2013年7月31日
///内容描述：实现船在水面行驶产生尾迹类（整合自Triton::WakeGenerator）
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
