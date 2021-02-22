#include "ACPI.h" 


namespace Visionizer
{
    namespace ACPI
    {
        void* FindTable(SDTHeader* sdtHdr, char* signature)
        {
            int entries = (sdtHdr->Length - sizeof(ACPI::SDTHeader)) / 8;
            for (int i = 0; i < entries; i++)
            {
                ACPI::SDTHeader* newSDTHeader = (ACPI::SDTHeader*)*(uint64_t*)((uint64_t)sdtHdr + sizeof(ACPI::SDTHeader) + (i * 8));
                for (int j = 0; j < 4; j++)
                {
                    if(newSDTHeader->Signature[j] != signature[j])
                        break;
                    
                    if (j == 3) return newSDTHeader;
                }
            }
            // Did not get a hdr
            return 0;

        }
    }
}