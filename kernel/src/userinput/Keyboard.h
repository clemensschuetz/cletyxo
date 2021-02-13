#pragma once
#include <stdint.h>
#include "kbScancodeTranslation.h"
#include "../BasicRenderer.h"
#include "../Core.h"

namespace Visionizer
{

//    enum KeyboardLayouts
//    {
//        QWERTY,
//        GermanQWERTZ,
//    };

//    struct KeyboardInformation
//    {
//        KeyboardLayouts layout;
//    };

    void HandleKeyboard(uint8_t scancode);
//    void SetKeyboardLayout(KeyboardLayouts newLayout);

	// Globals
// 	extern KeyboardInformation* info;
}