#include "ahci.h"
#include "text_console.h"
#include "page_table_manager.h"

extern Page_Table_Manager GlobalPageManager;
extern Text_Console GlobalConsole;

namespace AHCI{

    #define HBA_PORT_DEV_PRESENT 0x3
    #define HBA_PORT_IPM_ACTIVE 0x1
    #define SATA_SIG_ATAPI 0xEB140101
    #define SATA_SIG_ATA 0x00000101
    #define SATA_SIG_SEMB 0xC33C0101
    #define SATA_SIG_PM 0x96690101

    AHCI_Driver::AHCI_Driver(PCI::PCIDeviceHeader* pciBaseAddress){
        this->PCIBaseAddress = pciBaseAddress;
        GlobalConsole.print_line("AHCI Driver instance initialized");

        ABAR = (HBAMemory*)(pciBaseAddress->BAR5);

        GlobalPageManager.map_memory(ABAR, ABAR);
        ProbePorts();
        
    }

    AHCI_Driver::~AHCI_Driver(){

    }

    PortType CheckPortType(HBAPort* port){
        uint32_t sataStatus = port->sataStatus;

        uint8_t interfacePowerManagement = (sataStatus >> 8) & 0b111;
        uint8_t deviceDetection = sataStatus & 0b111;

        if (deviceDetection != HBA_PORT_DEV_PRESENT) return PortType::None;
        if (interfacePowerManagement != HBA_PORT_IPM_ACTIVE) return PortType::None;

        switch (port->signature){
            case SATA_SIG_ATAPI:
                return PortType::SATAPI;
            case SATA_SIG_ATA:
                return PortType::SATA;
            case SATA_SIG_PM:
                return PortType::PM;
            case SATA_SIG_SEMB:
                return PortType::SEMB;
            default:
                return PortType::None;
        }
    }

    void AHCI_Driver::ProbePorts(){
        uint32_t portsImplemented = ABAR->portsImplemented;
        for (int i = 0; i < 32; i++){
            if (portsImplemented & (1 << i)){
                PortType portType = CheckPortType(&ABAR->ports[i]);

                if (portType == PortType::SATA) 
                {
                    GlobalConsole.print_line("SATA drive");
                }
                else if (portType == PortType::SATAPI) 
                {
                    GlobalConsole.print_line("SATAPI drive");
                }else 
                {
                    GlobalConsole.print_line("Not interested");
                }
            }
        }
    }
}
