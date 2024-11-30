/*
 * uart_RingBuffer.c
 *
 */

#include "uart_RingBuffer.h"
#include <string.h>

/********************************* define the UART you are using *********************************/

UART_HandleTypeDef huart1;

#define uart &huart1

#define TIMEOUT_DEF 500  // 500ms timeout for the functions
uint16_t timeout;

/* put the following in the ISR

   extern void Uart_isr (UART_HandleTypeDef *huart);
   extern uint16_t timeout;

*/


/************************************* NO CHANGES AFTER THIS *************************************/

ring_buffer rx_buffer = { { 0 }, 0, 0};
ring_buffer tx_buffer = { { 0 }, 0, 0};

ring_buffer *_rx_buffer;
ring_buffer *_tx_buffer;


/******************** Utility Functions ********************/
static void Ringbuf_reset(ring_buffer *buffer);
static void store_char(unsigned char c, ring_buffer *buffer);

void Ringbuf_init(void)
{
    _rx_buffer = &rx_buffer;
    _tx_buffer = &tx_buffer;

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_FE) ||
            __HAL_UART_GET_FLAG(huart, UART_FLAG_NE) ||
            __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {

        // Clear error flags and reset the buffer
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE | UART_FLAG_NE | UART_FLAG_ORE);
        Ringbuf_reset(_rx_buffer);  // Reset only the receive buffer

        if (uart == NULL) return;
    }


    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(uart, UART_IT_ERR);

    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(uart, UART_IT_RXNE);
}

static void store_char(unsigned char c, ring_buffer *buffer)
{
    int i = (unsigned int)(buffer->head + 1) % UART_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if(i != buffer->tail) {
        buffer->buffer[buffer->head] = c;
        buffer->head = i;
    }
}

static void Ringbuf_reset(ring_buffer *buffer)
{
    for (int i = 0; i < UART_BUFFER_SIZE; i++)
    {
        buffer->buffer[i] = 0;
    }    
    buffer->head = 0;
    buffer->tail = 0;
}










