#include "Panic.h" 
#include "BasicRenderer.h"


namespace Visionizer
{

    void Panic(const char* panicTitle, const char* panicMessage)
    {   
        GlobalRenderer->Clear(0x00ff0000);

        GlobalRenderer->SetCursorToCenterOfScreen();

        GlobalRenderer->Colour = 0;
        GlobalRenderer->Print("PANIC | PANIC | PANIC | PANIC | PANIC | PANIC | PANIC");
        GlobalRenderer->NextLineCenterOfScreen();
        GlobalRenderer->Print("The Kernel has paniced. This means that something has gone wrong. The following error has occurred: ");

        GlobalRenderer->NextLineCenterOfScreen();
        GlobalRenderer->NextLineCenterOfScreen();

        GlobalRenderer->Print(panicTitle);
        GlobalRenderer->NextLineCenterOfScreen();
        GlobalRenderer->Print(panicMessage);

        GlobalRenderer->NextLineCenterOfScreen();
        GlobalRenderer->NextLineCenterOfScreen();

        GlobalRenderer->Print("Please restart your computer.");
        GlobalRenderer->NextLineCenterOfScreen();
        GlobalRenderer->Print("If the error occurs again, please go to the FAQ (faq.cletyxo.com/errors) and try to fix your error using them.");
        GlobalRenderer->NextLineCenterOfScreen();
        GlobalRenderer->Print("If you cannot solve your problem, ask your question on the official forums (forum.cletyxo.com) ");
    }

}