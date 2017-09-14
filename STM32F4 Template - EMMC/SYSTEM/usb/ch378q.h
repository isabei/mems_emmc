#ifndef __CH378Q_H
#define __CH378Q_H
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* оƬ�汾���� */					
#define	DEF_CH378_VER		  	   0x42							 /* ��ǰоƬ���̼��汾,��2λΪоƬ�汾,��6λΪ�̼��汾 */
#define	CH378_SPI_SDO_PIN( )	   GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_6 ) //��ȡCH378��SPI����������ŵ�ƽ
#define CH378_INT_WIRE     		   1	 
#define	CH378_INT_PIN_WIRE( )	   GPIOC->IDR & GPIO_Pin_5

/***********************************************************************************************************************/
/* �������ͺͳ������� */

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
#define MAX_PATH_LEN			128								 /* ���·������,������б�ָܷ�����С���������Լ�·��������00H */
#define MAX_FILE_NAME_LEN		128								 /* ���·������,������б�ָܷ�����С���������Լ�·��������00H */




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

#define	CMD_READ_VAR8			   0x0A							 /* ��ȡָ����8λCH378ϵͳ���� */
/* ����: ������ַ */
/* ���: ��ǰ��ַ��Ӧ��8λ���� */

#define	CMD14_READ_VAR32		   0x0C							 /* ��ȡָ����32λCH378ϵͳ���� */
/* ����: ������ַ */
/* ���: ��ǰ��ַ��Ӧ��32λ����(�ܳ���32λ,���ֽ���ǰ) */


#define	CMD_GET_STATUS		   0x22							 /* ��ȡ�ж�״̬��ȡ���ж����� */
/* ���: �ж�״̬ */


#define	CMD50_WRITE_VAR32		   0x0D							 /* ����ָ����32λCH378ϵͳ���� */
/* ����: ������ַ, ����(�ܳ���32λ,���ֽ���ǰ) */


/* ���ӵ�USB����״̬���� */
#define	ERR_USB_UNKNOWN			   0xFA							 /* δ֪����,��Ӧ�÷��������,����Ӳ�����߳������ */

#define ERR_SUCCESS 0x00     //�����ɹ������ص��ж�״̬��
#define ERR_OPEN_DIR         //ָ��·����Ŀ¼����
#define	ERR_DISK_DISCON			   0x82							 /* ������δ����,���ܴ����Ѿ��Ͽ� */


/* ����״̬ */
#ifndef	CMD_RET_SUCCESS
#define	CMD_RET_SUCCESS			   0x51						  	 /* ��������ɹ� */
#define	CMD_RET_ABORT			   0x5F						     /* �������ʧ�� */
#define	CMD_PARAME_ERR			   0x5E						     /* ����������� */
#define	CMD_IDENTIFY_ERR		   0x5D						     /* �޸�������� */
#define	CMD_TIMEOUT_ERR		   	   0x5C						     /* ���ʱ���� */
#endif



/* ********************************************************************************************************************* */
/* �����ļ�ģʽ�µ��ļ�ϵͳ�����ĵ�ַ */

#ifndef	VAR_FILE_SIZE

/* 8λ/���ֽڱ��� */
#define	VAR8_DEV_CONNECTSTATUS	   0x02                          /* ��ǰ�豸����״̬: 0--δ����, 1--����δ��ʼ��, 2--��ʼ���ɹ� */
#define	VAR8_USB_DEV_SPEED	       0x03                          /* ��ǰ���ӵ�USB�豸�ٶ�: 0--δ֪, 1--����, 2--ȫ��, 3--���� */ 	
#define	VAR8_CMD_INT_STATUS		   0x0B							 /* CH378��ǰ����ִ���ж�״̬(��Բ����жϵ�����) */
#define	VAR8_LAST_CMD     		   0x0C							 /* CH378���յ������һ�������� */
#define	VAR8_CMD_OP_STATUS		   0x0E							 /* CH378��ǰ�����ִ��״̬ */
#define	VAR8_SDO_INT_ENABLE        0x16							 /* SPIģʽ�Ƿ�ʹ��SDO�����ж�֪ͨ��־(0:��ֹ, 1:ʹ��) */
#define	VAR8_DISK_STATUS		   0x21							 /* �����ļ�ģʽ�µĴ��̼��ļ�״̬ */
#define	VAR8_DISK_FAT			   0x22							 /* �߼��̵�FAT��־:1=FAT12,2=FAT16,3=FAT32 */
#define	VAR8_SEC_PER_CLUS		   0x23							 /* �߼��̵�ÿ�������� */
#define	VAR8_DISK_SEC_LEN		   0x24							 /* �߼��̵�ÿ��������С(һ��Ϊ512�ֽ�,������U�̿���Ϊ1024,2048,4096��) */
																 /* 9 = 512, 10 = 1024, 11 = 2048, 12 = 4096 */
/* 32λ/4�ֽڱ��� */
#define	VAR32_DISK_CAP			   0x51							 /* �߼��̵������� */
#define	VAR32_DISK_SEC_SIZE	   	   0x52							 /* �߼��̵�ÿ��������С(һ��Ϊ512�ֽ�,������U�̿���Ϊ1024,2048,4096��) */
#define	VAR32_SYSTEM_PARA	   	   0x55							 /* CH378ϵͳ���� */
#define  RB_WORK_MODE_SET0   	   ( 1 << 0 )					 /* λ1--0: ��ǰоƬ�ӿ�ģʽ( 00:δ֪; 01:8λ����; 02:SPI�ӿ�; 03:�첽���� ) */ 	
#define  RB_WORK_MODE_SET1   	   ( 1 << 1 )
#define  RB_AUTO_CHECK_CONNECT	   ( 1 << 2 )					 /* λ2��CH378�Զ����USB�豸/SD�������ӺͶϿ�ʹ�� */
#define  RB_HOST_SD_UDISK	       ( 1 << 3 )			 		 /* λ3������ģʽ��0---USB�洢�豸��1---SD�� */
#define  RB_USB_CONFIG	    	   ( 1 << 4 ) 					 /* λ4��������ʽ��USB�豸���ӱ�־ */

#define	VAR32_FILE_SIZE			   0x68							 /* ��ǰ�ļ��ĳ���(�ܳ���32λ,���ֽ���ǰ) */
#define	VAR32_DSK_START_LBA		   0x70							 /* �߼��̵���ʼ����������LBA(�ܳ���32λ,���ֽ���ǰ) */
#define	VAR32_DISK_ROOT			   0x71							 /* ����FAT16��Ϊ��Ŀ¼ռ��������,����FAT32��Ϊ��Ŀ¼��ʼ�غ�(�ܳ���32λ,���ֽ���ǰ) */
#define	VAR32_DSK_DAT_START		   0x72							 /* �߼��̵������������ʼLBA(�ܳ���32λ,���ֽ���ǰ) */
#define	VAR32_START_CLUSTER		   0x73							 /* ��ǰ�ļ�����Ŀ¼(�ļ���)����ʼ�غ�(�ܳ���32λ,���ֽ���ǰ) */
#define	VAR32_CURRENT_OFFSET	   0x74							 /* ��ǰ�ļ�ָ��,��ǰ��дλ�õ��ֽ�ƫ��(�ܳ���32λ,���ֽ���ǰ) */
#define	VAR_FAT_DIR_LBA			   0x75							 /* ��ǰ�ļ�Ŀ¼��Ϣ���ڵ�����LBA��ַ(�ܳ���32λ,���ֽ���ǰ) */
#define	VAR_FAT_OFFSET			   0x76							 /* ��ǰ�ļ�Ŀ¼��Ϣ��������ƫ�Ƶ�ַ(�ܳ���32λ,���ֽ���ǰ) */

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



