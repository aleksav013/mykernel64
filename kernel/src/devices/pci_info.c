#include <types.h>
#include <pci.h>
#include <pci_info.h>
#include <libk/stdio.h>

const char *get_vendor(uint64_t vendor_id)
{
	switch (vendor_id) {
	case 0x8086:
		return "Intel";
	case 0x1022:
		return "AMD";
	default:
		return NULL;
	}
}

const char *get_class(uint64_t class_code)
{
	switch (class_code) {
	case 0x0:
		return "Unclassified";
	case 0x1:
		return "Mass Storage Controller";
	case 0x2:
		return "Network Controller";
	case 0x3:
		return "Display Controller";
	case 0x4:
		return "Multimedia Controller";
	case 0x5:
		return "Memory Controller";
	case 0x6:
		return "Bridge";
	case 0x7:
		return "Simple Communication Controller";
	case 0x8:
		return "Base System Peripheral";
	case 0x9:
		return "Input Device Controller";
	case 0xa:
		return "Docking Station";
	case 0xb:
		return "Processor";
	case 0xc:
		return "Serial Bus Controller";
	case 0xd:
		return "Wireless Controller";
	case 0xe:
		return "Intelligent Controller";
	case 0xf:
		return "Satellite Communication Controller";
	case 0x10:
		return "Encryption Controller";
	case 0x11:
		return "Signal Processing Controller";
	case 0x12:
		return "Processing Accelerator";
	case 0x13:
		return "Non-Essential Instrumentation";
	case 0x14:
		return "0x3F (Reserved)";
	case 0x40:
		return "Co-Processor";
	case 0x41:
		return "0xFE (Reserved)";
	case 0xFF:
		return "Unassigned Class (Vendor specific)";
	}
	return "Unknown";
}

const char *get_subclass(uint64_t class_code, uint64_t subclass)
{
	switch (class_code) {
	case 0x0:
		switch (subclass) {
		case 0x0:
			return "Non-VGA-Compatible";
		case 0x1:
			return "VGA-Compatible";
		}
		break;
	case 0x1:
		switch (subclass) {
		case 0x0:
			return "SCSI Bus Controller";
		case 0x1:
			return "IDE Controller";
		case 0x2:
			return "IPI Bus Controller";
		case 0x3:
			return "IPI Bus Controller";
		case 0x4:
			return "RAID Controller";
		case 0x5:
			return "ATA Controller";
		case 0x6:
			return "Serial ATA Controller";
		case 0x7:
			return "Serial Attached SCSI Controller";
		case 0x8:
			return "Non-Volatile Memory Controller";
		case 0x80:
			return "Other";
		}
		break;
	case 0x2:
		switch (subclass) {
		case 0x0:
			return "Ethernet Controller";
		case 0x1:
			return "Token Ring Controller";
		case 0x2:
			return "FDDI Controller";
		case 0x3:
			return "ATM Controller";
		case 0x4:
			return "ISDN Controller";
		case 0x5:
			return "WorldFip Controller";
		case 0x6:
			return "PICMG 2.14 Multi Computing Controller";
		case 0x7:
			return "Infiniband Controller";
		case 0x8:
			return "Fabric Controller";
		case 0x80:
			return "Other";
		}
		break;
	case 0x3:
		switch (subclass) {
		case 0x0:
			return "VGA Compatible Controller";
		case 0x1:
			return "XGA Controller";
		case 0x2:
			return "3D Controller (Not VGA-Compatible)";
		case 0x80:
			return "Other";
		}
		break;
	case 0x4:
		switch (subclass) {
		case 0x0:
			return "Multimedia Video Controller";
		case 0x1:
			return "Multimedia Audio Controller";
		case 0x2:
			return "Computer Telephony Device";
		case 0x3:
			return "Audio Device";
		case 0x80:
			return "Other";
		}
		break;
	case 0x5:
		switch (subclass) {
		case 0x0:
			return "RAM Controller";
		case 0x1:
			return "Flash Controller";
		case 0x80:
			return "Other";
		}
		break;
	case 0x6:
		switch (subclass) {
		case 0x0:
			return "Host Bridge";
		case 0x1:
			return "ISA Bridge";
		case 0x2:
			return "EISA Bridge";
		case 0x3:
			return "MCA Bridge";
		case 0x4:
			return "PCI-to-PCI Bridge";
		case 0x5:
			return "PCMCIA Bridge";
		case 0x6:
			return "NuBus Bridge";
		case 0x7:
			return "CardBus Bridge";
		case 0x8:
			return "RACEway Bridge";
		case 0x9:
			return "PCI-to-PCI Bridge";
		case 0xa:
			return "InfiniBand-to-PCI Host Bridge";
		case 0x80:
			return "Other";
		}
		break;
	case 0x7:
		switch (subclass) {
		case 0x0:
			return "Serial Controller";
		case 0x1:
			return "Parallel Controller";
		case 0x2:
			return "Multiport Serial Controller";
		case 0x3:
			return "Modem";
		case 0x4:
			return "IEEE 488.1/2 (GPIB) Controller";
		case 0x5:
			return "Smart Card Controller";
		case 0x80:
			return "Other";
		}
		break;
	case 0x8:
		switch (subclass) {
		case 0x0:
			return "PIC";
		case 0x1:
			return "DMA Controller";
		case 0x2:
			return "Timer";
		case 0x3:
			return "RTC Controller";
		case 0x4:
			return "PCI Hot-Plug Controller";
		case 0x5:
			return "SD Host controller";
		case 0x6:
			return "IOMMU";
		case 0x80:
			return "Other";
		}
		break;
	case 0x9:
		switch (subclass) {
		case 0x0:
			return "Keyboard Controller";
		case 0x1:
			return "Digitizer Pen";
		case 0x2:
			return "Mouse Controller";
		case 0x3:
			return "Scanner Controller";
		case 0x4:
			return "Gameport Controller";
		case 0x80:
			return "Other";
		}
		break;
	case 0xa:
		switch (subclass) {
		case 0x0:
			return "Generic";
		case 0x80:
			return "Other";
		}
		break;
	case 0xb:
		switch (subclass) {
		case 0x0:
			return "386";
		case 0x1:
			return "486";
		case 0x2:
			return "Pentium";
		case 0x3:
			return "Pentium Pro";
		case 0x10:
			return "Alpha";
		case 0x20:
			return "PowerPC";
		case 0x30:
			return "MIPS";
		case 0x40:
			return "Co-Processor";
		case 0x80:
			return "Other";
		}
		break;
	case 0xc:
		switch (subclass) {
		case 0x0:
			return "FireWire (IEEE 1394) Controller";
		case 0x1:
			return "ACCESS Bus Controller ";
		case 0x2:
			return "SSA";
		case 0x3:
			return "USB Controller";
		case 0x4:
			return "Fibre Channel";
		case 0x5:
			return "SMBus Controller";
		case 0x6:
			return "InfiniBand Controller";
		case 0x7:
			return "IPMI Interface";
		case 0x8:
			return "SERCOS Interface (IEC 61491)";
		case 0x9:
			return "CANbus Controller";
		case 0x80:
			return "Other";
		}
		break;
	case 0xd:
		switch (subclass) {
		case 0x0:
			return "iRDA Compatible Controller";
		case 0x1:
			return "Consumer IR Controller";
		case 0x10:
			return "RF Controller";
		case 0x11:
			return "Bluetooth Controller";
		case 0x12:
			return "Broadband Controller";
		case 0x20:
			return "Ethernet Controller (802.1a)";
		case 0x21:
			return "Ethernet Controller (802.1b) ";
		case 0x80:
			return "Other";
		}
		break;
	case 0xe:
		switch (subclass) {
		case 0x0:
			return "I20";
		}
		break;
	case 0xf:
		switch (subclass) {
		case 0x1:
			return "Satellite TV Controller";
		case 0x2:
			return "Satellite Audio Controller";
		case 0x3:
			return "Satellite Voice Controller";
		case 0x4:
			return "Satellite Data Controller";
		}
		break;
	case 0x10:
		switch (subclass) {
		case 0x0:
			return "Network and Computing Encrpytion/Decryption";
		case 0x10:
			return "Entertainment Encryption/Decryption";
		case 0x80:
			return "Other";
		}
		break;
	case 0x11:
		switch (subclass) {
		case 0x0:
			return "DPIO Modules";
		case 0x1:
			return "Performance Counters";
		case 0x10:
			return "Communication Synchronizer";
		case 0x20:
			return "Signal Processing Management";
		case 0x80:
			return "Other";
		}
		break;
	}
	return NULL;
}

const char *get_progif(uint64_t class_code, uint64_t subclass, uint64_t progif)
{
	switch (class_code) {
	case 0x1:
		switch (subclass) {
		case 0x6:
			switch (progif) {
			case 0x0:
				return "Vendor Specific Interface";
			case 0x1:
				return "AHCI 1.0";
			case 0x2:
				return "Serial Storage Bus";
			}
		}
		break;
	case 0xc:
		switch (subclass) {
		case 0x3:
			switch (progif) {
			case 0x0:
				return "UHCI Controller";
			case 0x10:
				return "OHCI Controller";
			case 0x20:
				return "EHCI (USB2) Controller";
			case 0x30:
				return "XHCI (USB3) Controller";
			case 0x80:
				return "Unspecified";
			case 0xFE:
				return "USB Device";
			}
		}
		break;
	}

	return NULL;
}
