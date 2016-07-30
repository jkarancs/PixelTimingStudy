#include "../interface/FedErrorFetcher.h"

std::map<uint32_t, int> FedErrorFetcher::getFedErrors(const edm::Event& iEvent, const edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>>& rawDataErrorToken)
{
	std::map<uint32_t, int> federrors;
	edm::Handle<edm::DetSetVector<SiPixelRawDataError>> siPixelRawDataErrorCollectionHandle;
	iEvent.getByToken(rawDataErrorToken,                siPixelRawDataErrorCollectionHandle);

	if(siPixelRawDataErrorCollectionHandle.isValid())
	{
		for(const auto& pixel_error_set: *siPixelRawDataErrorCollectionHandle)
		{
			for(const auto& pixel_error: pixel_error_set)
			{
				if(pixel_error_set.detId()!=0xffffffff)
				{
					DetId detId(pixel_error_set.detId());
					int type = pixel_error.getType();
					federrors.insert(std::pair<uint32_t,int>(detId.rawId(), type));
					// if(type>24&&type<=40) federr[type-25]++;
					// else edm::LogError("data_access") << "New FED error with not recognised error-type: " << type << std::endl;
				}
			}
		}
	}
	return federrors;
}