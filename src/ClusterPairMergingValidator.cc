#include "../interface/ClusterPairMergingValidator.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <iostream>

// #include <iostream>

ClusterPairMergingValidator::ClusterForValidator::ClusterForValidator(const SiPixelCluster& clusterArg)
: cluster(&clusterArg)
{
	float maxLength = 0;
	int maxPixelPairCharge = 0;
	const auto& pixels = cluster -> pixels();
	typedef std::vector<SiPixelCluster::Pixel>::const_iterator PixelConstIt_t;
	if(pixels.size() == 0)
	{
		length = 0;
		dir = -1;
		return;
	}
	if(pixels.size() == 1)
	{
		startPixel = &(*pixels.begin());
		endPixel   = &(*pixels.begin());
		length     = 0;
		dir        = -1;
		return;
	}
	// std::cerr << "---------- Begin node check ----------" << std::endl;
	// Find longest distance
	for(PixelConstIt_t startPixelCandidate = pixels.begin(); startPixelCandidate != pixels.end(); ++startPixelCandidate)
	{
		for(PixelConstIt_t endPixelCandidate = pixels.end() - 1; endPixelCandidate != startPixelCandidate; --endPixelCandidate)
		{
			// std::cerr << "Node pair: " << startPixelCandidate - pixels.begin() << ", " << endPixelCandidate - pixels.begin() << std::endl;
			bool saveAsNewMax = false;
			float length = ClusterGeometry::pixelAbsDistance(*startPixelCandidate, *endPixelCandidate);
			// Save the furthermost pixelpair
			if(maxLength < length) saveAsNewMax = true;
			// Check charge for pixels with the same length
			if(length == maxLength)
			{
				int pixelPairCharge = startPixelCandidate -> adc + endPixelCandidate -> adc;
				if(maxPixelPairCharge < pixelPairCharge) saveAsNewMax = true;
			}
			if(saveAsNewMax == true)
			{
				startPixel = &(*startPixelCandidate);
				endPixel   = &(*endPixelCandidate);
				maxLength  = length;
				maxPixelPairCharge = startPixel -> adc + endPixel -> adc;
			}
		}
	}
	// std::cerr << "----------  End node check  ----------" << std::endl;
	// Check start-end order
	if(!startPixel || !endPixel)
	{
		length = 0;
		dir = -1;
		return;
	}
	if(endPixel -> x < startPixel -> x) std::swap(endPixel, startPixel);
	// Save cluster data
	length = maxLength;
	dir = std::atan2(static_cast<float>(endPixel -> y - startPixel -> y), static_cast<float>(endPixel -> x - startPixel -> x));
}

void ClusterPairMergingValidator::ClusterForValidator::swap(ClusterForValidator&& other)
{
	std::swap(cluster, other.cluster);
	std::swap(startPixel, other.startPixel);
	std::swap(endPixel, other.endPixel);
	std::swap(length, other.length);
	std::swap(dir, other.dir);
}

ClusterPairMergingValidator::ClusterPairMergingValidator(const SiPixelCluster& first, const SiPixelCluster& second)
: firstCluster(first), secondCluster(second)
{
	if(second.x() < first.x())
	{
		firstCluster.swap(std::move(secondCluster));
	}
}

// Check if the distance between the two columns corresponds to a double pixel
bool ClusterPairMergingValidator::isMissingPartDoubleColumn()
{
	if(firstCluster.endPixel -> x % 2 == 1 && secondCluster.startPixel -> x == firstCluster.endPixel -> x + 3)
	{
		if(std::abs(firstCluster.endPixel -> y - secondCluster.startPixel -> y) < 4)
		{
			return true;
		}
		return false;
	}
	return false;
}

// Check if the merged cluster would be too long
bool ClusterPairMergingValidator::isPairNotTooLong()
{
	if(12 < firstCluster.length + secondCluster.length)
	{
		return false;
	}
	return true;
}

// Check if the direction of the "longest distance" in the clusters are similar
bool ClusterPairMergingValidator::isDirSimilar()
{
	float dirDiff = std::abs(secondCluster.dir - firstCluster.dir);
	if(dirDiff < M_PI / 4) // 45 degrees
	{
		return true;
	}
	return false;
}


