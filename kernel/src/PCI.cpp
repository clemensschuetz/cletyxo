#include "PCI.cpp" 
#include "BasicRenderer.h"
#include "cstr.h"


namespace Visionizer
{
    namespace PCI
    {
        void EnumerateFunction(uint64_t deviceAddress, uint64_t function)
        {
            uint64_t offset = function << 12;

            uint64_t functionAddress = deviceAddress + offset;
            GlobalPTM.MapMemory((void*)functionAddress, (void*)functionAddress);

            PCIDeviceHeader* pciDeviceHdr = (PCIDeviceHeader*)functionAddress;

            // A few checks
            if (pciDeviceHdr->DeviceID == 0) return;
            if (pciDeviceHdr->DeviceID == 0xFFFF) return;

            GlobalRenderer->Print(toHString(pciDeviceHdr->VendorID));
            GlobalRenderer->Print(" ");
            GlobalRenderer->Print(toHString(pciDeviceHdr->DeviceID));
            GlobalRenderer->NextLine();

        }

        void EnumerateDevice(uint64_t busAddress, uint64_t device)
        {
            uint64_t offset = device << 15;

            uint64_t deviceAddress = busAddress + offset;
            GlobalPTM.MapMemory((void*)deviceAddress, (void*)deviceAddress);

            PCIDeviceHeader* pciDeviceHdr = (PCIDeviceHeader*)deviceAddress;

            // A few checks
            if (pciDeviceHdr->DeviceID == 0) return;
            if (pciDeviceHdr->DeviceID == 0xFFFF) return;

            for (uin64_t function = 0; function < 32; function++)
            {
                EnumerateFunction(deviceAddress, function);
            }
        }
        
        void EnumerateBus(uint64_t baseAddress, uint64_t bus)
        {
            uint64_t offset = bus << 20;

            uint64_t busAddress = baseAddress + offset;
            GlobalPTM.MapMemory((void*)busAddress, (void*)busAddress);

            PCIDeviceHeader* pciDeviceHdr = (PCIDeviceHeader*)baseAddress;

            // A few checks
            if (pciDeviceHdr->DeviceID == 0) return;
            if (pciDeviceHdr->DeviceID == 0xFFFF) return;

            for (uin64_t device = 0; device < 32; device++)
            {
                EnumerateDevice(baseAddress, device);
            }

        }



        void EnumeratePCI(ACPI::MCFGHeader* mcfg)
        {
            int entries = ((mcfg->Header.Length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);
            for (int i = 0; i < entries, i++)
            {
                ACPI::DeviceConfig* newDeviceConfig = (ACPI::DeviceConfig*)((uint64_t)mcfg + sizeof(ACPI::MCFGHeader) + (sizeof(ACPI::DeviceConfig) * i));
                for (uint64_t bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++)
                {
                    EnumerateBus(newDeviceConfig->BaseAddress, bus);
                }
            }
        }
    }
}