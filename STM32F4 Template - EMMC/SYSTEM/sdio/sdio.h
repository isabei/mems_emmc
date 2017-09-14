#include "stm32f4xx.h"







/**
  ******************************************************************************
  * @file    SdioEmmcDrive.c
  * @author  Luoxianhui 
  * @version V1.0.0
  * @date    12/27/2012
  * @brief   This file is Logic file.
  ******************************************************************************
	**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDIO_SD_H
#define __SDIO_SD_H

#ifdef __cplusplus
 extern "C" {
#endif
 

/** @defgroup SDIO_EMMC_Exported_Types
  * @{
  */

	 
/** 
  * @brief  SDIO specific error defines  
  */   
typedef enum
{
 
  EMMC_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
  EMMC_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
  EMMC_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
  EMMC_DATA_TIMEOUT                    = (4), /*!< Data time out */
  EMMC_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
  EMMC_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
  EMMC_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
  EMMC_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
  EMMC_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
  EMMC_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  EMMC_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
  EMMC_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
  EMMC_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
  EMMC_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  EMMC_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
  EMMC_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
  EMMC_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
  EMMC_CC_ERROR                        = (18), /*!< Internal card controller error */
  EMMC_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
  EMMC_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
  EMMC_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
  EMMC_CID_CEMMC_OVERWRITE             = (22), /*!< CID/CSD overwrite error */
  EMMC_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
  EMMC_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
  EMMC_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
  EMMC_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
  EMMC_INVALID_VOLTRANGE               = (27),
  EMMC_ADDR_OUT_OF_RANGE               = (28),
  EMMC_SWITCH_ERROR                    = (29),
  EMMC_SDIO_DISABLED                   = (30),
  EMMC_SDIO_FUNCTION_BUSY              = (31),
  EMMC_SDIO_FUNCTION_FAILED            = (32),
  EMMC_SDIO_UNKNOWN_FUNCTION           = (33),

/** 
  * @brief  Standard error defines   
  */ 
  EMMC_INTERNAL_ERROR, 
  EMMC_NOT_CONFIGURED,
  EMMC_REQUEST_PENDING, 
  EMMC_REQUEST_NOT_APPLICABLE, 
  EMMC_INVALID_PARAMETER,  
  EMMC_UNSUPPORTED_FEATURE,  
  EMMC_UNSUPPORTED_HW,  
  EMMC_ERROR,  
  EMMC_OK = 0 
} EmmcError;

/** 
  * @brief  SDIO Transfer state  
  */   
typedef enum
{
  EMMC_TRANSFER_OK  = 0,
  EMMC_TRANSFER_BUSY = 1,
  EMMC_TRANSFER_ERROR
} EmmcTransferState;

/** 
  * @brief  SD Card States 
  */   
typedef enum
{
  EMMC_CARD_READY                  = ((u32)0x00000001),
  EMMC_CARD_IDENTIFICATION         = ((u32)0x00000002),
  EMMC_CARD_STANDBY                = ((u32)0x00000003),
  EMMC_CARD_TRANSFER               = ((u32)0x00000004),
  EMMC_CARD_SENDING                = ((u32)0x00000005),
  EMMC_CARD_RECEIVING              = ((u32)0x00000006),
  EMMC_CARD_PROGRAMMING            = ((u32)0x00000007),
  EMMC_CARD_DISCONNECTED           = ((u32)0x00000008),
  EMMC_CARD_ERROR                  = ((u32)0x000000FF)
}EmmcCardState;


/** 
  * @brief  Card Specific Data: CSD Register   
  */ 
typedef struct
{
  __IO u8  CSDStruct;            /*!< CSD structure */
  __IO u8  SysSpecVersion;       /*!< System specification version */
  __IO u8  Reserved1;            /*!< Reserved */
  __IO u8  TAAC;                 /*!< Data read access-time 1 */
  __IO u8  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO u8  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO u16 CardComdClasses;      /*!< Card command classes */
  __IO u8  RdBlockLen;           /*!< Max. read data block length */
  __IO u8  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO u8  WrBlockMisalign;      /*!< Write block misalignment */
  __IO u8  RdBlockMisalign;      /*!< Read block misalignment */
  __IO u8  DSRImpl;              /*!< DSR implemented */
  __IO u8  Reserved2;            /*!< Reserved */
  __IO u16 DeviceSize;           /*!< Device Size */
  __IO u8  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO u8  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO u8  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO u8  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO u8  DeviceSizeMul;        /*!< Device size multiplier */
  __IO u8  EraseGrSize;          /*!< Erase group size */
  __IO u8  EraseGrMul;           /*!< Erase group size multiplier */
  __IO u8  WrProtectGrSize;      /*!< Write protect group size */
  __IO u8  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO u8  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO u8  WrSpeedFact;          /*!< Write speed factor */
  __IO u8  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO u8  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO u8  Reserved3;            /*!< Reserded */
  __IO u8  ContentProtectAppli;  /*!< Content protection application */
  __IO u8  FileFormatGrouop;     /*!< File format group */
  __IO u8  CopyFlag;             /*!< Copy flag (OTP) */
  __IO u8  PermWrProtect;        /*!< Permanent write protection */
  __IO u8  TempWrProtect;        /*!< Temporary write protection */
  __IO u8  FileFormat;           /*!< File Format */
  __IO u8  ECC;                  /*!< ECC code */
  __IO u8  CSD_CRC;              /*!< CSD CRC */
  __IO u8  Reserved4;            /*!< always 1*/
} EMMC_CSD;



/** 
  * @brief  Card Specific Data: EXTCSD Register   
  */ 

typedef union
{
		struct _EXT_CSD
		{
			/***************************Properties Segment****************************/
			__IO u8     Reserved5[7];      		
			__IO u8 	S_CMD_SET;	
			__IO u8 	HPI_FEATURES; 	
			__IO u8 	BKOPS_SUPPORT;	
			__IO u8 	Reserved6[255];	
			__IO u8 	BKOPS_STATUS;	
			__IO u8 	CORRECTLY_PRG_SECTORS_NUM[4]; 	
			__IO u8 	INI_TIMEOUT_PA;	
			__IO u8 	Reserved7;
			__IO u8 	PWR_CL_DDR_52_360;	
			__IO u8 	PWR_CL_DDR_52_195; 	
			__IO u8 	Reserved8[2]; 	
			__IO u8 	MIN_PERF_DDR_W_8_52; 	
			__IO u8 	MIN_PERF_DDR_R_8_52; 	
			__IO u8 	Reserved9; 	
			__IO u8 	TRIM_MULT;	
			__IO u8 	SEC_FEATURE_SUPPORT;	
			__IO u8     SEC_ERASE_MULT;		
			__IO u8 	SEC_TRIM_MULT;	
			__IO u8 	BOOT_INFO; 	
			__IO u8 	Reserved10; 	
			__IO u8     BOOT_SIZE_MULT;		
			__IO u8 	ACC_SIZE;	
			__IO u8     HC_ERASE_GRP_SIZE;		
			__IO u8 	ERASE_TIMEOUT_MULT;	
			__IO u8 	REL_WR_SEC_C; 	
			__IO u8 	HC_WP_GRP_SIZE; 	
			__IO u8 	S_C_VCC;	
			__IO u8 	S_C_VCCQ; 	
			__IO u8 	Reserved11; 	
			__IO u8 	S_A_TIMEOUT; 	
			__IO u8 	Reserved12; 	
			__IO u8 	SEC_COUNT[4]; 	
			__IO u8 	Reserved13; 	
			__IO u8 	MIN_PERF_W_8_52; 	
			__IO u8 	MIN_PERF_R_8_52; 	
			__IO u8 	MIN_PERF_W_8_26_4_52; 	
			__IO u8 	MIN_PERF_R_8_26_4_52; 	
			__IO u8 	MIN_PERF_W_4_26; 	
			__IO u8 	MIN_PERF_R_4_26; 	
			__IO u8  Reserved14;		
			__IO u8 	PWR_CL_26_360;	
			__IO u8     PWR_CL_52_360;		
			__IO u8 	PWR_CL_26_195;	
			__IO u8 	PWR_CL_52_195; 	
			__IO u8 	PARTITION_SWITCH_TIME; 	
			__IO u8     OUT_OF_INTERRUPT_TIME;		
			__IO u8  Reserved15;		
			__IO u8     CARD_TYPE; 		
			__IO u8 	Reserved16; 	
			__IO u8     CSD_STRUCTURE;		
			__IO u8  Reserved17;		
			__IO u8  EXT_CSD_REV;


          /*************************Mode Segment***************************/			
			__IO u8  CMD_SET;		
			__IO u8  Reserved18;		
			__IO u8  CMD_SET_REV;		
			__IO u8  Reserved19;		
			__IO u8  POWER_CLASS;		
			__IO u8  Reserved20;		
			__IO u8  HS_TIMING;		
			__IO u8  Reserved21;		
			__IO u8  BUS_WIDTH;		
			__IO u8  Reserved22;		
			__IO u8  ERASED_MEM_CONT ;		
			__IO u8  Reserved23;		
			__IO u8  PARTITION_CONFIG;		
			__IO u8  BOOT_CONFIG_PROT;		
			__IO u8  BOOT_BUS_WIDTH;		
			__IO u8  Reserved24; 		
			__IO u8  ERASE_GROUP_DEF;		
			__IO u8  Reserved25;		
			__IO u8  BOOT_WP; 		
			__IO u8  Reserved26; 		
			__IO u8  USER_WP; 		
			__IO u8  Reserved27; 		
			__IO u8  FW_CONFIG;		
			__IO u8  RPMB_SIZE_MULT;		
			__IO u8  WR_REL_SET; 		
			__IO u8  WR_REL_PARAM; 		
			__IO u8  Reserved28; 		
			__IO u8  BKOPS_START; 		
			__IO u8  BKOPS_EN; 		
			__IO u8  RST_n_FUNCTION;		
			__IO u8  HPI_MGMT;		
			__IO u8  PARTITIONING_SUPPORT;		
			__IO u8  MAX_ENH_SIZE_MULT[3];		
			__IO u8  PARTITIONS_ATTRIBUTE;		
			__IO u8  PARTITION_SETTING_COMPLETED; 		
			__IO u8  GP_SIZE_MULT[12]; 		
			__IO u8  ENH_SIZE_MULT[3]; 		
			__IO u8  ENH_START_ADDR[4]; 		
			__IO u8  Reserved29; 		
			__IO u8  SEC_BAD_BLK_MGMNT; 		
			__IO u8  Reserved30[134]; 		
			       
		} EXT_CSD;
   __IO u8 CsdBuf[512];
} EMMCEXT_CSD;


/** 
  * @brief  Card Identification Data: CID Register   
  */
typedef struct
{
  __IO u8  ManufacturerID;       /*!< ManufacturerID */
  __IO u8  Reserved1;            /*!< Reserved1 */
  __IO u8  CardorBGA;            /*!< Card/BGA */
  __IO u8  OEM_AppliID;          /*!< OEM/Application ID */
  __IO u32 ProdName1;            /*!< Product Name part1 */
  __IO u16 ProdName2;            /*!< Product Name part2*/
  __IO u8  ProdRev;              /*!< Product Revision */
  __IO u32 ProdSN;               /*!< Product Serial Number */
  
  __IO u16 ManufactDate;         /*!< Manufacturing Date */
  __IO u8  CID_CRC;              /*!< CID CRC */
  __IO u8  Reserved2;            /*!< always 1 */
} EMMC_CID;

/** 
  * @brief SD Card Status 
  */
typedef struct
{
  __IO u8 DAT_BUS_WIDTH;
  __IO u8 SECURED_MODE;
  __IO u16 SD_CARD_TYPE;
  __IO u32 SIZE_OF_PROTECTED_AREA;
  __IO u8 SPEED_CLASS;
  __IO u8 PERFORMANCE_MOVE;
  __IO u8 AU_SIZE;
  __IO u16 ERASE_SIZE;
  __IO u8 ERASE_TIMEOUT;
  __IO u8 ERASE_OFFSET;
} EmmcCardStatus;


/** 
  * @brief SD Card information 
  */
typedef struct
{
  EMMC_CSD EmmcCsd;
  EMMCEXT_CSD EmmcExtCsd; 
  EMMC_CID EmmcCid;
  long long  CardCapacity;  /*!< Card Capacity */
  u32 CardBlockSize; /*!< Card Block Size */
  u16 RCA;
  u8 CardType;
}EmmcCardInfo;
   
/**
  * @}
  */


/** @defgroup SDIO_EMMC_Exported_Macros
  * @{
  */
#define SDIO_FIFO_ADDRESS                ((u32)0x40012C80)
/** 
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((u8)0xFF)
/** 
  * @brief  SDIO Data Transfer Frequency (26MHz max) 
  */
#define SDIO_TRANSFER_CLK_DIV            ((u8)0x76) 


#define SD_SDIO_DMA                   DMA2
#define SD_SDIO_DMA_CLK               RCC_AHB1Periph_DMA2
 
#define EMMC_SDIO_DMA_STREAM3	            3
//#define EMMC_SDIO_DMA_STREAM6           6

#ifdef EMMC_SDIO_DMA_STREAM3
 #define EMMC_SDIO_DMA_STREAM            DMA2_Stream3
 #define EMMC_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define EMMC_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
 #define EMMC_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
 #define EMMC_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
 #define EMMC_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
 #define EMMC_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF3 
 #define EMMC_SDIO_DMA_IRQn              DMA2_Stream3_IRQn
 #define EMMC_SDIO_DMA_IRQHANDLER        DMA2_Stream3_IRQHandler 
#elif defined EMMC_SDIO_DMA_STREAM6
 #define EMMC_SDIO_DMA_STREAM            DMA2_Stream6
 #define EMMC_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define EMMC_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF6
 #define EMMC_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF6
 #define EMMC_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF6
 #define EMMC_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF6
 #define EMMC_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF6 
 #define EMMC_SDIO_DMA_IRQn              DMA2_Stream6_IRQn
 #define EMMC_SDIO_DMA_IRQHANDLER        DMA2_Stream6_IRQHandler
#endif /* EMMC_SDIO_DMA_STREAM3 */
/** 
s  * @brief SDIO Commands  Index 
  */
#define EMMC_CMD_GO_IDLE_STATE                       ((u8)0)
#define EMMC_CMD_SEND_OP_COND                        ((u8)1)
#define EMMC_CMD_ALL_SEND_CID                        ((u8)2)
#define EMMC_CMD_SET_REL_ADDR                        ((u8)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define EMMC_CMD_SET_DSR                             ((u8)4)
#define EMMC_CMD_SDIO_SLEEP_AWAKE                    ((u8)5)
#define EMMC_CMD_HS_SWITCH                           ((u8)6)
#define EMMC_CMD_SEL_DESEL_CARD                      ((u8)7)
#define EMMC_CMD_HS_SEND_EXT_CSD                     ((u8)8)
#define EMMC_CMD_SEND_CSD                            ((u8)9)
#define EMMC_CMD_SEND_CID                            ((u8)10)
#define EMMC_CMD_READ_DAT_UNTIL_STOP                 ((u8)11) /*!< SD Card doesn't support it */
#define EMMC_CMD_STOP_TRANSMISSION                   ((u8)12)
#define EMMC_CMD_SEND_STATUS                         ((u8)13)
#define EMMC_CMD_HS_BUSTEST_READ                     ((u8)14)
#define EMMC_CMD_GO_INACTIVE_STATE                   ((u8)15)
#define EMMC_CMD_SET_BLOCKLEN                        ((u8)16)
#define EMMC_CMD_READ_SINGLE_BLOCK                   ((u8)17)
#define EMMC_CMD_READ_MULT_BLOCK                     ((u8)18)
#define EMMC_CMD_HS_BUSTEST_WRITE                    ((u8)19)
#define EMMC_CMD_WRITE_DAT_UNTIL_STOP                ((u8)20) /*!< SD Card doesn't support it */
#define EMMC_CMD_SET_BLOCK_COUNT                     ((u8)23) /*!< SD Card doesn't support it */
#define EMMC_CMD_WRITE_SINGLE_BLOCK                  ((u8)24)
#define EMMC_CMD_WRITE_MULT_BLOCK                    ((u8)25)
#define EMMC_CMD_PROG_CID                            ((u8)26) /*!< reserved for manufacturers */
#define EMMC_CMD_PROG_CSD                            ((u8)27)
#define EMMC_CMD_SET_WRITE_PROT                      ((u8)28)
#define EMMC_CMD_CLR_WRITE_PROT                      ((u8)29)
#define EMMC_CMD_SEND_WRITE_PROT                     ((u8)30)
#define EMMC_CMD_SEND_WRITE_PROT_TYPE                ((u8)31)
#define EMMC_CMD_ERASE_GRP_START                     ((u8)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define EMMC_CMD_ERASE_GRP_END                       ((u8)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define EMMC_CMD_ERASE                               ((u8)38)
#define EMMC_CMD_FAST_IO                             ((u8)39) /*!< SD Card doesn't support it */
#define EMMC_CMD_GO_IRQ_STATE                        ((u8)40) /*!< SD Card doesn't support it */
#define EMMC_CMD_LOCK_UNLOCK                         ((u8)42)
#define EMMC_CMD_APP_CMD                             ((u8)55)
#define EMMC_CMD_GEN_CMD                             ((u8)56)
#define EMMC_CMD_NO_CMD                              ((u8)64)



  
/* Uncomment the following line to select the SDIO Data transfer mode */  
#if !defined (SD_DMA_MODE) && !defined (SD_POLLING_MODE)
#define SD_DMA_MODE                                ((u32)0x00000000)    //DMA????
#define SD_POLLING_MODE                            ((u32)0x00000002)    //??????
#endif

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT                                 ((u8)0x01)
#define SD_NOT_PRESENT                             ((u8)0x00)

/** 
  * @brief Supported SD Memory Cards 
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((u32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((u32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((u32)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((u32)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((u32)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((u32)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((u32)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((u32)0x00000007)

  
/** 
  * @brief  SDIO Static flags, TimeOut, FIFO Address  
  */
#define NULL 0
#define SDIO_STATIC_FLAGS               ((u32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((u32)0x01000000)

/** 
  * @brief  Mask for errors Card Status R1 (OCR Register) 
  */
#define EMMC_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define EMMC_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define EMMC_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define EMMC_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define EMMC_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define EMMC_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define EMMC_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define EMMC_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define EMMC_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define EMMC_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define EMMC_OCR_CC_ERROR                 ((u32)0x00100000)
#define EMMC_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define EMMC_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define EMMC_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define EMMC_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define HOCR_WP_ERASE_SKIP                ((u32)0x00008000)
#define EMMC_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define EMMC_OCR_ERASE_RESET              ((u32)0x00002000)
#define EMMC_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define EMMC_OCR_ERRORBITS                ((u32)0xFDFFE008)

/** 
  * @brief  Masks for R6 Response 
  */
#define EMMC_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define EMMC_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define EMMC_R6_COM_CRC_FAILED            ((u32)0x00008000)

#define EMMC_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define EMMC_HIGH_CAPACITY                ((u32)0x40000000)
#define EMMC_STD_CAPACITY                 ((u32)0x00000000)
#define EMMC_CHECK_PATTERN                ((u32)0x000001AA)

#define EMMC_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define EMMC_ALLZERO                      ((u32)0x00000000)

#define EMMC_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define EMMC_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define EMMC_CARD_LOCKED                  ((u32)0x02000000)

#define EMMC_DATATIMEOUT                  ((u32)0xFFFFFFFF)
#define EMMC_0TO7BITS                     ((u32)0x000000FF)
#define EMMC_8TO15BITS                    ((u32)0x0000FF00)
#define EMMC_16TO23BITS                   ((u32)0x00FF0000)
#define EMMC_24TO31BITS                   ((u32)0xFF000000)
#define EMMC_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define EMMC_HALFFIFO                     ((u32)0x00000008)
#define EMMC_HALFFIFOBYTES                ((u32)0x00000020)

/** 
  * @brief  Command Class Supported 
  */
#define EMMC_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define EMMC_CCCC_WRITE_PROT              ((u32)0x00000040)
#define EMMC_CCCC_ERASE                   ((u32)0x00000020)


/**
  * @}
  */ 
 
#define EMMC_OCR_REG             0x00FF8080 




#define EMMC_POWER_REG           0x03BB0800
#define EMMC_HIGHSPEED_REG       0x03B90100
#define EMMC_4BIT_REG            0x03B70100
#define EMMC_8BIT_REG            0x03B70200
#define EMMC_4BIT_DUAL           0x03B70500
#define EMMC_8BIT_DUAL           0x03B70600

/** @defgroup SDIO_EMMC_Exported_Functions
  * @{
  */ 
EmmcError EmmcInit(void);
EmmcError CmdError(void);
EmmcError CmdResp1Error(u8 cmd);
EmmcError CmdResp7Error(void);
EmmcError CmdResp3Error(void);
EmmcError CmdResp2Error(void);
EmmcError CmdResp6Error(u8 cmd, u16 *prca);
EmmcError EmmcEnWideBus(FunctionalState NewState);
EmmcError IsCardProgramming(u8 *pstatus);
EmmcError EmmcPowerON(void);
EmmcError EmmcInitializeCards(EmmcCardInfo *E);
EmmcError EmmcSendStatus(u32 *pcardstatus);
EmmcError EmmcStopTransfer(void);
EmmcError EmmcGetCardInfo(EmmcCardInfo *E, u32 *CSD_Tab, u32 *CID_Tab, u16 Rca);
EmmcError EmmcSelectDeselect(u32 addr);
EmmcError EmmcProcessIRQSrc(void);
EmmcError EmmcReadExtCsd(EmmcCardInfo *E);
EmmcError EmmcEnableWideBusOperation(u32 WideMode);
EmmcError EmmcReadBlock(u8 *readbuff, u32 ReadAddr, u16 BlockSize);
EmmcError EmmcReadMultiBlocks(u8 *readbuff, u32 ReadAddr, u16 BlockSize, u32 NumberOfBlocks);
EmmcError EmmcWriteBlock(u8 *writebuff, u32 WriteAddr, u16 BlockSize);
EmmcError EmmcWriteMultiBlocks(u8 *writebuff, u32 WriteAddr, u16 BlockSize, u32 NumberOfBlocks);
EmmcError EmmcErase(u32 startaddr, u32 endaddr);
EmmcCardState EmmcGetState(void);
EmmcError EmmcWaitReadOperation(void);
EmmcError EmmcWaitWriteOperation(void);
void EmmcProcessDMAIRQ(void);
//void EmmcLowLevelDMATxConfig(u32 *BufferSRC, u32 BufferSize);
//void EmmcLowLevelDMARxConfig(u32 *BufferDST, u32 BufferSize);
u8 convert_from_bytes_to_power_of_two(u16 NumberOfBytes);

extern  EmmcCardInfo MyEmmcCardInfo;
void CmdOD2PP(void);
EmmcError EmmcBusTest(void);
/**
  * @}
  */ 
#ifdef __cplusplus
}
#endif

#endif /* __STM324xG_EVAL_SDIO_SD_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/




