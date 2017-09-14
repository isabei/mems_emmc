#ifndef __CH378Q_H
#define __CH378Q_H
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* 芯片版本定义 */					
#define	DEF_CH378_VER		  	   0x42							 /* 当前芯片及固件版本,高2位为芯片版本,低6位为固件版本 */
#define	CH378_SPI_SDO_PIN( )	   GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_6 ) //获取CH378的SPI数据输出引脚电平
#define CH378_INT_WIRE     		   1	 
#define	CH378_INT_PIN_WIRE( )	   GPIOC->IDR & GPIO_Pin_5

/***********************************************************************************************************************/
/* 常用类型和常量定义 */

#ifndef		TRUE
#define		TRUE	1
#define		FALSE	0
#endif
#ifndef		NULL
#define		NULL	0
#endif

#ifndef UINT8
typedef unsigned char              UINT8;
#endif
#ifndef UINT16
typedef unsigned short             UINT16;
#endif
#ifndef UINT32
typedef unsigned long              UINT32;
#endif
#ifndef PUINT8
typedef unsigned char              *PUINT8;
#endif
#ifndef PUINT16
typedef unsigned short             *PUINT16;
#endif
#ifndef PUINT32
typedef unsigned long              *PUINT32;
#endif
#ifndef UINT8V
typedef unsigned char volatile     UINT8V;
#endif
#ifndef PUINT8V
typedef unsigned char volatile     *PUINT8V;
#endif

//#ifndef	MAX_FILE_NAME_LEN
#define MAX_PATH_LEN			128								 /* 最大路径长度,含所有斜杠分隔符和小数点间隔符以及路径结束符00H */
#define MAX_FILE_NAME_LEN		128								 /* 最大路径长度,含所有斜杠分隔符和小数点间隔符以及路径结束符00H */




#define CMD_SET_USB_MODE  0x15
#define USB_HOST 0x6
#define CMD_DISK_CONNECT 0x30
#define CMD_DISK_MOUNT 0x31
#define CMD_SET_FILE_NAME 0x2F
#define CMD_FILE_CREATE 0x34
#define CMD_WR_HOST_OFS_DATA 0x2D
#define CMD_BYTE_WRITE 0x3C
#define CMD_FILE_CLOSE 0x36
#define CMD_FILE_OPEN 0x32
#define ERR_MISS_FILE 0x42
#define CMD_BYTE_LOCATE 0x39
#define CMD_CHECK_EXIST  0x06
#define CMD_FILE_OPEN  0x32
#define CMD_SEC_LOCATE  0x4A

#define	CMD_READ_VAR8			   0x0A							 /* 读取指定的8位CH378系统变量 */
/* 输入: 变量地址 */
/* 输出: 当前地址对应的8位数据 */

#define	CMD14_READ_VAR32		   0x0C							 /* 读取指定的32位CH378系统变量 */
/* 输入: 变量地址 */
/* 输出: 当前地址对应的32位数据(总长度32位,低字节在前) */


#define	CMD_GET_STATUS		   0x22							 /* 获取中断状态并取消中断请求 */
/* 输出: 中断状态 */


#define	CMD50_WRITE_VAR32		   0x0D							 /* 设置指定的32位CH378系统变量 */
/* 输入: 变量地址, 数据(总长度32位,低字节在前) */


/* 附加的USB操作状态定义 */
#define	ERR_USB_UNKNOWN			   0xFA							 /* 未知错误,不应该发生的情况,需检查硬件或者程序错误 */

#define ERR_SUCCESS 0x00     //操作成功（返回的中断状态）
#define ERR_OPEN_DIR         //指定路径的目录被打开
#define	ERR_DISK_DISCON			   0x82							 /* 磁盘尚未连接,可能磁盘已经断开 */


/* 操作状态 */
#ifndef	CMD_RET_SUCCESS
#define	CMD_RET_SUCCESS			   0x51						  	 /* 命令操作成功 */
#define	CMD_RET_ABORT			   0x5F						     /* 命令操作失败 */
#define	CMD_PARAME_ERR			   0x5E						     /* 命令参数错误 */
#define	CMD_IDENTIFY_ERR		   0x5D						     /* 无该命令错误 */
#define	CMD_TIMEOUT_ERR		   	   0x5C						     /* 命令超时错误 */
#endif



/* ********************************************************************************************************************* */
/* 主机文件模式下的文件系统变量的地址 */

#ifndef	VAR_FILE_SIZE

/* 8位/单字节变量 */
#define	VAR8_DEV_CONNECTSTATUS	   0x02                          /* 当前设备连接状态: 0--未连接, 1--连接未初始化, 2--初始化成功 */
#define	VAR8_USB_DEV_SPEED	       0x03                          /* 当前连接的USB设备速度: 0--未知, 1--低速, 2--全速, 3--高速 */ 	
#define	VAR8_CMD_INT_STATUS		   0x0B							 /* CH378当前命令执行中断状态(针对产生中断的命令) */
#define	VAR8_LAST_CMD     		   0x0C							 /* CH378接收到的最后一个命令码 */
#define	VAR8_CMD_OP_STATUS		   0x0E							 /* CH378当前命令的执行状态 */
#define	VAR8_SDO_INT_ENABLE        0x16							 /* SPI模式是否使能SDO引脚中断通知标志(0:禁止, 1:使能) */
#define	VAR8_DISK_STATUS		   0x21							 /* 主机文件模式下的磁盘及文件状态 */
#define	VAR8_DISK_FAT			   0x22							 /* 逻辑盘的FAT标志:1=FAT12,2=FAT16,3=FAT32 */
#define	VAR8_SEC_PER_CLUS		   0x23							 /* 逻辑盘的每簇扇区数 */
#define	VAR8_DISK_SEC_LEN		   0x24							 /* 逻辑盘的每个扇区大小(一般为512字节,大扇区U盘可能为1024,2048,4096等) */
																 /* 9 = 512, 10 = 1024, 11 = 2048, 12 = 4096 */
/* 32位/4字节变量 */
#define	VAR32_DISK_CAP			   0x51							 /* 逻辑盘的总容量 */
#define	VAR32_DISK_SEC_SIZE	   	   0x52							 /* 逻辑盘的每个扇区大小(一般为512字节,大扇区U盘可能为1024,2048,4096等) */
#define	VAR32_SYSTEM_PARA	   	   0x55							 /* CH378系统参数 */
#define  RB_WORK_MODE_SET0   	   ( 1 << 0 )					 /* 位1--0: 当前芯片接口模式( 00:未知; 01:8位并口; 02:SPI接口; 03:异步串口 ) */ 	
#define  RB_WORK_MODE_SET1   	   ( 1 << 1 )
#define  RB_AUTO_CHECK_CONNECT	   ( 1 << 2 )					 /* 位2：CH378自动检测USB设备/SD卡的连接和断开使能 */
#define  RB_HOST_SD_UDISK	       ( 1 << 3 )			 		 /* 位3：主机模式：0---USB存储设备；1---SD卡 */
#define  RB_USB_CONFIG	    	   ( 1 << 4 ) 					 /* 位4：主机方式下USB设备连接标志 */

#define	VAR32_FILE_SIZE			   0x68							 /* 当前文件的长度(总长度32位,低字节在前) */
#define	VAR32_DSK_START_LBA		   0x70							 /* 逻辑盘的起始绝对扇区号LBA(总长度32位,低字节在前) */
#define	VAR32_DISK_ROOT			   0x71							 /* 对于FAT16盘为根目录占用扇区数,对于FAT32盘为根目录起始簇号(总长度32位,低字节在前) */
#define	VAR32_DSK_DAT_START		   0x72							 /* 逻辑盘的数据区域的起始LBA(总长度32位,低字节在前) */
#define	VAR32_START_CLUSTER		   0x73							 /* 当前文件或者目录(文件夹)的起始簇号(总长度32位,低字节在前) */
#define	VAR32_CURRENT_OFFSET	   0x74							 /* 当前文件指针,当前读写位置的字节偏移(总长度32位,低字节在前) */
#define	VAR_FAT_DIR_LBA			   0x75							 /* 当前文件目录信息所在的扇区LBA地址(总长度32位,低字节在前) */
#define	VAR_FAT_OFFSET			   0x76							 /* 当前文件目录信息在扇区内偏移地址(总长度32位,低字节在前) */

#endif



void CH378_Port_Init(void); 
void xWriteCH378Cmd( u8 mCmd );
void xWriteCH378Data( u8 mData );
void xEndCH378Cmd( void);
UINT8 Wait378Interrupt( void );
UINT8 CH378SendCmdWaitInt( UINT8 mCmd );
UINT8 CH378SendCmdDatWaitInt( UINT8 mCmd, UINT8 mDat );
UINT8 CH378DiskConnect( void );
UINT8 CH378DiskReady( void );
UINT8 CH378FileCreate( PUINT8 PathName );
UINT8 CH378ByteWrite( PUINT8 buf, UINT16 ReqCount);
UINT8 CH378FileClose( UINT8 UpdateSz );
void mStopIfError( UINT8 iError );
UINT8 mInitCH378Host( void ) ;
UINT8 CH378FileOpen( PUINT8 PathName );
UINT32 CH378GetFileSize( void ) ;
UINT8 CH378SecLocate( UINT32 offset );
UINT8 CH378ByteLocate( UINT32 offset );
UINT8 CH378ReadVar8( UINT8 addr ) ;
void CH378SetFileSize( UINT32 filesize );
#endif



