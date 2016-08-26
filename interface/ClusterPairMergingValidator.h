#ifndef CLUSTER_PAIR_MERGING_VALIDATOR_H
#define CLUSTER_PAIR_MERGING_VALIDATOR_H

#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "../interface/ClusterGeometry.h"

class ClusterPairMergingValidator
{
	public:
		class ClusterForValidator
		{
			public:
				explicit ClusterForValidator(const SiPixelCluster& clusterArg);
				void swap(ClusterForValidator&& other);
				const SiPixelCluster* cluster;
				const SiPixelCluster::Pixel* startPixel = nullptr;
				const SiPixelCluster::Pixel* endPixel   = nullptr;
				int length;
				float dir;
		};

	private:
		ClusterForValidator firstCluster;
		ClusterForValidator secondCluster;

	public:
		ClusterPairMergingValidator(const SiPixelCluster& first, const SiPixelCluster& second);
		// Check if the distance between the two columns corresponds to a double pixel
		bool isMissingPartDoubleColumn();
		// Check if the merged cluster would be too long
		bool isPairNotTooLong();
		// Check if the direction of the "longest distance" in the clusters are similar
		bool isDirSimilar();
};

#endif