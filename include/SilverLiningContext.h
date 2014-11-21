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
#include "Export.h"
#include <osg/Referenced>
#include <osg/Camera>
#include "osg/Light"
#include <osgEarth/ThreadingUtils>

namespace SilverLining {
    class Atmosphere;
    class CloudLayer;
}
namespace osgEarth {
    class SpatialReference;
}

namespace VRSim3D {  namespace SilverLining
{
    using namespace osgEarth;

    /**
     * Contains all the SilverLining SDK pointers.
     */
    class VRSim3D_EXPORT SilverLiningContext : public osg::Referenced
    {
    public:
        SilverLiningContext();
        

        ///zqgao 设置地球坐标参考系
        void setSRS(const SpatialReference* srs);

    public: // accessors

        bool ready() const { return m_initAttempted && !m_initFailed; }

        ::SilverLining::Atmosphere* getAtmosphere() { return m_atmosphere; }

        /** Spatial reference of the map */
        const SpatialReference* getSRS() const { return m_srs.get(); }

        void initialize(osg::RenderInfo& renderInfo);
        void updateLocation();

		///zqgao 设置资源路径
		void setResourcePath(const std::string &path);
		std::string getResourePath()const{return m_path;}

        /** Set/get the cached camers. NOT THREAD/MULTI-CAM SAFE. */
        /** TODO */
        void setCamera(osg::Camera* camera) { m_camera = camera; }
        osg::Camera* getCamera() { return m_camera.get(); }

        void setCameraPosition(const osg::Vec3d& pos) { m_cameraPos = pos; }
        const osg::Vec3d& getCameraPosition() const { return m_cameraPos; }

		///zqgao 云层最大高度
		void setcloudsMaxAltitude(const double& altitude);
		double cloudsMaxAltitude()const {return m_cloudsMaxAltitude;}

		void setSkyBoxSize(double size) { _skyBoxSize = size; }
		double getSkyBoxSize() const { return _skyBoxSize; }

    protected:

        virtual ~SilverLiningContext();

    private:

        void setupClouds();

    private:
        ::SilverLining::Atmosphere* m_atmosphere;
        ::SilverLining::CloudLayer* m_clouds;

        osg::ref_ptr<const SpatialReference> m_srs;

        bool             m_initAttempted;
        bool             m_initFailed;
        Threading::Mutex m_initMutex;


        osg::observer_ptr<osg::Camera> m_camera;
        osg::Vec3d                     m_cameraPos; // eye point

		///zqgao 资源路径、
		std::string m_path;
		///zqgao SilverLining  username and license key
		std::string m_user;
		std::string m_licenseCode;
		///zqgao 云层最大高度
		double     m_cloudsMaxAltitude;
		double _skyBoxSize;
    };

} } // namespace VRSim3D::SilverLining 
