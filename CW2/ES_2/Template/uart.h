#ifndef UART_H__
#define UART_H__

extern void uartInit();
extern char uartGetchar(UART_MemMapPtr uartChannel);
extern void uartPutchar(UART_MemMapPtr uartChannel, char ch);
extern int uartGetcharPresent(UART_MemMapPtr uartChannel);
extern void uartPuts(UART_MemMapPtr uartChannel, char *p);

#endif
