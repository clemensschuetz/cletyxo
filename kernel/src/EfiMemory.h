#pragma once
#include <stdint.h>

namespace Visionizer
{
	struct EFI_MEMORY_DESCRIPTOR 
	{
		uint32_t type; // What type is it?
		void* physAddr; // Physical Address
		void* virtAddr; // Virtual Address
		uint64_t numPages; // Number of pages
		uint64_t attribs; // Attributes
	};

	extern const char* EFI_MEMORY_TYPE_STRINGS[];
}
