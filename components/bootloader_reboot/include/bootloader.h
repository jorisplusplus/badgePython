#ifndef MODBOOTLOADER_H
#define MODBOOTLOADER_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

__BEGIN_DECLS

typedef struct __attribute__ ((packed))
{
  uint32_t op_mode                  : 3; // 0: HNP and SRP | 1: SRP | 2: non-HNP, non-SRP
  uint32_t arch                     : 2; // 0: slave-only | 1: External DMA | 2: Internal DMA | 3: others
  uint32_t point2point              : 1; // 0: support hub and split | 1: no hub, no split
  uint32_t hs_phy_type              : 2; // 0: not supported | 1: UTMI+ | 2: ULPI | 3: UTMI+ and ULPI
  uint32_t fs_phy_type              : 2; // 0: not supported | 1: dedicated | 2: UTMI+ | 3: ULPI
  uint32_t num_dev_ep               : 4; // Number of device endpoints (not including EP0)
  uint32_t num_host_ch              : 4; // Number of host channel
  uint32_t period_channel_support   : 1; // Support Periodic OUT Host Channel
  uint32_t enable_dynamic_fifo      : 1; // Dynamic FIFO Sizing Enabled
  uint32_t mul_cpu_int              : 1; // Multi-Processor Interrupt Enabled
  uint32_t reserved21               : 1;
  uint32_t nperiod_tx_q_depth       : 2; // Non-periodic request queue depth: 0 = 2.  1 = 4, 2 = 8
  uint32_t host_period_tx_q_depth   : 2; // Host periodic request queue depth: 0 = 2.  1 = 4, 2 = 8
  uint32_t dev_token_q_depth        : 5; // Device IN token sequence learning queue depth: 0-30
  uint32_t otg_enable_ic_usb        : 1; // IC_USB mode specified for mode of operation
} dwc2_ghwcfg2_t;


typedef struct __attribute__ ((packed))
{
  uint32_t xfer_size_width          : 4;  // Transfer size counter in bits = 11 + n (max 19 bits)
  uint32_t packet_size_width        : 3;  // Packet size counter in bits = 4 + n (max 10 bits)
  uint32_t otg_enable               : 1;  // 1 is OTG capable
  uint32_t i2c_enable               : 1;  // I2C interface is available
  uint32_t vendor_ctrl_itf          : 1;  // Vendor control interface is available
  uint32_t optional_feature_removed : 1;  // remove User ID, GPIO, SOF toggle & counter
  uint32_t synch_reset              : 1;  // 0: async reset | 1: synch reset
  uint32_t otg_adp_support          : 1;  // ADP logic is present along with HSOTG controller
  uint32_t otg_enable_hsic          : 1;  // 1: HSIC-capable with shared UTMI PHY interface | 0: non-HSIC
  uint32_t battery_charger_support  : 1;  // support battery charger
  uint32_t lpm_mode                 : 1;  // LPC mode
  uint32_t total_fifo_size          : 16; // DFIFO depth value in terms of 32-bit words
}dwc2_ghwcfg3_t;


typedef struct __attribute__ ((packed))
{
  uint32_t num_dev_period_in_ep       : 4; // Number of Device Periodic IN Endpoints
  uint32_t power_optimized            : 1; // Partial Power Down Enabled
  uint32_t ahb_freq_min               : 1; // 1: minimum of AHB frequency is less than 60 MHz
  uint32_t hibernation                : 1; // Hibernation feature is enabled
  uint32_t reserved7                  : 3;
  uint32_t service_interval_mode      : 1; // Service Interval supported
  uint32_t ipg_isoc_en                : 1; // IPG ISOC supported
  uint32_t acg_enable                 : 1; // ACG enabled
  uint32_t reserved13                 : 1;
  uint32_t utmi_phy_data_width        : 2; // 0: 8 bits | 1: 16 bits | 2: 8/16 software selectable
  uint32_t dev_ctrl_ep_num            : 4; // Number of Device control endpoints in addition to EP0
  uint32_t iddg_filter_enabled        : 1;
  uint32_t vbus_valid_filter_enabled  : 1;
  uint32_t a_valid_filter_enabled     : 1;
  uint32_t b_valid_filter_enabled     : 1;
  uint32_t dedicated_fifos            : 1; // Dedicated tx fifo for device IN Endpoint is enabled
  uint32_t num_dev_in_eps             : 4; // Number of Device IN Endpoints including EP0
  uint32_t dma_desc_enable            : 1; // scatter/gather DMA configuration
  uint32_t dma_dynamic                : 1; // Dynamic scatter/gather DMA
}dwc2_ghwcfg4_t;

// Host Channel
typedef struct
{
  volatile uint32_t hcchar;           // 500 + 20*ch Host Channel Characteristics
  volatile uint32_t hcsplt;           // 504 + 20*ch Host Channel Split Control
  volatile uint32_t hcint;            // 508 + 20*ch Host Channel Interrupt
  volatile uint32_t hcintmsk;         // 50C + 20*ch Host Channel Interrupt Mask
  volatile uint32_t hctsiz;           // 510 + 20*ch Host Channel Transfer Size
  volatile uint32_t hcdma;            // 514 + 20*ch Host Channel DMA Address
  uint32_t reserved518;      // 518 + 20*ch
  volatile uint32_t hcdmab;           // 51C + 20*ch Host Channel DMA Address
} dwc2_channel_t;

// Endpoint IN
typedef struct
{
  volatile uint32_t diepctl;          // 900 + 20*ep Device IN Endpoint Control
  uint32_t reserved04;       // 904
  volatile uint32_t diepint;          // 908 + 20*ep Device IN Endpoint Interrupt
  uint32_t reserved0c;       // 90C
  volatile uint32_t dieptsiz;         // 910 + 20*ep Device IN Endpoint Transfer Size
  volatile uint32_t diepdma;          // 914 + 20*ep Device IN Endpoint DMA Address
  volatile uint32_t dtxfsts;          // 918 + 20*ep Device IN Endpoint Tx FIFO Status
  uint32_t reserved1c;       // 91C
} dwc2_epin_t;

// Endpoint OUT
typedef struct
{
  volatile uint32_t doepctl;          // B00 + 20*ep Device OUT Endpoint Control
  uint32_t reserved04;       // B04
  volatile uint32_t doepint;          // B08 + 20*ep Device OUT Endpoint Interrupt
  uint32_t reserved0c;       // B0C
  volatile uint32_t doeptsiz;         // B10 + 20*ep Device OUT Endpoint Transfer Size
  volatile uint32_t doepdma;          // B14 + 20*ep Device OUT Endpoint DMA Address
  uint32_t reserved18[2];    // B18..B1C
} dwc2_epout_t;

typedef struct
{
  //------------- Core Global -------------//
  volatile uint32_t gotgctl;          // 000 OTG Control and Status
  volatile uint32_t gotgint;          // 004 OTG Interrupt
  volatile uint32_t gahbcfg;          // 008 AHB Configuration
  volatile uint32_t gusbcfg;          // 00c USB Configuration
  volatile uint32_t grstctl;          // 010 Reset
  volatile uint32_t gintsts;          // 014 Interrupt
  volatile uint32_t gintmsk;          // 018 Interrupt Mask
  volatile uint32_t grxstsr;          // 01c Receive Status Debug Read
  volatile uint32_t grxstsp;          // 020 Receive Status Read/Pop
  volatile uint32_t grxfsiz;          // 024 Receive FIFO Size
  union {
    volatile uint32_t dieptxf0;         // 028 EP0 Tx FIFO Size
    volatile uint32_t gnptxfsiz;        // 028 Non-periodic Transmit FIFO Size
  };
  volatile uint32_t gnptxsts;         // 02c Non-periodic Transmit FIFO/Queue Status
  volatile uint32_t gi2cctl;          // 030 I2C Address
  volatile uint32_t gpvndctl;         // 034 PHY Vendor Control
  union {
    volatile uint32_t ggpio;            // 038 General Purpose IO
    volatile uint32_t stm32_gccfg;      // 038 STM32 General Core Configuration
  };
  volatile uint32_t guid;             // 03C User (Application programmable) ID
  volatile uint32_t gsnpsid;          // 040 Synopsys ID + Release version
  volatile uint32_t ghwcfg1;          // 044 User Hardware Configuration1: endpoint dir (2 bit per ep)
  union {
    volatile uint32_t ghwcfg2;          // 048 User Hardware Configuration2
    dwc2_ghwcfg2_t    ghwcfg2_bm;
  };
  union {
    volatile uint32_t ghwcfg3;          // 04C User Hardware Configuration3
    dwc2_ghwcfg3_t    ghwcfg3_bm;
  };
  union {
    volatile uint32_t ghwcfg4;          // 050 User Hardware Configuration4
    dwc2_ghwcfg4_t    ghwcfg4_bm;
  };
  volatile uint32_t glpmcfg;          // 054 Core LPM Configuration
  volatile uint32_t gpwrdn;           // 058 Power Down
  volatile uint32_t gdfifocfg;        // 05C DFIFO Software Configuration
  volatile uint32_t gadpctl;          // 060 ADP Timer, Control and Status
  uint32_t reserved64[39];   // 064..0FF
  volatile uint32_t hptxfsiz;         // 100 Host Periodic Tx FIFO Size
  volatile uint32_t dieptxf[15];      // 104..13C Device Periodic Transmit FIFO Size
  uint32_t reserved140[176]; // 140..3FF

  //------------- Host -------------//
  volatile uint32_t hcfg;             // 400 Host Configuration
  volatile uint32_t hfir;             // 404 Host Frame Interval
  volatile uint32_t hfnum;            // 408 Host Frame Number / Frame Remaining
  uint32_t reserved40c;      // 40C
  volatile uint32_t hptxsts;          // 410 Host Periodic TX FIFO / Queue Status
  volatile uint32_t haint;            // 414 Host All Channels Interrupt
  volatile uint32_t haintmsk;         // 418 Host All Channels Interrupt Mask
  volatile uint32_t hflbaddr;         // 41C Host Frame List Base Address
  uint32_t reserved420[8];   // 420..43F
  volatile uint32_t hprt;             // 440 Host Port Control and Status
  uint32_t reserved444[47];  // 444..4FF

  //------------- Host Channel -------------//
  dwc2_channel_t    channel[16];      // 500..6FF Host Channels 0-15
  uint32_t reserved700[64];  // 700..7FF

  //------------- Device -------------//
  volatile uint32_t dcfg;             // 800 Device Configuration
  volatile uint32_t dctl;             // 804 Device Control
  volatile uint32_t dsts;             // 808 Device Status (RO)
  uint32_t reserved80c;      // 80C
  volatile uint32_t diepmsk;          // 810 Device IN Endpoint Interrupt Mask
  volatile uint32_t doepmsk;          // 814 Device OUT Endpoint Interrupt Mask
  volatile uint32_t daint;            // 818 Device All Endpoints Interrupt
  volatile uint32_t daintmsk;         // 81C Device All Endpoints Interrupt Mask
  volatile uint32_t dtknqr1;          // 820 Device IN token sequence learning queue read1
  volatile uint32_t dtknqr2;          // 824 Device IN token sequence learning queue read2
  volatile uint32_t dvbusdis;         // 828 Device VBUS Discharge Time
  volatile uint32_t dvbuspulse;       // 82C Device VBUS Pulsing Time
  volatile uint32_t dthrctl;          // 830 Device threshold Control
  volatile uint32_t diepempmsk;       // 834 Device IN Endpoint FIFO Empty Interrupt Mask
  volatile uint32_t deachint;         // 838 Device Each Endpoint Interrupt
  volatile uint32_t deachmsk;         // 83C Device Each Endpoint Interrupt msk
  volatile uint32_t diepeachmsk[16];  // 840..87C Device Each IN Endpoint mask
  volatile uint32_t doepeachmsk[16];  // 880..8BF Device Each OUT Endpoint mask
  uint32_t reserved8c0[16];  // 8C0..8FF

  //------------- Device Endpoint -------------//
  dwc2_epin_t       epin[16];         // 900..AFF  IN Endpoints
  dwc2_epout_t      epout[16];        // B00..CFF  OUT Endpoints
  uint32_t reservedd00[64];  // D00..DFF

  //------------- Power Clock -------------//
  volatile uint32_t pcgctl;           // E00 Power and Clock Gating Control
  volatile uint32_t pcgctl1;          // E04
  uint32_t reservede08[126]; // E08..FFF

  //------------- FIFOs -------------//
  // Word-accessed only using first pointer since it auto shift
  volatile uint32_t fifo[16][0x400];  // 1000..FFFF Endpoint FIFO
} dwc2_regs_t;

extern void reboot_into_bootloader(bool dfu);
extern esp_err_t bootloader_init(void);

__END_DECLS

#endif // MODBOOTLOADER_H
