#include "Mouse.h" 


namespace Visionizer
{

    
    // Waits
    void MouseWait()
    {
        uint64_t timeout = 100000; // Wait for 100k loop cycles
        while (timeout--)
        {
            if ((inb(0x64) & 0b10) == 0) return;            
        }
    }
    // Waits for input
    void MouseWaitInput()
    {
        uint64_t timeout = 100000; // Wait for 100k loop cycles
        while (timeout--)
        {
            if (inb(0x64) & 0b1) return;            
        }
    }

    // Reads the mouse | Can be used for acknowledgment of  mouse reading
    uint8_t  MouseRead()
    {
        MouseWaitInput();
        return inb(0x60);
    }


    // Sends cmd to mouse | 0xF6 are the default settings
    void MouseWrite(uint8_t value)
    {
        MouseWait();
        outb(0x64, 0xD4); // Telling mouse that we are going to send a cmd
        MouseWait();
        outb(0x60, value);
    }


    uint8_t MouseCycle = 0;
    uint8_t MousePacket[4];
    bool MousePacketReady = false;
    Point MousePosition;

    // Fills the packet with data
    void HandlePS2Mouse(uint8_t data)
    {
        switch(MouseCycle)
        {
            case 0: 
                if (MousePacketReady) break;
                if (data & 0b00001000 == 0/* Always has to be 1 */)  break;
                MousePacket[0] = data;
                MouseCycle++;
                break;
            case 1:
                if (MousePacketReady) break;
                MousePacket[1] = data;
                MouseCycle++;
                break;
            case 2:
                if (MousePacketReady) break;
                MousePacket[2] = data;
                MousePacketReady = true;
                MouseCycle = 0;
                break;
        }
    }


    void ProcessMousePacket()
    {
    if (!MousePacketReady) return;

        bool xNegative, yNegative, xOverflow, yOverflow;

        if (MousePacket[0] & PS2XSign){
            xNegative = true;
        }else xNegative = false;

        if (MousePacket[0] & PS2YSign){
            yNegative = true;
        }else yNegative = false;

        if (MousePacket[0] & PS2XOverflow){
            xOverflow = true;
        }else xOverflow = false;

        if (MousePacket[0] & PS2YOverflow){
            yOverflow = true;
        }else yOverflow = false;

        if (!xNegative){
            MousePosition.X += MousePacket[1];
            if (xOverflow){
                MousePosition.X += 255;
            }
        } else
        {
            MousePacket[1] = 256 - MousePacket[1];
            MousePosition.X -= MousePacket[1];
            if (xOverflow){
                MousePosition.X -= 255;
            }
        }

        if (!yNegative){
            MousePosition.Y -= MousePacket[2];
            if (yOverflow){
                MousePosition.Y -= 255;
            }
        } else
        {
            MousePacket[2] = 256 - MousePacket[2];
            MousePosition.Y += MousePacket[2];
            if (yOverflow){
                MousePosition.Y += 255;
            }
        }

        if (MousePosition.X < 0) MousePosition.X = 0;
        if (MousePosition.X > GlobalRenderer->TargetFramebuffer->Width-8) MousePosition.X = GlobalRenderer->TargetFramebuffer->Width-8;
        
        if (MousePosition.Y < 0) MousePosition.Y = 0;
        if (MousePosition.Y > GlobalRenderer->TargetFramebuffer->Height-16) MousePosition.Y = GlobalRenderer->TargetFramebuffer->Height-16;
        
        GlobalRenderer->PutChar('a', MousePosition.X, MousePosition.Y);

        MousePacketReady = false;
    }


    void InitPS2Mouse()
    {
        outb(0x64, 0xA8); //enabling the auxiliary device - mouse

        MouseWait();
        outb(0x64, 0x20); //tells the keyboard controller that we want to send a command to the mouse
        MouseWaitInput();
        uint8_t status = inb(0x60);
        status |= 0b10;
        MouseWait();
        outb(0x64, 0x60);
        MouseWait();
        outb(0x60, status); // setting the correct bit is the "compaq" status byte

        MouseWrite(0xF6);
        MouseRead();

        MouseWrite(0xF4);
        MouseRead();

    }


}