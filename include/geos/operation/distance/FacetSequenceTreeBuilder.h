/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2016 Daniel Baston
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 **********************************************************************
 *
 * Last port: operation/distance/FacetSequenceTreeBuilder.java (JTS-1.14)
 *
 **********************************************************************/

#ifndef GEOS_OPERATION_DISTANCE_FACETSEQUENCETREEBUILDER_H
#define GEOS_OPERATION_DISTANCE_FACETSEQUENCETREEBUILDER_H

#include <geos/index/strtree/STRtree.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/operation/distance/FacetSequence.h>

using namespace geos::geom;
using namespace geos::index::strtree;
using namespace geos::operation::distance;

namespace geos {
    namespace operation {
        namespace distance {
            class FacetSequenceTreeBuilder {
            private:
                // 6 seems to be a good facet sequence size
                static const int FACET_SEQUENCE_SIZE = 6;

                // Seems to be better to use a minimum node capacity
                static const int STR_TREE_NODE_CAPACITY = 4;

                static void addFacetSequences(const CoordinateSequence* pts, std::vector<FacetSequence*> & sections);
                static std::vector<FacetSequence*> * computeFacetSequences(const Geometry* g);

            public:
                static STRtree* build(const Geometry* g);
            };
        }
    }
}

#endif //GEOS_FACETSEQUENCETREEBUILDER_H
