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

#include "SilverLiningNode.h"
#include "SilverLiningContext.h"
#include "SilverLiningCloudsDrawable.h"
#include "SilverLiningSkyDrawable.h"

#include <osg/Light>
#include <osg/LightSource>
#include <osgEarth/CullingUtils>
#include <osgEarth/PhongLightingEffect>
#include <SilverLining.h>

#define LC "[SilverLiningNode] "

using namespace VRSim3D::SilverLining;

SilverLiningNode::SilverLiningNode(const osgEarth::Map* map,osgEarth::Util::SkyNode* skyNode,const std::string &source ):
m_lastAltitude(DBL_MAX)
{

	m_skyNode = skyNode;
    // The main silver lining data:
    m_SL = new SilverLiningContext();  
    m_SL->setSRS( map->getSRS() );
	m_SL->setResourcePath(source);
	m_SL->setcloudsMaxAltitude(10000);

    // Geode to hold each of the SL drawables:
    m_geode = new osg::Geode();
    m_geode->setCullingActive( false );
    this->addChild( m_geode );

	_skyDrawable = new SkyDrawable( m_SL.get() );
	m_geode->addDrawable( _skyDrawable );

    // Clouds
    m_cloudsDrawable = new CloudsDrawable( m_SL.get() );
    m_geode->addDrawable( m_cloudsDrawable.get() );

	this->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    // SL requires an update pass.
    ADJUST_UPDATE_TRAV_COUNT(this, +1);
}


SilverLiningNode::~SilverLiningNode()
{
   
}

void
SilverLiningNode::traverse(osg::NodeVisitor& nv)
{
    if ( m_SL->ready() )
    {
        if ( nv.getVisitorType() == nv.UPDATE_VISITOR )
        {
			int frameNumber = nv.getFrameStamp()->getFrameNumber();
            m_SL->updateLocation();
			 m_SL->getAtmosphere()->UpdateSkyAndClouds();
			_skyDrawable->dirtyBound();
			onSetDateTime();
            if (m_lastAltitude <= m_SL->cloudsMaxAltitude() )
            {
                if ( m_cloudsDrawable->getNumParents() == 0 )
                    m_geode->addDrawable( m_cloudsDrawable.get() );
                
                m_cloudsDrawable->dirtyBound();
            }
            else
            {
                if ( m_cloudsDrawable->getNumParents() > 0 )
                    m_geode->removeDrawable( m_cloudsDrawable.get() );
			}
        }
        else if ( nv.getVisitorType() == nv.CULL_VISITOR )
        {
            // TODO: make this multi-camera safe
            m_SL->setCameraPosition( nv.getEyePoint() );
            osgUtil::CullVisitor* cv = Culling::asCullVisitor(nv);
            m_SL->getAtmosphere()->SetCameraMatrix( cv->getModelViewMatrix()->ptr() );
            m_SL->getAtmosphere()->SetProjectionMatrix( cv->getProjectionMatrix()->ptr() );

			m_lastAltitude = m_SL->getSRS()->isGeographic() ?
				cv->getEyePoint().length() - m_SL->getSRS()->getEllipsoid()->getRadiusEquator() :
				cv->getEyePoint().z();

			if (m_lastAltitude <= m_SL->cloudsMaxAltitude() )
			{
				m_SL->getAtmosphere()->CullObjects();
			}
        }
    }
    Group::traverse( nv );
}

void VRSim3D::SilverLining::SilverLiningNode::onSetDateTime()
{
	::SilverLining::LocalTime utcTime;
	utcTime.SetTimeZone(GMT);
	utcTime.SetObservingDaylightSavingsTime(false);
	/*utcTime.SetFromSystemTime();*/
	utcTime.SetYear(m_skyNode->getDateTime().year());
	utcTime.SetMonth(m_skyNode->getDateTime().month());
	utcTime.SetDay(m_skyNode->getDateTime().day());
	int h, m, s;
	double hs = m_skyNode->getDateTime().hours();
	h = (int)hs;
	hs = (hs - h) * 60;
	m = int(hs);  
	hs = (hs - m) * 60;
	s = int(hs + 0.49);
	utcTime.SetHour(h);
	utcTime.SetMinutes(m);
	utcTime.SetSeconds(s);
	//utcTime.SetMinutes(m_skyNode->getDateTime().);
	//utcTime.SetFromEpochSeconds( m_skyNode->getDateTime().asTimeStamp() );
	m_SL->getAtmosphere()->GetConditions()->SetTime( utcTime );
}
