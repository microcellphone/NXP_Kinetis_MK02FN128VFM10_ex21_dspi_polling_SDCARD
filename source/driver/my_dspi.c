#include <my_dspi.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK02F12810.h"
#include "fsl_dspi.h"


void SPI_Config_Request(uint32_t bitlen, uint32_t speed, uint32_t spi_cs_type)
{
/*
	uint32_t i;
  uint32_t Dummy;

  if (spi_cs_type == CS_AUTO) {
	  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, (IOCON_FUNC1 | IOCON_MODE_INACT));
  } else {// if (spi_cs_type == CS_GPIO)
	  Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 2); // PIO0_2 out
	  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_2, (IOCON_FUNC0 | IOCON_MODE_INACT));
	  Chip_GPIO_SetPinState(LPC_GPIO, 0, 2, true); // high
  }

  Chip_SSP_Init(LPC_SSP0);
  Chip_SSP_SetBitRate(LPC_SSP0, 1000000);

  ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
  ssp_format.bits        = SSP_DATA_BITS;
  ssp_format.clockMode   = SSP_CLOCK_MODE0;
  Chip_SSP_SetFormat(LPC_SSP0, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
  Chip_SSP_SetMaster(LPC_SSP0, 1);
  Chip_SSP_Enable(LPC_SSP0);

  NVIC_EnableIRQ(SSP0_IRQn);
  NVIC_SetPriority(SSP0_IRQn, 3);
*/
}

void  SPI_Tx_Rx_Data(uint8_t *txdata, uint8_t tx_length, uint8_t *rxdata, uint8_t rx_length)
{

	while (((SPI0->SR) & SPI_SR_TFFF_MASK) == 0U){;} // Is HW buffer empty?
	SPI0->PUSHR = *txdata;          					// Put a character with command to the transmit register
	SPI0->SR = (uint32_t)(SPI_SR_TFFF_MASK);    		// Clear Tx FIFO fill flags

	while (((SPI0->SR) & SPI_SR_RFDF_MASK) == 0U){;} // Is any char in HW buffer?
	*rxdata = (uint16_t)SPI0->POPR;                	// Read character from receiver
	SPI0->SR = (uint32_t)(SPI_SR_RFDF_MASK);    		// Clear Rx FIFO drain flags

	return;
}


uint32_t SPI_TxRxData(uint32_t txdata)
{
  uint32_t rxdata;

  SPI_Tx_Rx_Data((uint8_t *)&txdata, 1, (uint8_t *)&rxdata, 1);

  return rxdata;
}


void SPI_TxData(uint32_t txdata)
{
    SPI_TxRxData(txdata);
/*
    dspi_transfer_t masterXfer;
    status_t dspi_status;

    masterXfer.txData   = &txdata;
    masterXfer.rxData   = 0;
    masterXfer.dataSize = 1;
    dspi_status = DSPI_MasterTransferBlocking(SPI0, &masterXfer);
	if(dspi_status != kStatus_Success) while(1);
*/
}


uint32_t SPI_RxData(void)
{
    uint32_t rxdata;

    rxdata = SPI_TxRxData(0xff);
    return rxdata;
}


void SSP0_Send_Request(uint8_t dat)
{
  SPI_TxData(dat);
}

void SSP0_Send_Request16(uint16_t dat)
{
  SSP0_Send_Request((uint8_t)(dat>>8));
  SSP0_Send_Request((uint8_t)dat);
}
