#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "delay.h" 

#include "stm32f4xx.h"
#include "ch378q.h"

#define CH378_SPI_SCS_LOW( )     PAout(4)=0   /* SPI片选引脚输出低电平 */
#define CH378_SPI_SCS_HIGH( )    PAout(4)=1    /* SPI片选引脚输出高电平 */ 

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
	CH378_SPI_SCS_HIGH( );  									 /* 防止之前未通过xEndCH378Cmd禁止SPI片选 */
	CH378_SPI_SCS_HIGH( ); 
			
	/* 对于双向I/O引脚模拟SPI接口,那么必须确保已经设置SPI_SCS,SPI_SCK,SPI_SDI为输出方向,SPI_SDO为输入方向 */
	CH378_SPI_SCS_LOW( );  										 /* SPI片选有效 */

	/* 发送命令码 */
	Spi376Exchange( mCmd );  									 /* 发出命令码 */
  delay_us(2);											 /* 延时1.5uS确保读写周期大于1.5uS */
	
}

/*******************************************************************************
* Function Name  : xWriteCH378Data
* Description    : 向CH378写数据
* Input          : mData---将要写入CH378的数据
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH378Data( UINT8 mData ) 
{
	Spi376Exchange( mData );  									 /* 发送数据 */
}

/*******************************************************************************
* Function Name  : xReadCH378Data
* Description    : 从CH378读数据
* Input          : None
* Output         : None
* Return         : 返回读取的数据
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
* Description    : 查询CH378中断(INT#低电平)
* Input          : None
* Output         : None
* Return         : 返回中断状态
*******************************************************************************/
UINT8 Query378Interrupt( void )
{
	/* 如果连接了CH378的中断引脚则直接查询中断引脚 */
	/* 如果未连接CH378的中断引脚则查询兼做中断输出的SDO引脚状态 */
#ifdef	CH378_INT_WIRE
	return( CH378_INT_PIN_WIRE( ) ? FALSE : TRUE );  
#else
	return( CH378_SPI_SDO_PIN( ) ? FALSE : TRUE );  
#endif
}

/*******************************************************************************
* Function Name  : mInitCH378Host
* Description    : 初始化CH378
* Input          : None
* Output         : None
* Return         : 返回操作状态
*******************************************************************************/
UINT8 mInitCH378Host( void ) 
{
	UINT8  res;

	/* 检测CH378连接是否正常 */	
	CH378_Port_Init( );  										 /* 接口硬件初始化 */
	xWriteCH378Cmd( CMD_CHECK_EXIST  );  						 /* 测试单片机与CH378之间的通讯接口 */
	xWriteCH378Data( 0x65 );
	res = xReadCH378Data( );
	xEndCH378Cmd( );
	if( res != 0x9A ) 
	{
		/* 通讯接口不正常,可能原因有:接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
		return( ERR_USB_UNKNOWN );  
	}	

	/* 设置CH378工作模式 */
	xWriteCH378Cmd( CMD_SET_USB_MODE );  						 /* 设备USB工作模式 */
//	xWriteCH378Data( 0x04 );
	xWriteCH378Data( 0x06 );

	/* 等待模式设置完毕,对于操作SD卡大概需要10mS左右时间,对于操作USB设备大概需要35mS左右时间 */	
	delay_ms( 50 );											 
	res = xReadCH378Data( );
	xEndCH378Cmd( );
#ifndef	CH378_INT_WIRE
	xWriteCH378Cmd( CMD20_SET_SDO_INT );  						 /* 设置SPI的SDO引脚的中断方式 */
	xWriteCH378Data( 0x16 );
	xWriteCH378Data( 0x01 );  									 /* SDO引脚在SCS片选无效时兼做中断请求输出 */
	xEndCH378Cmd( );
#endif
	if( res == CMD_RET_SUCCESS ) 
	{
		return( ERR_SUCCESS );
	}
	else 
	{
		return( ERR_USB_UNKNOWN );  							 /* 设置模式错误 */
	}
}

/*******************************************************************************
* Function Name  : CH378GetIntStatus
* Description    : 获取中断状态并取消中断请求
* Input          : None
* Output         : None
* Return         : 返回中断状态
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
* Description    : 等待CH378中断(INT#低电平)
* Input          : None
* Output         : None
* Return         : 返回中断状态码, 超时则返回ERR_USB_UNKNOWN
*******************************************************************************/
#ifndef	NO_DEFAULT_CH378_INT

UINT8 Wait378Interrupt( void ) 
{
#ifdef DEF_INT_TIMEOUT

#if	DEF_INT_TIMEOUT < 1
	while( Query378Interrupt( ) == FALSE );  					 /* 一直等中断 */
	return( CH378GetIntStatus( ) );  							 /* 检测到中断 */
#else
	UINT32 i;

	for( i = 0; i < DEF_INT_TIMEOUT; i ++ ) 
	{  
		/* 计数防止超时 */
		if( Query378Interrupt( ) ) 
		{
			return( CH378GetIntStatus( ) );  					 /* 检测到中断 */
		}
		mDelayuS( 3 );
		/* 在等待CH378中断的过程中,可以做些需要及时处理的其它事情 */
	}
	return( ERR_USB_UNKNOWN );  								 /* 不应该发生的情况 */
#endif

#else
	UINT32 i;

	for( i = 0; i < 5000000; i ++ ) 
	{  
		/* 计数防止超时,默认的超时时间,与单片机主频有关 */
		if ( Query378Interrupt( ) ) 
		{
			return( CH378GetIntStatus( ) );  					 /* 检测到中断 */
		}
		delay_ms( 3 );
		 
		/* 在等待CH378中断的过程中,可以做些需要及时处理的其它事情 */
	}
	return( ERR_USB_UNKNOWN );  								 /* 不应该发生的情况 */
#endif
}
#endif


/*******************************************************************************
* Function Name  : CH378SendCmdWaitInt
* Description    : 发出命令码后,等待中断
* Input          : mCmd---需要执行的命令码
* Output         : None
* Return         : 返回中断状态码
*******************************************************************************/
UINT8 CH378SendCmdWaitInt( UINT8 mCmd )  
{
	xWriteCH378Cmd( mCmd );
	xEndCH378Cmd( );
	return( Wait378Interrupt( ) );
}

/*******************************************************************************
* Function Name  : CH378SendCmdDatWaitInt
* Description    : 发出命令码和一字节数据后,等待中断
* Input          : mCmd---需要执行的命令码
*                  mDat---1个字节参数
* Output         : None
* Return         : 返回中断状态码
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
* Description    : 检查U盘/SD卡是否连接
* Input          : None
* Output         : None
* Return         : 返回中断状态码
*******************************************************************************/
UINT8 CH378DiskConnect( void )  
{
	if( Query378Interrupt( ) ) 
	{
		CH378GetIntStatus( );  									 /* 检测到中断 */
	}
	return( CH378SendCmdWaitInt( CMD_DISK_CONNECT ) );
}


/*******************************************************************************
* Function Name  : CH378DiskReady
* Description    : 初始化磁盘并测试磁盘是否就绪
* Input          : None
* Output         : None
* Return         : 返回中断状态码
*******************************************************************************/
UINT8 CH378DiskReady( void )  
{
	return( CH378SendCmdWaitInt( CMD_DISK_MOUNT ) );
}

/*******************************************************************************
* Function Name  : CH378SetFileName
* Description    : 设置将要操作的文件的完整文件名(路径名)
* Input          : PathName---要设置的文件名或路径名
* Output         : None
* Return         : None
*******************************************************************************/
void CH378SetFileName( PUINT8 PathName ) 
{
	UINT8  i, c;

	if( PathName == NULL )										 /* 如果指针为空,不去从新设置文件名 */
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
* Description    : 新建文件并打开,如果文件已经存在那么先删除后再新建
* Input          : PathName---要新建文件的路径及文件名
* Output         : None
* Return         : 返回中断状态
*******************************************************************************/
UINT8 CH378FileCreate( PUINT8 PathName ) 
{
	CH378SetFileName( PathName );  								 /* 设置将要操作的文件的文件名 */
	return( CH378SendCmdWaitInt( CMD_FILE_CREATE ) );
}



/*******************************************************************************
* Function Name  : CH378FileOpen
* Description    : 打开文件或者目录(文件夹)
* Input          : PathName---要打开的文件或目录的完整路径名
* Output         : None
* Return         : 返回中断状态
*******************************************************************************/
UINT8 CH378FileOpen( PUINT8 PathName )  
{
	CH378SetFileName( PathName );  								 /* 设置将要操作的文件的文件名 */
	return( CH378SendCmdWaitInt( CMD_FILE_OPEN ) );	
}


/*******************************************************************************
* Function Name  : CH378Read32bitDat
* Description    : 从CH378芯片读取32位的数据并结束命令
* Input          : None
* Output         : None
* Return         : 返回32位数据
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
* Description    : 读CH378芯片内部的32位变量
* Input          : addr---32位变量地址
* Output         : None
* Return         : 返回32位数据
*******************************************************************************/
UINT32 CH378ReadVar32( UINT8 addr )
{
	xWriteCH378Cmd( CMD14_READ_VAR32 );
	xWriteCH378Data( addr );
	return( CH378Read32bitDat( ) );  							 /* 从CH378芯片读取32位的数据并结束命令 */
}

/*******************************************************************************
* Function Name  : CH378ReadVar8
* Description    : 读CH378芯片内部的8位变量
* Input          : addr---8位变量地址
* Output         : None
* Return         : 返回8位数据
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
* Description    : 读取当前文件长度
* Input          : None
* Output         : None
* Return         : 返回当前文件长度
*******************************************************************************/
UINT32 CH378GetFileSize( void ) 
{
	return( CH378ReadVar32( VAR32_FILE_SIZE ) );
}



/*******************************************************************************
* Function Name  : CH378SecLocate
* Description    : 以扇区为单位移动当前文件指针
* Input          : offset---偏移扇区数
* Output         : None
* Return         : 返回中断状态
*******************************************************************************/
UINT8 CH378SecLocate( UINT32 offset )
{
	xWriteCH378Cmd( CMD_SEC_LOCATE );
	xWriteCH378Data( (UINT8)offset );
	xWriteCH378Data( (UINT8)((UINT16)offset >> 8 ) );
	xWriteCH378Data( (UINT8)( offset >> 16 ) );
	xWriteCH378Data( 0 );  										 /* 超出最大文件尺寸 */
	xEndCH378Cmd( );
	return( Wait378Interrupt( ) );
}


/*******************************************************************************
* Function Name  : CH378ByteLocate
* Description    : 以字节为单位移动当前文件指针
* Input          : offset---偏移字节数
* Output         : None
* Return         : 返回中断状态
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
* Description    : 向内部指定缓冲区指定偏移地址写入数据块(内部指针自动增加)
* Input          : buf---数据缓冲区
*				   offset---偏移地址
*				   len---写入数据长度	
* Output         : None
* Return         : 返回长度
*******************************************************************************/
UINT16 CH378WriteOfsBlock( PUINT8 buf, UINT16 offset, UINT16 len ) 
{
	UINT16 l;
	
	xWriteCH378Cmd( CMD_WR_HOST_OFS_DATA );					 /* 写入1个字节命令码 */		
	xWriteCH378Data( ( UINT8 )offset );							 /* 写入2个字节偏移地址 */			
	xWriteCH378Data( ( UINT8 )(offset >> 8 ));							
	xWriteCH378Data( ( UINT8 )len );							 /* 写入2个字节数据长度 */
	xWriteCH378Data( ( UINT8 )(len >> 8) );
//	mDelayuS( 1 ); 												 /* 对于高速MCU,可在此处适当增加延时 */	
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
* Description    : 以字节为单位向当前位置写入数据块
* Input          : buf---缓冲区数据
*				   ReqCount----想要写入的数据长度
*				   RealCount---实际写入的数据长度	
* Output         : None
* Return         : 返回中断状态
*******************************************************************************/
UINT8 CH378ByteWrite( PUINT8 buf, UINT16 ReqCount )  
{
	UINT8  s;

	/* 将要写入的数据预先送到内部缓冲区中 */
	CH378WriteOfsBlock( buf, 0x0000, ReqCount );

	/* 发送字节写入命令 */		
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
* Description    : 写CH378芯片内部的32位变量
* Input          : addr---32位变量地址
*                  dat---32位变量值
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
* Description    : 设置当前文件长度
*                  注意: 该函数并不是真正设置当前文件长度,而是临时修改当前打开文件
*                        的长度,以便进行某些操作,比如读取文件最后不足一个扇区的数据
* Input          : filesize---文件长度
* Output         : None
* Return         : None
*******************************************************************************/
void CH378SetFileSize( UINT32 filesize ) 
{
	CH378WriteVar32( VAR32_FILE_SIZE, filesize );
}




/*******************************************************************************
* Function Name  : CH378FileClose
* Description    : 关闭当前已经打开的文件或者目录(文件夹)
* Input          : UpdateSz---0---不自动更新文件长度; 1---自动更新文件长度
* Output         : None
* Return         : 返回中断状态
*******************************************************************************/
UINT8 CH378FileClose( UINT8 UpdateSz ) 
{
	return( CH378SendCmdDatWaitInt( CMD_FILE_CLOSE, UpdateSz ) );

}


/*******************************************************************************
* Function Name  : mStopIfError
* Description    : 检查操作状态
*                  如果错误,则显示错误代码并停机,应该替换为实际的处理措施,
*                  例如显示错误信息,等待用户确认后重试等
* Input          : iError---待检测的状态值
* Output         : None
* Return         : None
*******************************************************************************/
void mStopIfError( UINT8 iError )
{
	if( iError == ERR_SUCCESS ) 
	{
		return;  												 /* 操作成功 */
	}
//	printf( "Error: %02X\n", (UINT16)iError );  			 	 /* 显示错误 */
	while( 1 ) 
	{
/*		LED_OUT_ACT( );*/  										 /* LED闪烁 */
		delay_ms( 200 );
/*		LED_OUT_INACT( );*/
		delay_ms( 200 );
	}
}































































