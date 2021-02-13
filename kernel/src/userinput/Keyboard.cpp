#include "Keyboard.h" 


namespace Visionizer
{



    bool isLeftShiftPressed;
    bool isRightShiftPressed;
    void HandleKeyboard(uint8_t scancode)
    {

        // Any special modifiers?
        // Add 0x80 to get the released-counter-part of the key (Pressed - Released)
        switch (scancode)
        {
            case LEFT_SHIFT: 
                isLeftShiftPressed = true;
                return;
            case LEFT_SHIFT + 0x80: 
                isLeftShiftPressed = false;
                return;

            case RIGHT_SHIFT:
                isRightShiftPressed = true;
                return;
            case RIGHT_SHIFT + 0x80:
                isRightShiftPressed = false;
                return;
            case ENTER:
                GlobalRenderer->NextLine();
                return;   
            case SPACEBAR:
                GlobalRenderer->PutChar(' ');
                return;

            case BACKSPACE:
                GlobalRenderer->ClearChar();
                break;
        }   

        char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);
        if (ascii != 0)
        {
            GlobalRenderer->PutChar(ascii);
        }
    }

//   void SetKeyboardLayout(KeyboardLayouts newLayout)
//    {
//        info->layout = newLayout;
//    }

}