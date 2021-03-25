#pragma once

#include <stdint.h>



namespace PCI {

    const uint16_t PCI_CONFIG_PORT = 0xCF8;
    const uint16_t PCI_DATA_PORT = 0xCFC;
    
    const uint8_t REG_OFFSET = 0x04;

    const uint16_t NUM_BUSES = 256;
    const uint8_t DEVICES_PER_BUS = 32;
    const uint8_t FUNCTIONS_PER_DEVICE = 8;
    const uint8_t HEADER_TYPE_MULTI_FUNCTION_DEVICE = 0x80;

    struct PCIDeviceHeader{
        union {
            struct {
                uint16_t VendorID;
                uint16_t DeviceID;
                uint16_t Command;
                uint16_t Status;
                uint8_t RevisionID;
                uint8_t ProgIF;
                uint8_t Subclass;
                uint8_t Class;
                uint8_t CacheLineSize;
                uint8_t LatencyTimer;
                uint8_t HeaderType;
                uint8_t BIST;
                uint32_t BAR0;
                uint32_t BAR1;
                union {
                    uint32_t BAR2;
                    struct {
                        uint8_t PrimaryBusNumber;
                        uint8_t SecondaryBusNumber;
                        uint8_t SubordinateBusNumber;
                        uint8_t SecondaryLatencyTimer;
                    };
                };
                union {
                    uint32_t BAR3;
                    struct {
                        uint8_t IOBase;
                        uint8_t IOLimit;
                        uint16_t SecondaryStatus;
                    };
                };
                union {
                    uint32_t BAR4;
                    struct {
                        uint16_t MemBase;
                        uint16_t MemLimit;
                    };
                };
                union {
                    uint32_t BAR5;
                    struct {
                        uint16_t PrefetchMemBase;
                        uint16_t PrefetchMemLimit;
                    };
                };
                union {
                    uint32_t CardbusCISPtr;
                    uint32_t PrefetchBaseUppper;
                };
                union {
                    struct {
                        uint16_t SubsystemVendorID;
                        uint16_t SubsystemID;
                    };
                    uint32_t PrefetchLimitUpper;
                };
                union {
                    uint32_t ExpansionROMBaseAddress;
                    struct {
                        uint16_t IOBaseUpper;
                        uint16_t IOLimitUpper;
                    };
                };
                union {
                    struct {
                        uint8_t CapabilitiesPtr;
                        uint8_t Reserved[3];
                    };
                };
                union {
                    uint32_t Rsverved2;
                    uint32_t ExpansionRomBaseAddress;
                };
                uint8_t InterruptLine;
                uint8_t InterruptPin;
                union {
                    struct {
                        uint8_t MinGrant;
                        uint8_t MaxLatency;
                    };
                    uint16_t BridgeControl;
                };
            };
            uint32_t reg[0xF];
        };
    };

    struct PCIDevice
    {
        uint8_t bus_number;
        uint8_t device_number;
        PCIDeviceHeader* header;
        PCIDeviceHeader* functions[8];
    };

    struct PCIBus
    {
        uint8_t bus_number;
        PCIDevice* devices[32];
    };
    

    enum struct PCI_VendorID : uint16_t {
        Intel_Corp = 0x8086,
        AMD = 0x1022,
        Nvidia_Corporation = 0x10DE
    };

    enum struct PCI_Class : uint8_t {
        Unclassified = 0x00,
        Mass_Storage_Controller = 0x01,
        Network_Controller = 0x02,
        Display_Controller = 0x03,
        Multimedia_Controller = 0x04,
        Memory_Controller = 0x05,
        Bridge_Device = 0x06,
        Simple_Communication_Controller = 0x07,
        Base_System_Peripheral = 0x08,
        Input_Device_Controller = 0x09,
        Docking_Station = 0x0A,
        Processor = 0x0B,
        Serial_Bus_Controller = 0x0C,
        Wirelews_Controller = 0x0D,
        Intelligent_Controller = 0x0E,
        Satellite_Communication_Controller = 0x0F,
        Encryption_Controller = 0x10,
        Signal_Processing_Controller = 0x11,
        Processing_Accelerator = 0x12,
        Non_Essential_Instrumentation = 0x13,
        Co_Processor = 0x40,
        Unassigned = 0xFF
    };

    enum struct Mass_Storage_Controller_Subclass : uint8_t {
        SCSI_Bus_Controller = 0x00,
        IDE_Controller = 0x01,
        Floppy_Disk_Controller = 0x02,
        IPI_Bus_Controller = 0x03,
        RAID_Controller = 0x04,
        ATA_Controller = 0x05,
        SATA_Controller = 0x06,
        SAS_Controller = 0x07,
        NVM_Controller = 0x09,
        Other = 0x80
    };

    enum struct Serial_Bus_Controller_Subclass : uint8_t {
        Firewire_Controller = 0x00,
        ACCESS_Bus = 0x01,
        SSA = 0x02,
        USB_Controller = 0x03,
        Fibre_Channel = 0x04,
        SMBus = 0x05,
        InfiniBand = 0x06,
        IPMI_Interface = 0x07,
        SERCOS_Interface = 0x08,
        CANBus = 0x09,
        Other = 0x80
    };
    
    uint32_t pci_read(uint8_t bus, uint8_t devics, uint8_t function, uint8_t offset);

    PCIDevice* check_device(uint8_t bus_number, uint8_t device_number, uint8_t function_number = 0) ;
    PCIBus* enumerate_bus(uint8_t bus_number);
}
