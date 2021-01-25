
#include "BasicRenderer.h"

namespace Visionizer
{

	BasicRenderer::BasicRenderer(Framebuffer* targetFrameBuffer, PSF1_FONT* psf1_font)
	{
		TargetFramebuffer = targetFrameBuffer;
		PSF1_Font = psf1_font;
		Colour = 0xffffffff;
		CursorPosition = {0, 0};
	}

	// Prints a character to a certain position on the screen
	void BasicRenderer::PutChar(char chr, unsigned int xOff, unsigned int yOff)
	{
	    unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
	    char* fontPtr = (char*) PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize);
	    for (unsigned long y = yOff; y < yOff + 16; y++)
	    {
	        for (unsigned long x = xOff; x < xOff+8; x++)
	        {
	            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
	            {
	            	*(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanline)) = Colour;
	            }

	        }
	        fontPtr++;
	    }
	}


	Point CursorPosition;

	// Prints to the screen
	void BasicRenderer::Print(const char* str)
	{
		// After a line it should automatically add a \n and \r (!) TODO
		char* chr = (char*) str;
		while(*chr != 0)
		{
			PutChar(*chr, CursorPosition.X, CursorPosition.Y);
			CursorPosition.X+=8;
			// If line full, goto next line
			if(CursorPosition.X + 8 > TargetFramebuffer->Width)
			{
				CursorPosition.X = 0;
				CursorPosition.Y +=16;
			}
			chr++;
		}
	}

}