#include "pci.h"
#include "x86io.h"
#include "text_console.h"

extern Text_Console GlobalConsole;


namespace PCI {
    // PCIDeviceHeader** pci_devices[256][32];

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
        for(uint8_t device = 0; device < 32; device++) {
            check_device(bus, device);
        }
    }

    bool check_device(uint8_t bus, uint8_t device, uint8_t function) {
        uint32_t tmp = pci_read(bus, device, function, Device_Vendor_Offset);
        if( (tmp & 0xFFFF) != 0xFFFF){
            // (*pci_devices)[bus][device]->VendorID = (uint16_t)(tmp & 0xFFFF);
            // (*pci_devices)[bus][device]->DeviceID = (uint16_t)((tmp >> 16) & 0xFFFF);
            // tmp = pciRead(bus, device, 0, Command_Status_Offset);
            // (*pci_devices)[bus][device]->Command = (uint16_t)(tmp & 0xFFFF);
            // (*pci_devices)[bus][device]->Status = (uint16_t)((tmp >> 16) & 0xFFFF);
            // tmp = pciRead(bus, device, 0, Class_Sub_ProgIF_RevID_Offset);
            // (*pci_devices)[bus][device]->RevisionID = (uint8_t)(tmp & 0xFF);
            // (*pci_devices)[bus][device]->ProgIF = (uint8_t)((tmp >> 8) & 0xFF);
            // (*pci_devices)[bus][device]->Subclass = (uint8_t)((tmp >> 16) & 0xFF);
            // (*pci_devices)[bus][device]->Class = (uint8_t)((tmp >> 24) & 0xFF);
            // tmp = pciRead(bus, device, 0, BIST_HeaderType_Latency_CacheLine_Offset);
            // (*pci_devices)[bus][device]->CacheLineSize = (uint8_t)(tmp & 0xFF);
            // (*pci_devices)[bus][device]->LatencyTimer = (uint8_t)((tmp >> 8) & 0xFF);
            // (*pci_devices)[bus][device]->HeaderType = (uint8_t)((tmp >> 16) & 0xFF);
            // (*pci_devices)[bus][device]->BIST = (uint8_t)((tmp >> 24) & 0xFF);
            if(function == 0) {
                GlobalConsole.print_str("Found: "); GlobalConsole.print_num(bus); GlobalConsole.print_str(":"); GlobalConsole.print_num(device);
            } else {
                GlobalConsole.print_str("   "); GlobalConsole.print_num(bus); GlobalConsole.print_str(":"); GlobalConsole.print_num(device);GlobalConsole.print_str(":"); GlobalConsole.print_num(function);
            }
            GlobalConsole.print_str(". VendID: "); GlobalConsole.print_hex((uint16_t)(tmp & 0xFFFF));
            GlobalConsole.print_str(", DevID: "); GlobalConsole.print_hex((uint16_t)((tmp >> 16) & 0xFFFF));
            tmp = pci_read(bus, device, function, Class_Sub_ProgIF_RevID_Offset);
            GlobalConsole.print_str(", "); GlobalConsole.print_hex(tmp);
            tmp = pci_read(bus, device, function, BIST_HeaderType_Latency_CacheLine_Offset);
            GlobalConsole.print_str(", Header: "); GlobalConsole.print_hex((uint8_t)((tmp >> 16) & 0xFF));
            GlobalConsole.print_line();
            if((uint8_t)((tmp >> 16) & 0xFF) & 0x80 && function == 0){
                for(uint8_t i = 1; i < 8; i++){
                    check_device(bus, device, i);
                }
            }
            return true;
        }
        return false;
    }

}
