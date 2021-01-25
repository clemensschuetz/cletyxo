#include "EfiMemory.h"


namespace Visionizer
{
	const char* EFI_MEMORY_TYPE_STRINGS[]
	{
		"EfiReservedMemoryTyped",
		"EfiLoaderCode",
		"EfiLoaderData",
		"EfiBootServicesCode",
		"EfiBootServicesData",
		"EfiRuntimeServicesCode",
		"EfiRuntimeServicesData",
		"EfiConventionalMemory",
		"EfiUnusableMemory",
		"EfiACPIReclaimMemory",
		"EfiACPIMemoryNVS",
		"EfiMemoryMappedIO",
		"EfiMemoryMappedIOPortSpace",
		"EfiPalCode",
	};
}
