/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2006 Refractions Research Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 *
 **********************************************************************
 *
 * Last port: noding/FastSegmentSetIntersectionFinder.java r388 (JTS-1.12)
 *
 **********************************************************************/

#ifndef GEOS_NODING_FASTSEGMENTSETINTERSECTIONFINDER_H
#define GEOS_NODING_FASTSEGMENTSETINTERSECTIONFINDER_H

#include <geos/noding/SegmentString.h>
#include <geos/noding/MCIndexSegmentSetMutualIntersector.h>

#include <memory>

#include <geos/index/chain/MonotoneChain.h>
#include <geos/index/SpatialIndex.h>

//forward declarations
namespace geos {
	namespace noding {
		class SegmentIntersectionDetector;
	}
}

namespace geos {
namespace noding { // geos::noding

/** \brief
 * Finds if two sets of {@link SegmentStrings}s intersect.
 *
 * Uses indexing for fast performance and to optimize repeated tests
 * against a target set of lines.
 * Short-circuited to return as soon an intersection is found.
 *
 * @version 1.7
 */
class FastSegmentSetIntersectionFinder
{
private:
	typedef std::vector<std::unique_ptr<index::chain::MonotoneChain>> MonoChains;
	MonoChains chainStore;
	/*
	 * The {@link SpatialIndex} used should be something that supports
	 * envelope (range) queries efficiently (such as a {@link Quadtree}
	 * or {@link STRtree}.
	 */
	std::unique_ptr<index::SpatialIndex> index;

	void setBaseSegments(SegmentString::ConstVect* segStrings);
	void addToIndex(SegmentString* segStr, int & indexCounter);
protected:
public:
	FastSegmentSetIntersectionFinder( SegmentString::ConstVect * baseSegStrings);

	~FastSegmentSetIntersectionFinder() = default;

	bool intersects( SegmentString::ConstVect * segStrings);
	bool intersects( SegmentString::ConstVect * segStrings, SegmentIntersectionDetector * intDetector);

};

} // geos::noding
} // geos

#endif // GEOS_NODING_FASTSEGMENTSETINTERSECTIONFINDER_H

