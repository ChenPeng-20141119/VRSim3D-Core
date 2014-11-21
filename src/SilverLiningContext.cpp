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
#include "SilverLiningContext.h"
#include <SilverLining.h> // SilverLinking SDK

#include <osgEarth/SpatialReference>

#define LC "[SilverLiningContext] "

using namespace osgEarth;
using namespace VRSim3D::SilverLining;


SilverLiningContext::SilverLiningContext():
m_initAttempted( false ),
m_initFailed( false ),
m_atmosphere( 0L ),
m_clouds( 0L )
{
    // Create a SL atmosphere (the main SL object).
    // TODO: plug in the username + license key.
	m_user = "test";
	m_licenseCode = "test";
    m_atmosphere = new ::SilverLining::Atmosphere(
        m_user.c_str(),
        m_licenseCode.c_str() );
}

void
SilverLiningContext::setSRS(const SpatialReference* srs)
{
    m_srs = srs;
}

void
SilverLiningContext::initialize(osg::RenderInfo& renderInfo)
{
    if ( !m_initAttempted && !m_initFailed )
    {
        // lock/double-check:
        Threading::ScopedMutexLock excl(m_initMutex);
        if ( !m_initAttempted && !m_initFailed )
        {
            m_initAttempted = true;

            // constant random seed ensures consistent clouds across windows
            // TODO: replace this with something else since this is global! -gw
            ::srand(1234);

            int result = m_atmosphere->Initialize(
                ::SilverLining::Atmosphere::OPENGL,
                m_path.c_str(),
                true,
                0 );
            if ( result != ::SilverLining::Atmosphere::E_NOERROR )
            {
                m_initFailed = true;
                OE_WARN << LC << "SilverLining failed to initialize: " << result << std::endl;
            }
            else
            {
                OE_INFO << LC << "SilverLining initialized OK!" << std::endl;

                // Defaults for a projected terrain. ECEF terrain vectors are set
                // in updateLocation().
				::SilverLining::LocalTime t;
				t.SetFromSystemTime();
				t.SetHour( 12.0 );
				t.SetTimeZone( GMT );
				m_atmosphere->GetConditions()->SetTime( t );
                m_atmosphere->SetUpVector( 0.0, 0.0, 1.0 );
                m_atmosphere->SetRightVector( 1.0, 0.0, 0.0 );

#if 0 // todo: review this
                _maxAmbientLightingAlt = 
                    m_atmosphere->GetConfigOptionDouble("atmosphere-height");
#endif
                    setupClouds();
            }
        }
    }
}

void
SilverLiningContext::setupClouds()
{
    m_clouds = ::SilverLining::CloudLayerFactory::Create( CUMULUS_CONGESTUS );
    m_clouds->SetIsInfinite( true );
    m_clouds->SetFadeTowardEdges(true);
    m_clouds->SetBaseAltitude( 2000 );
    m_clouds->SetThickness( 200 );
    m_clouds->SetBaseLength( 100000 );
    m_clouds->SetBaseWidth( 100000 );
	//m_clouds->SetWind(-5,0);
    m_clouds->SetDensity( 0.6 );
    m_clouds->SetAlpha( 0.8 );

    m_clouds->SeedClouds( *m_atmosphere );
    m_clouds->GenerateShadowMaps( false );
    
    m_clouds->SetLayerPosition(0, 0);

    m_atmosphere->GetConditions()->AddCloudLayer( m_clouds );

	::SilverLining::WindVolume windVolume;
	windVolume.SetWindSpeed(100);
	m_atmosphere->GetConditions()->SetWind(windVolume);
}

void
SilverLiningContext::updateLocation()
{
    if ( !ready() || !m_srs.valid() )
        return;

    if ( m_srs->isGeographic() )
    {
        // Get new local orientation
        osg::Vec3d up = m_cameraPos;
        up.normalize();
        osg::Vec3d north = osg::Vec3d(0, 1, 0);
        osg::Vec3d east = north ^ up;

        // Check for edge case of north or south pole
        if (east.length2() == 0)
        {
            east = osg::Vec3d(1, 0, 0);
        }

        east.normalize();

        m_atmosphere->SetUpVector(up.x(), up.y(), up.z());
        m_atmosphere->SetRightVector(east.x(), east.y(), east.z());

        // Get new lat / lon / altitude
        osg::Vec3d latLonAlt;
        m_srs->transformFromWorld(m_cameraPos, latLonAlt);

        ::SilverLining::Location loc;
        loc.SetAltitude ( latLonAlt.z() );
        loc.SetLongitude( osg::DegreesToRadians(latLonAlt.x()) );
        loc.SetLatitude ( osg::DegreesToRadians(latLonAlt.y()) );

        m_atmosphere->GetConditions()->SetLocation( loc );

        if ( m_clouds )
        {
#if 1 //TODO: figure out why we need to call this a couple times before
      //      it takes effect. -gw
            static int c = 2;
            if ( c > 0 ) {
                --c;
                m_clouds->SetLayerPosition(0, 0);
            }
        }
#endif
    }
}

SilverLiningContext::~SilverLiningContext()
{
    // clean up all the SL handles.
    delete m_atmosphere;
}

void VRSim3D::SilverLining::SilverLiningContext::setResourcePath( const std::string &path )
{
	m_path = path;
}

void VRSim3D::SilverLining::SilverLiningContext::setcloudsMaxAltitude( const double& altitude )
{
	m_cloudsMaxAltitude = altitude;   
}
