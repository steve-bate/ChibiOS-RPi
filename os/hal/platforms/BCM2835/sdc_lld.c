
/* ChibiOS SDC low-level driver for BCM2835 eMMC peripheral
 *
 * Rob Reilink, 2017
 *
 * References:
 *  https://github.com/raspberrypi/linux/blob/rpi-4.9.y/drivers/mmc/host/sdhci.h
 *  https://github.com/jncronin/rpi-boot/blob/master/emmc.c
 *  Broadcom BCM2835 Peripherals Guide
 */


#include "ch.h"
#include "hal.h"
#include "sdc.h"
#include "sdc_lld.h"
#include "chtypes.h"
#include "bcm2835.h"
#include "bcmmailbox.h"

#include <stdio.h>

SDCDriver SDCD1;

#define EMMC_ARG2           REG(0x20300000)
#define EMMC_BLKSIZECNT     REG(0x20300004)
#define EMMC_ARG1           REG(0x20300008)
#define EMMC_CMDTM          REG(0x2030000C)
#define EMMC_RESP0          REG(0x20300010)
#define EMMC_RESP1          REG(0x20300014)
#define EMMC_RESP2          REG(0x20300018)
#define EMMC_RESP3          REG(0x2030001C)
#define EMMC_DATA           REG(0x20300020)
#define EMMC_STATUS         REG(0x20300024)
#define EMMC_CONTROL0       REG(0x20300028)
#define EMMC_CONTROL1       REG(0x2030002C)
#define EMMC_INTERRUPT      REG(0x20300030)
#define EMMC_IRPT_MASK      REG(0x20300034)
#define EMMC_IRPT_EN	        REG(0x20300038)
#define EMMC_CONTROL2       REG(0x2030003C)
#define EMMC_FORCE_IRPT     REG(0x20300050)
#define EMMC_BOOT_TIMEOUT   REG(0x20300070)
#define EMMC_DBG_SEL        REG(0x20300074)
#define EMMC_EXRDFIFO_CFG   REG(0x20300080)
#define EMMC_EXRDFIFO_EN    REG(0x20300084)
#define EMMC_TUNE_STEP      REG(0x20300088)
#define EMMC_TUNE_STEPS_STD REG(0x2030008C)
#define EMMC_TUNE_STEPS_DDR REG(0x20300090)
#define EMMC_SPI_INT_SPT    REG(0x203000F0)
#define EMMC_SLOTISR_VER    REG(0x203000FC)

#define INTERRUPT_ERR               BIT(15)
#define INTERRUPT_READ_RDY          BIT(5)
#define INTERRUPT_WRITE_RDY         BIT(4)
#define INTERRUPT_DATA_DONE         BIT(1)
#define INTERRUPT_CMD_DONE          BIT(0)
#define INTERRUPT_ALL               0x017ff137 // all interrupts which are defined
#define INTERRUPT_ALLERRORS         0x017f8000 // all error interrupts which are defined (including INTERRUPT_ERR)

#define CONTROL0_HCTL_8BIT          BIT(5)
#define CONTROL0_HCTL_HS_EN         BIT(2)
#define CONTROL0_HCTL_DWIDTH        BIT(1)

#define CONTROL1_SRST_DATA          BIT(26)
#define CONTROL1_SRST_CMD           BIT(25)
#define CONTROL1_SRST_HC            BIT(24)

#define CONTROL1_DATA_TOUNIT(x)     (((x)&0x0f)<<16)
#define CONTROL1_CLK_FREQ_8(x)      (((x)&0x03)<<8)  //divider bits 7..0
#define CONTROL1_CLK_FREQ_MS2(x)    (((x)&0xff)<<6)  //divider bits 9..8
#define CONTROL1_CLK_GENSEL         BIT(5)
#define CONTROL1_CLK_EN             BIT(2) // clock to card enable
#define CONTROL1_CLK_STABLE         BIT(1)
#define CONTROL1_CLK_INTLEN         BIT(0) // internal clock enable

#define STATUS_DAT_INHIBIT          BIT(1)
#define STATUS_CMD_INHIBIT          BIT(0)

#define CMDTM_CMD_ISDATA            BIT(21)
#define CMDTM_CMD_CRCCHK_EN         BIT(19)
#define CMDTM_CMD_RSPNS_TYPE        (3<<16)
#define CMDTM_CMD_RSPNS_TYPE_NONE   (0<<16)
#define CMDTM_CMD_RSPNS_TYPE_136    (1<<16)
#define CMDTM_CMD_RSPNS_TYPE_48     (2<<16)
#define CMDTM_CMD_RSPNS_TYPE_48b    (3<<16)
#define CMDTM_TM_MULTIBLOCK         BIT(5)
#define CMDTM_TM_DAT_DIR            BIT(4)
#define CMDTM_TM_AUTO_CMD_EN_CMD12  (1<<2)
#define CMDTM_TM_AUTO_CMD_EN_CMD23  (2<<2)
#define CMDTM_TM_BLKCNT_EN          BIT(1)



#define SDC_START_CLOCKRATE 400000
#define SDC_DATA_CLOCKRATE 25000000

static int switch_clockrate(int rate, int baseclock) {
    int divider;
    
    divider = -(-baseclock / rate); // 2x - causes round-up instead of round-down
    if (divider == 0) divider = 1;
    if (divider > 0x3ff) divider = 0x3ff;

    // Wait for transfer complete
    while (EMMC_STATUS & (STATUS_DAT_INHIBIT | STATUS_CMD_INHIBIT));
    
    EMMC_CONTROL1 &=~ CONTROL1_CLK_EN; // disable clock to SD card

    // divider must be multiple of 2 according to some sources (e.g.
    // comments in the Linux kernel); round up
    divider += divider & 1;

    // divider bits 9:8 go into 7:6; bits 7:0 go into 15:8
    EMMC_CONTROL1 = (EMMC_CONTROL1 & ~0xffe0) | 
        CONTROL1_CLK_FREQ_8(divider & 0xff) | 
        CONTROL1_CLK_FREQ_MS2((divider & 0x300) >> 8);
    
    while(!(EMMC_CONTROL1 & CONTROL1_CLK_STABLE)) ; // wait to stabilize
    
    EMMC_CONTROL1 |= CONTROL1_CLK_EN; // enable clock to SD card

    return baseclock / divider;
}

void sdc_lld_init(void) {
    sdcObjectInit(&SDCD1);
}


void sdc_lld_start(SDCDriver *sdcp) {
    uint32_t tag[2];
    
    DataMemBarrier();
    
    // Switch GPIO48-53 to ALT3 (SD card). Note: this alternate function is not
    // documented in the BCM2835 ARM peripherals manual
    
    for(int i=48;i<=53;i++) {
        bcm2835_gpio_fnsel(i, GPFN_ALT3);
    }
    
    DataMemBarrier();

    // Switch on power to SD (probably already on from boot, but just to be sure)
    tag[0] = 0; //device: SD card
    tag[1] = 3; // power on, wait
    if (PiPyOS_bcm_get_set_property_tag(0x28001, tag, 8) != 8) {
        printf("sdc_lld_start: error powering on eMMC");
        return;
    }

    // Get base clock rate (required to calculate clock divider)
    tag[0] = 1; //device: SD card
    if (PiPyOS_bcm_get_set_property_tag(0x30002, tag, 8) != 8) {
        printf("sdc_lld_start: error querying eMMC clock rate");
        return;    
    }
    sdcp->emmc_base_clock_rate = tag[1];

    // reset host controller; disable clocks
    EMMC_CONTROL1 = CONTROL1_SRST_HC; 
    while(EMMC_CONTROL1 & (CONTROL1_SRST_HC | CONTROL1_SRST_CMD | CONTROL1_SRST_DATA)); //wait while any reset bit is high
    
    EMMC_CONTROL2 = 0; // reset tuning
    
    //enable internal clock, set clock rate and enable command timeout
    EMMC_CONTROL1 = CONTROL1_CLK_INTLEN;
    //switch_clockrate(SDC_CLOCKRATE, emmc_clock_rate);
    EMMC_CONTROL1 |= CONTROL1_DATA_TOUNIT(7); // timeout = 2^(7+13) cycles
    
    EMMC_IRPT_EN = 0; // disable interrupts to ARM
    EMMC_IRPT_MASK = INTERRUPT_ALL; //unmask all interrupts
    EMMC_INTERRUPT = 0xffffffff; // reset all interrupts
 
    DataMemBarrier();  
}


void sdc_lld_stop(SDCDriver *sdcp) {

}


void sdc_lld_start_clk(SDCDriver *sdcp) {
    switch_clockrate(SDC_START_CLOCKRATE, sdcp->emmc_base_clock_rate);
}

void sdc_lld_set_data_clk(SDCDriver *sdcp) {
    switch_clockrate(SDC_DATA_CLOCKRATE, sdcp->emmc_base_clock_rate);
}

void sdc_lld_stop_clk(SDCDriver *sdcp) {

}

void sdc_lld_set_bus_mode(SDCDriver *sdcp, sdcbusmode_t mode) {
  
    switch (mode) {
    case SDC_MODE_1BIT:
        EMMC_CONTROL0 = EMMC_CONTROL0 & ~(CONTROL0_HCTL_8BIT | CONTROL0_HCTL_DWIDTH);
        break;
    case SDC_MODE_4BIT:
        EMMC_CONTROL0 = (EMMC_CONTROL0 & ~CONTROL0_HCTL_8BIT) | CONTROL0_HCTL_DWIDTH;
        break;
    case SDC_MODE_8BIT:
        EMMC_CONTROL0 = (EMMC_CONTROL0 & ~CONTROL0_HCTL_DWIDTH) | CONTROL0_HCTL_8BIT;
        break;
    }
    

}

/* Common code for all sdc_llc_send_cmd_xxx functions
 *
 * Returns: CH_SUCCESS on success; CH_FAILED value on error
 */
static uint32_t sdc_lld_send_cmd_x(uint8_t cmd, uint32_t flags, uint32_t arg, uint32_t *resp) {
    uint32_t emmc_interrupt;

    DataMemBarrier();

    EMMC_ARG1 = arg;
    
    EMMC_CMDTM = flags | (cmd<<24);
    
    EMMC_INTERRUPT = INTERRUPT_ALL;
    
    // Wait for completion or for error
    // todo: interrupt-based using ChibiOS
    //printf("SDC CMD %d ARG %ld...", cmd, arg);
    
    while(!((emmc_interrupt = EMMC_INTERRUPT) & (INTERRUPT_ERR | INTERRUPT_CMD_DONE)));
    
    if ((emmc_interrupt & (INTERRUPT_ERR | INTERRUPT_CMD_DONE)) != INTERRUPT_CMD_DONE) {
        DataMemBarrier();
        //printf("SDC COMMAND %d FAILED\n", cmd);
        return CH_FAILED; //ERR is clear or INTERRUPT_CMD_DONE is not set --> failiure
    }
    
    flags = flags & CMDTM_CMD_RSPNS_TYPE;
    
    // Read RESPx and store in *resp, depending on response size
    if (flags != CMDTM_CMD_RSPNS_TYPE_NONE) {
        *resp++ = EMMC_RESP0;
    }
    
    if (flags == CMDTM_CMD_RSPNS_TYPE_136) {
        *resp++ = EMMC_RESP1;
        *resp++ = EMMC_RESP2;
        *resp++ = EMMC_RESP3;
    }

    DataMemBarrier();
    return CH_SUCCESS;
}


void sdc_lld_send_cmd_none(SDCDriver *sdcp, uint8_t cmd, uint32_t arg) {
    sdc_lld_send_cmd_x(cmd, CMDTM_CMD_RSPNS_TYPE_NONE, arg, NULL);
}

bool_t sdc_lld_send_cmd_short(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                            uint32_t *resp)
{
    return sdc_lld_send_cmd_x(cmd, CMDTM_CMD_RSPNS_TYPE_48, arg, resp);                      
}

bool_t sdc_lld_send_cmd_short_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                uint32_t *resp)
{
    return sdc_lld_send_cmd_x(cmd, CMDTM_CMD_RSPNS_TYPE_48 | CMDTM_CMD_CRCCHK_EN, arg, resp);                               
}
bool_t sdc_lld_send_cmd_long_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                uint32_t *resp)
{
    return sdc_lld_send_cmd_x(cmd, CMDTM_CMD_RSPNS_TYPE_136 | CMDTM_CMD_CRCCHK_EN, arg, resp);
}

bool_t sdc_lld_read(SDCDriver *sdcp, uint32_t startblk,
                    uint8_t *buf, uint32_t n)
{
    uint32_t resp[1];
    uint32_t emmc_interrupt;
    uint32_t data;
    
    DataMemBarrier();
    

    // TODO: also implement multiple-block transfers
    while(n) {
        EMMC_INTERRUPT = INTERRUPT_ALL;
        
        EMMC_BLKSIZECNT = (1<<16) | 512; // 1 block of 512 bytes
        
        if (sdc_lld_send_cmd_x(MMCSD_CMD_READ_SINGLE_BLOCK, CMDTM_CMD_RSPNS_TYPE_48 | CMDTM_CMD_CRCCHK_EN | CMDTM_CMD_ISDATA | CMDTM_TM_DAT_DIR,
                                    startblk, resp) || MMCSD_R1_ERROR(resp[0])) {
            printf("Failed\n");
    
            return CH_FAILED;
        }

        // Wait until READ_RDY or ERROR
        while(!((emmc_interrupt = EMMC_INTERRUPT) & (INTERRUPT_ERR | INTERRUPT_READ_RDY)));
        if ((emmc_interrupt & (INTERRUPT_ERR | INTERRUPT_READ_RDY)) != INTERRUPT_READ_RDY) {
            DataMemBarrier();
            printf("SDC COMMAND READ FAILED\n");
            return CH_FAILED; //ERR is clear or INTERRUPT_READ_RDY is not set --> failiure
        }

        for(int i=0;i<128;i++) {
            data = EMMC_DATA;
            *buf++ = data & 0xff;
            *buf++ = (data>>8) & 0xff;
            *buf++ = (data>>16) & 0xff;
            *buf++ = (data>>24) & 0xff;
        }
        
        startblk++;
        n--;
        
    };
    DataMemBarrier();
    return CH_SUCCESS;
}

bool_t sdc_lld_write(SDCDriver *sdcp, uint32_t startblk,
                    const uint8_t *buf, uint32_t n)
{
    printf("SDC WRITE not implemented\n");

    return CH_FAILED;
}

bool_t sdc_lld_sync(SDCDriver *sdcp) {
    printf("SDC SYNC not implemented\n");
    
    return CH_FAILED;
}

bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) {
    return 1;
}

bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) {
    return 1;
}

