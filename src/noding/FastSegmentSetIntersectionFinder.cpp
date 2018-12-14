/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 **********************************************************************
 *
 * Last port: noding/FastSegmentSetIntersectionFinder.java r388 (JTS-1.12)
 *
 **********************************************************************/

#include <geos/noding/FastSegmentSetIntersectionFinder.h>
#include <geos/noding/SegmentString.h>
#include <geos/noding/SegmentIntersectionDetector.h>
#include <geos/noding/SegmentSetMutualIntersector.h>
#include <geos/noding/MCIndexSegmentSetMutualIntersector.h>
#include <geos/algorithm/LineIntersector.h>
#include <geos/index/chain/MonotoneChainBuilder.h>
#include <geos/index/strtree/STRtree.h>

using namespace geos::index::chain;

namespace geos {
namespace noding { // geos::noding

/*
 * private:
 */

void
FastSegmentSetIntersectionFinder::setBaseSegments(SegmentString::ConstVect* segStrings)
{
	// NOTE - mloskot: const qualifier is removed silently, dirty.

	int indexCounter = 0;
	for (std::size_t i = 0, n = segStrings->size(); i < n; i++)
	{
		const SegmentString* css = (*segStrings)[i];
		SegmentString* ss = const_cast<SegmentString*>(css);
		addToIndex(ss, indexCounter);
	}
}

void
FastSegmentSetIntersectionFinder::addToIndex(SegmentString* segStr, int & indexCounter)
{
	MonoChains segChains;
	MonotoneChainBuilder::getChains(segStr->getCoordinates(), segStr, segChains);

	MonoChains::size_type n = segChains.size();
	chainStore.reserve(chainStore.size() + n);
	for (auto& mc : segChains)
	{
		mc->setId(indexCounter++);
		this->index->insert(&(mc->getEnvelope()), mc.get());
		chainStore.push_back(std::move(mc));
	}
}

/*
 * protected:
 */

/*
 * public:
 */
FastSegmentSetIntersectionFinder::
FastSegmentSetIntersectionFinder( noding::SegmentString::ConstVect * baseSegStrings) :
	index(new geos::index::strtree::STRtree())
{
	setBaseSegments(baseSegStrings);
}

bool
FastSegmentSetIntersectionFinder::
intersects( noding::SegmentString::ConstVect * segStrings)
{
	std::unique_ptr<geos::algorithm::LineIntersector> lineIntersector(new LineIntersector());
	SegmentIntersectionDetector intFinder(lineIntersector.get());

	return this->intersects( segStrings, &intFinder);
}

bool
FastSegmentSetIntersectionFinder::
intersects( noding::SegmentString::ConstVect * segStrings,
			SegmentIntersectionDetector * intDetector)
{
	std::unique_ptr<MCIndexSegmentSetMutualIntersector> segSetMutInt(
		new MCIndexSegmentSetMutualIntersector());
	segSetMutInt->setSegmentIntersector( intDetector);
	segSetMutInt->process(index.get(), segStrings);

	return intDetector->hasIntersection();
}

} // geos::noding
} // geos

