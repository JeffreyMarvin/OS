#pragma once

#include <stdint.h>



namespace PCI {

    const uint16_t PCI_CONFIG_PORT = 0xCF8;
    const uint16_t PCI_DATA_PORT = 0xCFC;
    
    const uint8_t Device_Vendor_Offset = 0x00;
    const uint8_t Command_Status_Offset = 0x04;
    const uint8_t Class_Sub_ProgIF_RevID_Offset = 0x08;
    const uint8_t BIST_HeaderType_Latency_CacheLine_Offset = 0x0c;

    struct PCIDeviceHeader{
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

    bool check_device(uint8_t bus, uint8_t device, uint8_t function = 0) ;
    void enumerate_bus(uint8_t bus);
    // extern PCIDeviceHeader** pci_devices[256][32];
}
