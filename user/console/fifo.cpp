/**	@file	fifo.cpp
    @brief	FIFO buffer.
	@author Liu, Yi-Sheng

	- 2020/09/20 Created.
*/

#include <stdio.h>

#include "fifo.h"

/**	\brief Constructor.
 *
 */
FifoBuffer::FifoBuffer(uint32_t bufSize)
{
	Clear();

	m_bufSize = bufSize;

	m_pBuffer = new uint8_t[m_bufSize];
}

/**	\brief Destructor.
 *
 */
FifoBuffer::~FifoBuffer()
{
	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
}

/**	\brief	Reset index to clear buffer.
 *
 */
void	FifoBuffer::Clear()
{
	m_putPtr = 0;
	m_getPtr = 0;
	m_dataSize = 0;
}

/**	\brief	Save data byte to FIFO.
 *
 */
FIFO_STATE	FifoBuffer::PutData(uint8_t byte)
{
	if (m_dataSize >= m_bufSize)
		return FIFO_FULL;

	m_pBuffer[m_putPtr] = byte;

	if (++m_putPtr >= m_bufSize)
		m_putPtr = 0;

	m_dataSize++;

	return FIFO_OK;
}

/**	\brief	Get data byte from FIFO.
 *
 */
uint8_t	FifoBuffer::GetData()
{
	uint8_t	byte = 0;

	if (m_dataSize > 0)
	{
		byte = m_pBuffer[m_getPtr];

		if (++m_getPtr >= m_bufSize)
			m_getPtr = 0;

		m_dataSize--;
	}

	return byte;
}
