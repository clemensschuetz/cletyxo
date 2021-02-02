#pragma once
#include "Math.h"
#include "Framebuffer.h"
#include "Font.h"
#include <stdint.h>

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

		// Sets the screen to a clear color
		void Clear(uint32_t colour);
		// Goes to NextLine
		void NextLine();

		// Sets cursor to middle of screen
		void SetCursorToCenterOfScreen();
		// Next Line, but it stays in the middle of the screen
		void NextLineCenterOfScreen();

	};

	extern BasicRenderer* GlobalRenderer;
}
