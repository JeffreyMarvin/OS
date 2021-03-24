#include "pci.h"
#include "x86io.h"
#include "text_console.h"
#include "heap.h"

extern Text_Console GlobalConsole;
PCI::PCIDeviceHeader* pci_devices;


namespace PCI {

    void create_device_array(){
        pci_devices = (PCIDeviceHeader*)malloc((uint64_t)NUM_BUSES * DEVICES_PER_BUS * FUNCTIONS_PER_DEVICE * sizeof(PCIDeviceHeader));
    }

    uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
        uint32_t pci_address = 0x80000000;  //Bit 32 = Enable Bit, Bits 24-31 are reserved
        pci_address |= ((uint32_t)bus) << 16; //Bits 23-16
        pci_address |= ((uint32_t)device & 0x1f) << 11; //Bits 15-11
        pci_address |= ((uint32_t)function & 0x07) << 8; //Bits 10-8
        pci_address |= offset & 0xfc;  //Offset must always be 32-bit aligned
        outl(PCI_CONFIG_PORT, pci_address);
        return inl(PCI_DATA_PORT);
    }

    void enumerate_bus(uint8_t bus){
        for(uint8_t device = 0; device < DEVICES_PER_BUS; device++) {
            check_device(bus, device);
        }
    }

    bool check_device(uint8_t bus, uint8_t device, uint8_t function) {
        uint64_t offset = ((uint64_t)bus * DEVICES_PER_BUS + device) * FUNCTIONS_PER_DEVICE + function;
        uint32_t tmp = pci_read(bus, device, function, Device_Vendor_Offset);
        if( (tmp & 0xFFFF) != 0xFFFF){
            PCIDeviceHeader* header = new PCIDeviceHeader;
            header->VendorID = (uint16_t)(tmp & 0xFFFF);
            header->DeviceID = (uint16_t)((tmp >> 16) & 0xFFFF);
            tmp = pci_read(bus, device, 0, Command_Status_Offset);
            header->Command = (uint16_t)(tmp & 0xFFFF);
            header->Status = (uint16_t)((tmp >> 16) & 0xFFFF);
            tmp = pci_read(bus, device, 0, Class_Sub_ProgIF_RevID_Offset);
            header->RevisionID = (uint8_t)(tmp & 0xFF);
            header->ProgIF = (uint8_t)((tmp >> 8) & 0xFF);
            header->Subclass = (uint8_t)((tmp >> 16) & 0xFF);
            header->Class = (uint8_t)((tmp >> 24) & 0xFF);
            tmp = pci_read(bus, device, 0, BIST_HeaderType_Latency_CacheLine_Offset);
            header->CacheLineSize = (uint8_t)(tmp & 0xFF);
            header->LatencyTimer = (uint8_t)((tmp >> 8) & 0xFF);
            header->HeaderType = (uint8_t)((tmp >> 16) & 0xFF);
            header->BIST = (uint8_t)((tmp >> 24) & 0xFF);
            pci_devices[offset] = *header;
            if(header->HeaderType & 0x80 && function == 0){
                for(uint8_t i = 1; i < 8; i++){
                    check_device(bus, device, i);
                }
            }
            return true;
        }
        pci_devices[offset].VendorID = 0xFFFF;
        return false;
    }

}
