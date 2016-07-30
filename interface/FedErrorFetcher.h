#ifndef FED_ERROR_FETCHER_H
#define FED_ERROR_FETCHER_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"

#include <map>

class FedErrorFetcher
{
	private:
		FedErrorFetcher() = delete;
	public:
		~FedErrorFetcher();
		static std::map<uint32_t, int> getFedErrors(const edm::Event& iEvent, const edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>>& federrors);
};

#endif