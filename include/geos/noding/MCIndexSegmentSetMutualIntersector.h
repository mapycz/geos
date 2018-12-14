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
 * Last port: noding/MCIndexSegmentSetMutualIntersector.java r388 (JTS-1.12)
 *
 **********************************************************************/

#ifndef GEOS_NODING_MCINDEXSEGMENTSETMUTUALINTERSECTOR_H
#define GEOS_NODING_MCINDEXSEGMENTSETMUTUALINTERSECTOR_H

#include <geos/noding/SegmentSetMutualIntersector.h> // inherited
#include <geos/index/chain/MonotoneChainOverlapAction.h> // inherited

namespace geos {
	namespace index {
		class SpatialIndex;

		namespace chain {
			class MonotoneChain;
		}
	}
	namespace noding {
		class SegmentIntersector;
	}
}

//using namespace geos::index::strtree;

namespace geos {
namespace noding { // geos::noding

/** \brief
 * Intersects two sets of {@link SegmentStrings} using a index based
 * on {@link MonotoneChain}s and a {@link SpatialIndex}.
 *
 * @version 1.7
 */
class MCIndexSegmentSetMutualIntersector : public SegmentSetMutualIntersector
{
public:
	typedef std::vector<std::unique_ptr<index::chain::MonotoneChain>> MonoChains;

	MCIndexSegmentSetMutualIntersector();

	~MCIndexSegmentSetMutualIntersector() override;

	void process(SegmentString::ConstVect* segStrings) override {};
	void process(index::SpatialIndex * index, SegmentString::ConstVect* segStrings);

	void setBaseSegments(SegmentString::ConstVect* segStrings) override { }

    class SegmentOverlapAction : public index::chain::MonotoneChainOverlapAction
    {
    private:
        SegmentIntersector & si;

        // Declare type as noncopyable
        SegmentOverlapAction(const SegmentOverlapAction& other) = delete;
        SegmentOverlapAction& operator=(const SegmentOverlapAction& rhs) = delete;

    public:
        SegmentOverlapAction(SegmentIntersector & p_si) :
          index::chain::MonotoneChainOverlapAction(), si(p_si)
          {}

          void overlap(index::chain::MonotoneChain& mc1, std::size_t start1,
              index::chain::MonotoneChain& mc2, std::size_t start2) override;
    };

private:
	void intersectChains(index::SpatialIndex * index, const MonoChains & monoChains);

	void addToMonoChains( SegmentString * segStr, MonoChains & monoChains, int & processCounter);

};

} // namespace geos::noding
} // namespace geos

#endif // GEOS_NODING_MCINDEXSEGMENTSETMUTUALINTERSECTOR_H
