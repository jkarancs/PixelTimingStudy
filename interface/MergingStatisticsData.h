#ifndef MERGING_STATISTICS_STRUCTURE_H
#define MERGING_STATISTICS_STRUCTURE_H

#include "../interface/ModuleData.h"

#include <string>

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999
#endif

class MergingStatisticsData
{
	public:
		int clusterSizes[2];
		float clusterCharges[2];
		int totalPixelADCs[2];
		int medianPixelADCs[2];
		float averagePixelADCs[2];
		float sizeDifference;
		float chargeDifference;
		// int closestPixelADCDifference;
		int isSplitCluster;
		float averagePixelADCDifference;
		int medianPixelADCDifference;
		// Module
		ModuleData mod;
		ModuleData mod_on;
		// Instantiation
		MergingStatisticsData() { init(); mod.init(); mod_on.init(); };
		void init()
		{
			for(int i = 0; i < 2; ++i)
			{
				clusterSizes[i] = NOVAL_I;
				clusterCharges[i] = NOVAL_F;
				totalPixelADCs[i] = NOVAL_I;
				medianPixelADCs[i] = NOVAL_I;
				averagePixelADCs[i] = NOVAL_F;
			}
			sizeDifference = NOVAL_F;
			chargeDifference = NOVAL_F;
			// closestPixelADCDifference = NOVAL_I;
			isSplitCluster = NOVAL_I;
			averagePixelADCDifference = NOVAL_F;
			medianPixelADCDifference = NOVAL_I;
		};
		// Data list
		// std::string list = "clusterSizes[2]/I:clusterCharges[2]/F:totalPixelADCs[2]/I:medianPixelADCs[2]/I:averagePixelADCs[2]/F:sizeDifference/F:chargeDifference/F:closestPixelADCDifference/I:averagePixelADCDifference/F:medianPixelADCDifference/I";
		std::string list = "clusterSizes[2]/I:clusterCharges[2]/F:totalPixelADCs[2]/I:medianPixelADCs[2]/I:averagePixelADCs[2]/F:sizeDifference/F:chargeDifference/F:isSplitCluster/I:averagePixelADCDifference/F:medianPixelADCDifference/I";
};

#endif