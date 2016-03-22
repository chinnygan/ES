#ifndef BUTTON_H__
#define BUTTON_H__

#define BUTTON_TOP 0
#define BUTTON_BOTTOM 1

#define BUTTON_TOP_BIT (1 << 0)
#define BUTTON_BOTTOM_BIT (1 << 26)

extern void BUTTON_init();
extern bool BUTTON_value(int BUTTON_ID);

#endif
