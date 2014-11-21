/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
 * Copyright 2008-2013 Pelican Mapping
 * http://osgearth.org
 *
 * osgEarth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
#include "SilverLiningCloudsDrawable.h"
#include "SilverLiningContext.h"
#include <osgEarth/SpatialReference>
#include <SilverLining.h>


using namespace VRSim3D::SilverLining;

CloudsDrawable::CloudsDrawable(SilverLiningContext* SL) :
_SL( SL )
{
    // call this to ensure draw() gets called every frame.
    setSupportsDisplayList( false );
    
    // not MT-safe (camera updates, etc)
    this->setDataVariance(osg::Object::DYNAMIC);    
}

void
CloudsDrawable::drawImplementation(osg::RenderInfo& renderInfo) const
{
	//osg::Camera* camera = renderInfo.getCurrentCamera();
	//if ( camera )
	//{
	//	renderInfo.getState()->disableAllVertexArrays();
	//	_SL->initialize( renderInfo );

	//	double fovy, ar, znear, zfar;
	//	_SL->setCamera(camera);

	//	//renderInfo.getCurrentCamera()->setNearFarRatio(.00000001);

	//	camera->getProjectionMatrixAsPerspective(fovy, ar, znear, zfar);
	//}
    renderInfo.getState()->disableAllVertexArrays();
    _SL->getAtmosphere()->DrawObjects( true, false, true,1.0 );
    renderInfo.getState()->dirtyAllVertexArrays();
}

osg::BoundingBox
CloudsDrawable::computeBound() const
{
    osg::BoundingBox cloudBoundBox;
    if ( !_SL->ready() )
        return cloudBoundBox;
    
    double minX, minY, minZ, maxX, maxY, maxZ;
    _SL->getAtmosphere()->GetCloudBounds( minX, minY, minZ, maxX, maxY, maxZ );
    cloudBoundBox.set( osg::Vec3d(minX, minY, minZ), osg::Vec3d(maxX, maxY, maxZ) );
    return cloudBoundBox;
}
