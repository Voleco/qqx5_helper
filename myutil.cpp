
#include "myutil.h"

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
{
    wchar_t* wString=new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

void raise_process_priority(void)
{
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
}

void lower_process_priority(void)
{
    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
}

void PressKey(InterceptionContext& context,InterceptionDevice& device,InterceptionKeyStroke& stroke, NoteType key)
{
    stroke.information =0;
    switch (key) {
    case NOTE_RIGHT:
        stroke.code = 0x4D;
        stroke.state = INTERCEPTION_KEY_DOWN | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"right";
        break;
    case NOTE_UP:
        stroke.code = 0x48;
        stroke.state = INTERCEPTION_KEY_DOWN | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"up";
        break;
    case NOTE_LEFT:
        stroke.code = 0x4B;
        stroke.state = INTERCEPTION_KEY_DOWN | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"left";
        break;
    case NOTE_DOWN:
        stroke.code = 0x50;
        stroke.state = INTERCEPTION_KEY_DOWN | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"down";
        break;
    default:
        break;
    }
}

void ReleaseKey(InterceptionContext &context, InterceptionDevice &device, InterceptionKeyStroke &stroke, NoteType key)
{
    stroke.information =0;
    switch (key) {
    case NOTE_RIGHT:
        stroke.code = 0xCD;
        stroke.state = INTERCEPTION_KEY_UP | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"right";
        break;
    case NOTE_UP:
        stroke.code = 0xC8;
        stroke.state = INTERCEPTION_KEY_UP | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"up";
        break;
    case NOTE_LEFT:
        stroke.code = 0xCB;
        stroke.state = INTERCEPTION_KEY_UP | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"left";
        break;
    case NOTE_DOWN:
        stroke.code = 0xD1;
        stroke.state = INTERCEPTION_KEY_UP | INTERCEPTION_KEY_E0;
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
        qDebug()<<"down";
        break;
    default:
        break;
    }
}
