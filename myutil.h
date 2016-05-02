#ifndef MYUTIL_H
#define MYUTIL_H

#include <windows.h>
#include <windef.h>
#include <winuser.h>
#include <interception.h>
#include "imageprocessing.h"

enum ScanCode
{
    SCANCODE_TAB = 0x0F,
    SCANCODE_ESC = 0x01
};


wchar_t *convertCharArrayToLPCWSTR(const char* charArray);

void raise_process_priority(void);
void lower_process_priority(void);


void PressKey(InterceptionContext &context, InterceptionDevice &device, InterceptionKeyStroke &stroke, NoteType key);

void ReleaseKey(InterceptionContext &context, InterceptionDevice &device, InterceptionKeyStroke &stroke, NoteType key);



#endif // MYUTIL_H
