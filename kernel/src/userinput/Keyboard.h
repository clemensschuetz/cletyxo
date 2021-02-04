#pragma once
#include <stdint.h>
#include "kbScancodeTranslation.h"
#include "../BasicRenderer.h"

namespace Visionizer
{

    enum KeyboardLayouts
    {
        QWERTY,
        GermanQWERTZ,
    };


    void HandleKeyboard(uint8_t scancode);


}