#pragma once

#ifndef MAPIOBJECTUPDATE_H
#define MAPIOBJECTUPDATE_H

#include "MAPIObjectUpdateEnum.h"
#include "MAPIObjectRequest.h"

/* 
* 
Description: This class is used as a record to indicate that some resource has been modified (i.e, created or deleted);
the updates can then be passed around to other various components, as a way to signal that something happened.

It is used by functions such as MShaderController::insertDynamicBufferData and MShaderController::processBindingDeleteRequests.

*/

class MAPIObjectUpdate
{
	public:
		MAPIObjectUpdate() {};
		MAPIObjectUpdate(MAPIObjectUpdateEnum in_updateType, MAPIObjectRequest in_requestForUpdate) :
			updateType(in_updateType),
			requestForUpdate(in_requestForUpdate)
		{};

		MAPIObjectUpdate(MAPIObjectUpdateEnum in_updateType, MAPIObjectMetadata in_metadataForUpdate) :
			updateType(in_updateType)
		{
			switch (in_metadataForUpdate.fetchMdMapKeyType())
			{
				case MAPIObjectMapKeyType::STRING_KEYTYPE:
				{
					MAPIObjectRequest stringedRequest(in_metadataForUpdate.fetchMdObjectType(),
												      in_metadataForUpdate.fetchMdName());
					requestForUpdate = stringedRequest;
					break;
				}

				case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
				{
					MAPIObjectRequest keyedRequest(in_metadataForUpdate.fetchMdObjectType(),
						                              in_metadataForUpdate.fetchMdKeyValue(),
													  in_metadataForUpdate.fetchMdKeyMapName());
					requestForUpdate = keyedRequest;
					break;
				}

			}
		};

		void printUpdateData()
		{
			switch (updateType)
			{
				case MAPIObjectUpdateEnum::MAPI_OBJECT_CREATED:
				{
					std::cout << "Update type: MAPI_OBJECT_CREATED ";
					break;
				}

				case MAPIObjectUpdateEnum::MAPI_OBJECT_DELETED:
				{
					std::cout << "Update type: MAPI_OBJECT_DELETED ";
					break;
				}
			}
			requestForUpdate.printRequestData();
			std::cout << std::endl;
		}

		MAPIObjectUpdateEnum updateType = MAPIObjectUpdateEnum::MAPI_OBJECT_CREATED;	// created is default; may also be set to delete.
		MAPIObjectRequest requestForUpdate;
};

#endif
