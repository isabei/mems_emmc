#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "delay.h" 

#include "stm32f4xx.h"
#include "ch378q.h"

#define CH378_SPI_SCS_LOW( )     PAout(4)=0   /* SPIƬѡ��������͵�ƽ */
#define CH378_SPI_SCS_HIGH( )    PAout(4)=1    /* SPIƬѡ��������ߵ�ƽ */ 

void CH378_Port_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_SPI1); 
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); 
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);

  CH378_SPI_SCS_HIGH( );
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);   
  SPI_Cmd(SPI1, ENABLE);

}



UINT8 Spi376Exchange( UINT8 d )  
{  
  /* Loop while DR register in not emplty */
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);	
	while( ( SPI1->SR & SPI_I2S_FLAG_TXE ) == RESET );

	/* Send byte through the SPI1 peripheral */
//	SPI_I2S_SendData(SPI1, byte);
	SPI1->DR = d;

	/* Wait to receive a byte */
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	while( ( SPI1->SR & SPI_I2S_FLAG_RXNE ) == RESET );

	/* Return the byte read from the SPI bus */
//	return  SPI_I2S_ReceiveData(SPI1);
	return( SPI1->DR );	
} 


void xWriteCH378Cmd( UINT8 mCmd )  
{
	CH378_SPI_SCS_HIGH( );  									 /* ��ֹ֮ǰδͨ��xEndCH378Cmd��ֹSPIƬѡ */
	CH378_SPI_SCS_HIGH( ); 
			
	/* ����˫��I/O����ģ��SPI�ӿ�,��ô����ȷ���Ѿ�����SPI_SCS,SPI_SCK,SPI_SDIΪ�������,SPI_SDOΪ���뷽�� */
	CH378_SPI_SCS_LOW( );  										 /* SPIƬѡ��Ч */

	/* ���������� */
	Spi376Exchange( mCmd );  									 /* ���������� */
  delay_us(2);											 /* ��ʱ1.5uSȷ����д���ڴ���1.5uS */
	
}

/*******************************************************************************
* Function Name  : xWriteCH378Data
* Description    : ��CH378д����
* Input          : mData---��Ҫд��CH378������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH378Data( UINT8 mData ) 
{
	Spi376Exchange( mData );  									 /* �������� */
}

/*******************************************************************************
* Function Name  : xReadCH378Data
* Description    : ��CH378������
* Input          : None
* Output         : None
* Return         : ���ض�ȡ������
*******************************************************************************/
UINT8 xReadCH378Data( void ) 
{
	return( Spi376Exchange( 0xFF ) );  	
}

void xEndCH378Cmd(void )	
{ 
	CH378_SPI_SCS_HIGH( ); 
 
}  

/*******************************************************************************
* Function Name  : Query378Interrupt
* Description    : ��ѯCH378�ж�(INT#�͵�ƽ)
* Input          : None
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 Query378Interrupt( void )
{
	/* ���������CH378���ж�������ֱ�Ӳ�ѯ�ж����� */
	/* ���δ����CH378���ж��������ѯ�����ж������SDO����״̬ */
#ifdef	CH378_INT_WIRE
	return( CH378_INT_PIN_WIRE( ) ? FALSE : TRUE );  
#else
	return( CH378_SPI_SDO_PIN( ) ? FALSE : TRUE );  
#endif
}

/*******************************************************************************
* Function Name  : mInitCH378Host
* Description    : ��ʼ��CH378
* Input          : None
* Output         : None
* Return         : ���ز���״̬
*******************************************************************************/
UINT8 mInitCH378Host( void ) 
{
	UINT8  res;

	/* ���CH378�����Ƿ����� */	
	CH378_Port_Init( );  										 /* �ӿ�Ӳ����ʼ�� */
	xWriteCH378Cmd( CMD_CHECK_EXIST  );  						 /* ���Ե�Ƭ����CH378֮���ͨѶ�ӿ� */
	xWriteCH378Data( 0x65 );
	res = xReadCH378Data( );
	xEndCH378Cmd( );
	if( res != 0x9A ) 
	{
		/* ͨѶ�ӿڲ�����,����ԭ����:�ӿ������쳣,�����豸Ӱ��(Ƭѡ��Ψһ),���ڲ�����,һֱ�ڸ�λ,���񲻹��� */
		return( ERR_USB_UNKNOWN );  
	}	

	/* ����CH378����ģʽ */
	xWriteCH378Cmd( CMD_SET_USB_MODE );  						 /* �豸USB����ģʽ */
//	xWriteCH378Data( 0x04 );
	xWriteCH378Data( 0x06 );

	/* �ȴ�ģʽ�������,���ڲ���SD�������Ҫ10mS����ʱ��,���ڲ���USB�豸�����Ҫ35mS����ʱ�� */	
	delay_ms( 50 );											 
	res = xReadCH378Data( );
	xEndCH378Cmd( );
#ifndef	CH378_INT_WIRE
	xWriteCH378Cmd( CMD20_SET_SDO_INT );  						 /* ����SPI��SDO���ŵ��жϷ�ʽ */
	xWriteCH378Data( 0x16 );
	xWriteCH378Data( 0x01 );  									 /* SDO������SCSƬѡ��Чʱ�����ж�������� */
	xEndCH378Cmd( );
#endif
	if( res == CMD_RET_SUCCESS ) 
	{
		return( ERR_SUCCESS );
	}
	else 
	{
		return( ERR_USB_UNKNOWN );  							 /* ����ģʽ���� */
	}
}

/*******************************************************************************
* Function Name  : CH378GetIntStatus
* Description    : ��ȡ�ж�״̬��ȡ���ж�����
* Input          : None
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 CH378GetIntStatus( void ) 
{
	UINT8  s;

	xWriteCH378Cmd( CMD_GET_STATUS );
	s = xReadCH378Data( );
	xEndCH378Cmd( );
	return( s );
}


/*******************************************************************************
* Function Name  : Wait378Interrupt
* Description    : �ȴ�CH378�ж�(INT#�͵�ƽ)
* Input          : None
* Output         : None
* Return         : �����ж�״̬��, ��ʱ�򷵻�ERR_USB_UNKNOWN
*******************************************************************************/
#ifndef	NO_DEFAULT_CH378_INT

UINT8 Wait378Interrupt( void ) 
{
#ifdef DEF_INT_TIMEOUT

#if	DEF_INT_TIMEOUT < 1
	while( Query378Interrupt( ) == FALSE );  					 /* һֱ���ж� */
	return( CH378GetIntStatus( ) );  							 /* ��⵽�ж� */
#else
	UINT32 i;

	for( i = 0; i < DEF_INT_TIMEOUT; i ++ ) 
	{  
		/* ������ֹ��ʱ */
		if( Query378Interrupt( ) ) 
		{
			return( CH378GetIntStatus( ) );  					 /* ��⵽�ж� */
		}
		mDelayuS( 3 );
		/* �ڵȴ�CH378�жϵĹ�����,������Щ��Ҫ��ʱ������������� */
	}
	return( ERR_USB_UNKNOWN );  								 /* ��Ӧ�÷�������� */
#endif

#else
	UINT32 i;

	for( i = 0; i < 5000000; i ++ ) 
	{  
		/* ������ֹ��ʱ,Ĭ�ϵĳ�ʱʱ��,�뵥Ƭ����Ƶ�й� */
		if ( Query378Interrupt( ) ) 
		{
			return( CH378GetIntStatus( ) );  					 /* ��⵽�ж� */
		}
		delay_ms( 3 );
		 
		/* �ڵȴ�CH378�жϵĹ�����,������Щ��Ҫ��ʱ������������� */
	}
	return( ERR_USB_UNKNOWN );  								 /* ��Ӧ�÷�������� */
#endif
}
#endif


/*******************************************************************************
* Function Name  : CH378SendCmdWaitInt
* Description    : �����������,�ȴ��ж�
* Input          : mCmd---��Ҫִ�е�������
* Output         : None
* Return         : �����ж�״̬��
*******************************************************************************/
UINT8 CH378SendCmdWaitInt( UINT8 mCmd )  
{
	xWriteCH378Cmd( mCmd );
	xEndCH378Cmd( );
	return( Wait378Interrupt( ) );
}

/*******************************************************************************
* Function Name  : CH378SendCmdDatWaitInt
* Description    : �����������һ�ֽ����ݺ�,�ȴ��ж�
* Input          : mCmd---��Ҫִ�е�������
*                  mDat---1���ֽڲ���
* Output         : None
* Return         : �����ж�״̬��
*******************************************************************************/
UINT8 CH378SendCmdDatWaitInt( UINT8 mCmd, UINT8 mDat )
{
	xWriteCH378Cmd( mCmd );
	xWriteCH378Data( mDat );
	xEndCH378Cmd( );
	return( Wait378Interrupt( ) );
}


/*******************************************************************************
* Function Name  : CH378DiskConnect
* Description    : ���U��/SD���Ƿ�����
* Input          : None
* Output         : None
* Return         : �����ж�״̬��
*******************************************************************************/
UINT8 CH378DiskConnect( void )  
{
	if( Query378Interrupt( ) ) 
	{
		CH378GetIntStatus( );  									 /* ��⵽�ж� */
	}
	return( CH378SendCmdWaitInt( CMD_DISK_CONNECT ) );
}


/*******************************************************************************
* Function Name  : CH378DiskReady
* Description    : ��ʼ�����̲����Դ����Ƿ����
* Input          : None
* Output         : None
* Return         : �����ж�״̬��
*******************************************************************************/
UINT8 CH378DiskReady( void )  
{
	return( CH378SendCmdWaitInt( CMD_DISK_MOUNT ) );
}

/*******************************************************************************
* Function Name  : CH378SetFileName
* Description    : ���ý�Ҫ�������ļ��������ļ���(·����)
* Input          : PathName---Ҫ���õ��ļ�����·����
* Output         : None
* Return         : None
*******************************************************************************/
void CH378SetFileName( PUINT8 PathName ) 
{
	UINT8  i, c;

	if( PathName == NULL )										 /* ���ָ��Ϊ��,��ȥ���������ļ��� */
	{
		return;
	}	
	xWriteCH378Cmd( CMD_SET_FILE_NAME );
	for( i = MAX_FILE_NAME_LEN; i != 0; --i ) 
	{
		c = *PathName;
		xWriteCH378Data( c );
		if( c == 0 ) 
		{
			break;
		}
		PathName ++;
	}
	xEndCH378Cmd( );
}


/*******************************************************************************
* Function Name  : CH378FileCreate
* Description    : �½��ļ�����,����ļ��Ѿ�������ô��ɾ�������½�
* Input          : PathName---Ҫ�½��ļ���·�����ļ���
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 CH378FileCreate( PUINT8 PathName ) 
{
	CH378SetFileName( PathName );  								 /* ���ý�Ҫ�������ļ����ļ��� */
	return( CH378SendCmdWaitInt( CMD_FILE_CREATE ) );
}



/*******************************************************************************
* Function Name  : CH378FileOpen
* Description    : ���ļ�����Ŀ¼(�ļ���)
* Input          : PathName---Ҫ�򿪵��ļ���Ŀ¼������·����
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 CH378FileOpen( PUINT8 PathName )  
{
	CH378SetFileName( PathName );  								 /* ���ý�Ҫ�������ļ����ļ��� */
	return( CH378SendCmdWaitInt( CMD_FILE_OPEN ) );	
}


/*******************************************************************************
* Function Name  : CH378Read32bitDat
* Description    : ��CH378оƬ��ȡ32λ�����ݲ���������
* Input          : None
* Output         : None
* Return         : ����32λ����
*******************************************************************************/
UINT32 CH378Read32bitDat( void )
{
	UINT8  c0, c1, c2, c3;

	c0 = xReadCH378Data( );
	c1 = xReadCH378Data( );
	c2 = xReadCH378Data( );
	c3 = xReadCH378Data( );
	xEndCH378Cmd( );
	return( c0 | (UINT16)c1 << 8 | (UINT32)c2 << 16 | (UINT32)c3 << 24 );
}

/*******************************************************************************
* Function Name  : CH378ReadVar32
* Description    : ��CH378оƬ�ڲ���32λ����
* Input          : addr---32λ������ַ
* Output         : None
* Return         : ����32λ����
*******************************************************************************/
UINT32 CH378ReadVar32( UINT8 addr )
{
	xWriteCH378Cmd( CMD14_READ_VAR32 );
	xWriteCH378Data( addr );
	return( CH378Read32bitDat( ) );  							 /* ��CH378оƬ��ȡ32λ�����ݲ��������� */
}

/*******************************************************************************
* Function Name  : CH378ReadVar8
* Description    : ��CH378оƬ�ڲ���8λ����
* Input          : addr---8λ������ַ
* Output         : None
* Return         : ����8λ����
*******************************************************************************/
UINT8 CH378ReadVar8( UINT8 addr ) 
{
	UINT8 dat;

	xWriteCH378Cmd( CMD_READ_VAR8	 );
	xWriteCH378Data( addr );
	dat = xReadCH378Data( );
	xEndCH378Cmd( );
	return( dat );
}


/*******************************************************************************
* Function Name  : CH378GetFileSize
* Description    : ��ȡ��ǰ�ļ�����
* Input          : None
* Output         : None
* Return         : ���ص�ǰ�ļ�����
*******************************************************************************/
UINT32 CH378GetFileSize( void ) 
{
	return( CH378ReadVar32( VAR32_FILE_SIZE ) );
}



/*******************************************************************************
* Function Name  : CH378SecLocate
* Description    : ������Ϊ��λ�ƶ���ǰ�ļ�ָ��
* Input          : offset---ƫ��������
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 CH378SecLocate( UINT32 offset )
{
	xWriteCH378Cmd( CMD_SEC_LOCATE );
	xWriteCH378Data( (UINT8)offset );
	xWriteCH378Data( (UINT8)((UINT16)offset >> 8 ) );
	xWriteCH378Data( (UINT8)( offset >> 16 ) );
	xWriteCH378Data( 0 );  										 /* ��������ļ��ߴ� */
	xEndCH378Cmd( );
	return( Wait378Interrupt( ) );
}


/*******************************************************************************
* Function Name  : CH378ByteLocate
* Description    : ���ֽ�Ϊ��λ�ƶ���ǰ�ļ�ָ��
* Input          : offset---ƫ���ֽ���
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 CH378ByteLocate( UINT32 offset ) 
{
	xWriteCH378Cmd( CMD_BYTE_LOCATE );
	xWriteCH378Data( (UINT8)offset );
	xWriteCH378Data( (UINT8)( (UINT16)offset >> 8 ) );
	xWriteCH378Data( (UINT8)( offset >> 16 ) );
	xWriteCH378Data( (UINT8)( offset >> 24 ) );
	xEndCH378Cmd( );
	return( Wait378Interrupt( ) );
}



/*******************************************************************************
* Function Name  : CH378WriteOfsBlock
* Description    : ���ڲ�ָ��������ָ��ƫ�Ƶ�ַд�����ݿ�(�ڲ�ָ���Զ�����)
* Input          : buf---���ݻ�����
*				   offset---ƫ�Ƶ�ַ
*				   len---д�����ݳ���	
* Output         : None
* Return         : ���س���
*******************************************************************************/
UINT16 CH378WriteOfsBlock( PUINT8 buf, UINT16 offset, UINT16 len ) 
{
	UINT16 l;
	
	xWriteCH378Cmd( CMD_WR_HOST_OFS_DATA );					 /* д��1���ֽ������� */		
	xWriteCH378Data( ( UINT8 )offset );							 /* д��2���ֽ�ƫ�Ƶ�ַ */			
	xWriteCH378Data( ( UINT8 )(offset >> 8 ));							
	xWriteCH378Data( ( UINT8 )len );							 /* д��2���ֽ����ݳ��� */
	xWriteCH378Data( ( UINT8 )(len >> 8) );
//	mDelayuS( 1 ); 												 /* ���ڸ���MCU,���ڴ˴��ʵ�������ʱ */	
  delay_ms(10);	
	if( len )
	{
		l = len;
		do
		{
			xWriteCH378Data( *buf );
			buf++;
			delay_ms(1);
		}while( --l );		
	}
	xEndCH378Cmd( );
	return( len );
}


/*******************************************************************************
* Function Name  : CH378ByteWrite
* Description    : ���ֽ�Ϊ��λ��ǰλ��д�����ݿ�
* Input          : buf---����������
*				   ReqCount----��Ҫд������ݳ���
*				   RealCount---ʵ��д������ݳ���	
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 CH378ByteWrite( PUINT8 buf, UINT16 ReqCount )  
{
	UINT8  s;

	/* ��Ҫд�������Ԥ���͵��ڲ��������� */
	CH378WriteOfsBlock( buf, 0x0000, ReqCount );

	/* �����ֽ�д������ */		
	xWriteCH378Cmd( CMD_BYTE_WRITE );
	xWriteCH378Data( (UINT8)ReqCount );
	xWriteCH378Data( (UINT8)( ReqCount >> 8 ) );
	xEndCH378Cmd( );
	s = Wait378Interrupt( );
//	if( s != ERR_SUCCESS )
//	{
//		if( RealCount ) 
//		{
//			*RealCount = 0;
//		}		
//	}
//	else
//	{
//		if( RealCount ) 
//		{
//			*RealCount = ReqCount;
//		}				
//	}	
	return( s );
}



/*******************************************************************************
* Function Name  : CH378WriteVar32
* Description    : дCH378оƬ�ڲ���32λ����
* Input          : addr---32λ������ַ
*                  dat---32λ����ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void CH378WriteVar32( UINT8 addr, UINT32 dat ) 
{
	xWriteCH378Cmd( CMD50_WRITE_VAR32 );
	xWriteCH378Data( addr );
	xWriteCH378Data( (UINT8)dat );
	xWriteCH378Data( (UINT8)( (UINT16)dat >> 8 ) );
	xWriteCH378Data( (UINT8)( dat >> 16 ) );
	xWriteCH378Data( (UINT8)( dat >> 24 ) );
	xEndCH378Cmd( );
}



/*******************************************************************************
* Function Name  : CH378SetFileSize
* Description    : ���õ�ǰ�ļ�����
*                  ע��: �ú����������������õ�ǰ�ļ�����,������ʱ�޸ĵ�ǰ���ļ�
*                        �ĳ���,�Ա����ĳЩ����,�����ȡ�ļ������һ������������
* Input          : filesize---�ļ�����
* Output         : None
* Return         : None
*******************************************************************************/
void CH378SetFileSize( UINT32 filesize ) 
{
	CH378WriteVar32( VAR32_FILE_SIZE, filesize );
}




/*******************************************************************************
* Function Name  : CH378FileClose
* Description    : �رյ�ǰ�Ѿ��򿪵��ļ�����Ŀ¼(�ļ���)
* Input          : UpdateSz---0---���Զ������ļ�����; 1---�Զ������ļ�����
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 CH378FileClose( UINT8 UpdateSz ) 
{
	return( CH378SendCmdDatWaitInt( CMD_FILE_CLOSE, UpdateSz ) );

}


/*******************************************************************************
* Function Name  : mStopIfError
* Description    : ������״̬
*                  �������,����ʾ������벢ͣ��,Ӧ���滻Ϊʵ�ʵĴ����ʩ,
*                  ������ʾ������Ϣ,�ȴ��û�ȷ�Ϻ����Ե�
* Input          : iError---������״ֵ̬
* Output         : None
* Return         : None
*******************************************************************************/
void mStopIfError( UINT8 iError )
{
	if( iError == ERR_SUCCESS ) 
	{
		return;  												 /* �����ɹ� */
	}
//	printf( "Error: %02X\n", (UINT16)iError );  			 	 /* ��ʾ���� */
	while( 1 ) 
	{
/*		LED_OUT_ACT( );*/  										 /* LED��˸ */
		delay_ms( 200 );
/*		LED_OUT_INACT( );*/
		delay_ms( 200 );
	}
}































































