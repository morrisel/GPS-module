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
static int check_for(char *str, char *buffinder);


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

/* checks if the entered string is present in the given buffer */
static int check_for(char *str, char *buffinder)
{
    int slen = strlen(str);       // Length of the string to find
    int blen = strlen(buffinder); // Length of the buffer
    int so_far = 0;               // Number of characters matched so far
    int indx = 0;                 // Current index in the buffer

repeat:
    while (str[so_far] != buffinder[indx]) // Look for the start of the match
    {
        indx++;
        if (indx >= blen) return -1; // If we reached the end of the buffer, the string is not found
    }

    if (str[so_far] == buffinder[indx])
    {
        while (str[so_far] == buffinder[indx]) // Try to match the entire string
        {
            so_far++;
            indx++;
            if (so_far == slen) return 1; // String found
        }
    }

    if (indx < blen)
    {
        so_far = 0; // If the string was not found, start from the next index
        goto repeat; // Repeat the process from the new position in the buffer
    }

    return -1; // If the string is not found
}



int Uart_read(void)
{
    // If the head isn't ahead of the tail, there are no characters available
    if (_rx_buffer->head == _rx_buffer->tail)
    {
        return -1; // No data available
    }
    else
    {
        // Read the data from the buffer at the tail position
        unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];

        // Increment the tail index, wrapping around if necessary
        _rx_buffer->tail = (_rx_buffer->tail + 1) % UART_BUFFER_SIZE;

        // Return the read character
        return c;
    }
}


/* schema of the ring buffer:
 *
 *
 *                 0                                      1
 *         -----------------                      -----------------
 *        |                 |                    |                 |
 *        |  tail  |  head  |    ----------->    |  tail  |  head  |
 *        |                 |                    |                 |
 *         -----------------                      -----------------
 *                     head-->|     buffer     |<--tail
 *                      ^        [0-9][A-Za-z]
 *                      |
 *                  (Ring buffer: after 512 goes back to 0)
 *                _tx_buffer  0 --------------> 512
 *
 *                _rx_buffer
 *
 * After reaching the end of the buffer (512), the pointers wrap back to the beginning (0).
 *
 */
/* writes a single character to the uart and increments head */
void Uart_write(int c)
{
    if (c < 0 || c > 255) return; // Validate input

    // calculate the new value of head
    int i = (unsigned int)(_tx_buffer->head + 1) % UART_BUFFER_SIZE;

    while (i == _tx_buffer->tail);  // Wait if the buffer is full

    // variation 1: timeout mechanism
    //    uint32_t start_time = HAL_GetTick(); // Start time
    //    while (i == _tx_buffer->tail)
    //    {
    //        if ((HAL_GetTick() - start_time) > TIMEOUT_DEF)
    //        {
    //            return; // Exit after timeout
    //        }   
    //    }
    //  
    // variation 2: handle buffer overflow
    //    // if head reaches tail, it indicates a buffer overflow
    //    if (i == _tx_buffer->tail)      // buffer overflow
    //    {
    //        return;
    //    }

    // store the character in the buffer
    _tx_buffer->buffer[_tx_buffer->head] = (uint8_t)c;    // Write character

    // update the head pointer
    _tx_buffer->head = i;


    __HAL_UART_ENABLE_IT(uart, UART_IT_TXE); // Enable UART transmission interrupt

}










