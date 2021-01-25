#pragma once
#include "Math.h"
#include "Framebuffer.h"
#include "Font.h"

namespace Visionizer 
{
	class BasicRenderer 
	{
	public:
		BasicRenderer(Framebuffer* targetFrameBuffer, PSF1_FONT* psf1_font); // TODO Make a curPos default

		Point CursorPosition;
		Framebuffer* TargetFramebuffer;
		PSF1_FONT* PSF1_Font;
		unsigned int Colour;

		void Print(const char* str); // TODO Maybe add colour param to this, but default it to colour;
		void PutChar(char chr, unsigned int xOff, unsigned int yOff);

	};
}
