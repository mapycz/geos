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
 * Last port: noding/MCIndexSegmentSetMutualIntersector.java r388 (JTS-1.12)
 *
 **********************************************************************/

#include <geos/noding/MCIndexSegmentSetMutualIntersector.h>
#include <geos/noding/SegmentSetMutualIntersector.h>
#include <geos/noding/SegmentString.h>
#include <geos/noding/SegmentIntersector.h>
#include <geos/index/SpatialIndex.h>
#include <geos/index/chain/MonotoneChain.h>
#include <geos/index/chain/MonotoneChainBuilder.h>
// std
#include <cstddef>

using namespace geos::index::chain;

namespace geos {
namespace noding { // geos::noding

/*private*/
void
MCIndexSegmentSetMutualIntersector::intersectChains(index::SpatialIndex * index, const MonoChains & monoChains)
{
    MCIndexSegmentSetMutualIntersector::SegmentOverlapAction overlapAction( *segInt);

    for (const auto& queryChain : monoChains)
    {
        std::vector<void*> overlapChains;
        index->query( &(queryChain->getEnvelope()), overlapChains);

        for (std::size_t j = 0, nj = overlapChains.size(); j < nj; j++)
        {
            MonotoneChain * testChain = (MonotoneChain *)(overlapChains[j]);

            queryChain->computeOverlaps( testChain, &overlapAction);
            if (segInt->isDone())
                return;
        }
    }
}

/*private*/
void
MCIndexSegmentSetMutualIntersector::addToMonoChains(SegmentString* segStr, MonoChains & monoChains, int & processCounter)
{
    MonoChains segChains;
    MonotoneChainBuilder::getChains(segStr->getCoordinates(),
                                    segStr, segChains);

    MonoChains::size_type n = segChains.size();
    monoChains.reserve(monoChains.size() + n);
    for (auto& mc : segChains)
    {
        mc->setId( processCounter++ );
        monoChains.push_back(std::move(mc));
    }
}

/* public */
MCIndexSegmentSetMutualIntersector::MCIndexSegmentSetMutualIntersector()
{
}

/* public */
MCIndexSegmentSetMutualIntersector::~MCIndexSegmentSetMutualIntersector()
{
    MonoChains::iterator i, e;
}

/*public*/
void
MCIndexSegmentSetMutualIntersector::process(index::SpatialIndex * index,
                                            SegmentString::ConstVect * segStrings)
{
    int processCounter = 0; //indexCounter + 1;
    MonoChains monoChains;

    for (SegmentString::ConstVect::size_type i = 0, n = segStrings->size(); i < n; i++)
    {
        SegmentString * seg = (SegmentString *)((*segStrings)[i]);
        addToMonoChains( seg, monoChains, processCounter);
    }
    intersectChains(index, monoChains);
}

/* public */
void
MCIndexSegmentSetMutualIntersector::SegmentOverlapAction::overlap(
    MonotoneChain& mc1, size_t start1, MonotoneChain& mc2, size_t start2)
{
    SegmentString * ss1 = (SegmentString *)(mc1.getContext());
    SegmentString * ss2 = (SegmentString *)(mc2.getContext());

    si.processIntersections(ss1, start1, ss2, start2);
}

} // geos::noding
} // geos

