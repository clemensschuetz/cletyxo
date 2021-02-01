#include "Interrupts.h" 


namespace Visionizer
{

    
    __attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame)
    {
        GlobalRenderer->Print("Page fault detected");
        while(true);
    }

}