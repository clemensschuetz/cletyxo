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
		unsigned int DefaultClearColour;

		void Print(const char* str); // TODO Maybe add colour param to this, but default it to colour;

		void PutChar(char chr, unsigned int xOff, unsigned int yOff);
		void PutChar(char chr);

		// Sets the screen to a clear color
		void ClearColour(uint32_t colour);

		// Deletes the last character
		void ClearChar();

		// Clears the screen
		void Clear();

		// Goes to NextLine
		void NextLine();

		// Deletes a character

		// Sets cursor to middle of screen
		void SetCursorToCenterOfScreen();
		// Next Line, but it stays in the middle of the screen
		void NextLineCenterOfScreen();

	};

	extern BasicRenderer* GlobalRenderer;
}
