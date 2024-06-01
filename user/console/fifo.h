/**	@file	fifo.h
    @brief	FIFO buffer.
	@author Liu, Yi-Sheng
*/

#ifndef __fifo_H
#define __fifo_H

#include "stdint.h"

enum FIFO_STATE
{
	FIFO_OK,	//!< FIFO not overflow
	FIFO_FULL,	//!< FIFO overflow
};

/**	\brief	FIFO buffer.
 *
 */
class FifoBuffer
{
public:
	FifoBuffer(uint32_t bufSize);
	~FifoBuffer();

	void	Clear();
	FIFO_STATE PutData(uint8_t byte);
	uint8_t GetData();
	uint32_t DataSize() {return m_dataSize;}	//!< return number of data in FIFO

private:
	volatile uint32_t	m_putPtr;	//!< index of FIFO buffer to put data
	volatile uint32_t	m_getPtr;	//!< index of FIFO buffer to get data
	volatile uint32_t	m_dataSize;	//!< number of data in FIFO

	uint32_t	m_bufSize;	//!< buffer size of FIFO
	uint8_t		*m_pBuffer;	//!< pointer to FIFO
};

#endif
