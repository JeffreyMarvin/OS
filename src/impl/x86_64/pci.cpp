#include "pci.h"
#include "x86io.h"
#include "text_console.h"
#include "heap.h"

extern Text_Console GlobalConsole;

namespace PCI {

    uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
        uint32_t pci_address = 0x80000000;  //Bit 32 = Enable Bit, Bits 24-31 are reserved
        pci_address |= ((uint32_t)bus) << 16; //Bits 23-16
        pci_address |= ((uint32_t)device & 0x1f) << 11; //Bits 15-11
        pci_address |= ((uint32_t)function & 0x07) << 8; //Bits 10-8
        pci_address |= offset & 0xfc;  //Offset must always be 32-bit aligned
        outl(PCI_CONFIG_PORT, pci_address);
        return inl(PCI_DATA_PORT);
    }

    PCIBus* enumerate_bus(uint8_t bus_number){
        PCIBus* bus = new PCIBus;
        bus->bus_number = bus_number;
        for(uint8_t device = 0; device < DEVICES_PER_BUS; device++) {
            bus->devices[device] = check_device(bus->bus_number, device);
        }
        return bus;
    }

    PCIDevice* check_device(uint8_t bus_number, uint8_t device_number, uint8_t function_number) {
        uint32_t tmp = pci_read(bus_number, device_number, function_number, 0 * REG_OFFSET);
        if( (tmp & 0xFFFF) != 0xFFFF){
            PCIDevice* device = new PCIDevice;
            device->device_number = device_number;
            device->bus_number = bus_number;
            device->header = new PCIDeviceHeader;
            device->header->reg[0] = tmp;
            for(int i = 1; i < 0xF; i++) {
                device->header->reg[i] = pci_read(bus_number, device_number, function_number, i * REG_OFFSET);
            }
            if(device->header->HeaderType & HEADER_TYPE_MULTI_FUNCTION_DEVICE && function_number == 0){
                PCIDevice* tmp;
                for(uint8_t i = 1; i < 8; i++){
                    tmp = check_device(bus_number, device_number, i);
                    if(tmp) {
                        device->functions[i] = tmp->header;
                    } else {
                        device->functions[i] = nullptr;
                    }
                }
            }
            return device;
        }
        return nullptr;
    }

}
