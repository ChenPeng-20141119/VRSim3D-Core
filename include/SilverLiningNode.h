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

#include "osg/Group"
#include <osgEarth/Map>
#include "osgEarthUtil/Sky"

namespace VRSim3D { namespace SilverLining
{
    class SilverLiningContext;
    /**
     * Node that roots the silverlining adapter.
     */
    class VRSim3D_EXPORT SilverLiningNode : public osg::Group
    {
    public:
        SilverLiningNode( const osgEarth::Map* map,osgEarth::Util::SkyNode* skyNode,const std::string &source );

    public: 

    public: // osg::Node

        void traverse(osg::NodeVisitor&);

    protected:
        virtual ~SilverLiningNode();
		void onSetDateTime();

        osg::ref_ptr<SilverLiningContext> m_SL;
		osg::Geode* m_geode;
		osg::Drawable* _skyDrawable;
		osg::ref_ptr<osg::Drawable> m_cloudsDrawable;
		double m_lastAltitude;
		osgEarth::Util::SkyNode* m_skyNode;
    };

} } // namespace osgEarth::SilverLining