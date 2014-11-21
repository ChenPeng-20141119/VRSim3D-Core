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
#include <osg/Drawable>
#include <osg/RenderInfo>

namespace VRSim3D {  namespace SilverLining
{
    class SilverLiningContext;

    /**
     * Custom drawable for rendering the SilverLining clouds
     */
    class CloudsDrawable : public osg::Drawable
    {
    public:
        CloudsDrawable(SilverLiningContext* SL =0L);
        META_Object(SilverLining, CloudsDrawable);

		/* Sets whether to draw this item */
		void setDraw(bool draw);
     
    public: // osg::Drawable

        // custom draw (called with an active GC)
        void drawImplementation(osg::RenderInfo& ri) const;

        // custom bounds computation
        osg::BoundingBox computeBound() const;

    protected:
        virtual ~CloudsDrawable() { }

        osg::observer_ptr<SilverLiningContext> _SL;
		bool _draw;
        
        CloudsDrawable(const CloudsDrawable& copy, const osg::CopyOp& op=osg::CopyOp::SHALLOW_COPY) { }
    };

} }  // namespace VRSim3D::SilverLining
