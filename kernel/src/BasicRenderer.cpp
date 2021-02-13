
#include "BasicRenderer.h"

namespace Visionizer
{

	BasicRenderer* GlobalRenderer;

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

	void BasicRenderer::PutChar(char chr)
	{
		PutChar(chr, CursorPosition.X, CursorPosition.Y);
		CursorPosition.X += 8;
		if (CursorPosition.X + 8 > TargetFramebuffer->Width)
		{
			CursorPosition.X = 0;
			CursorPosition.Y += 16;
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


		
	void BasicRenderer::Clear()
	{
		uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
		uint64_t bytesPerScanline = (uint64_t)TargetFramebuffer->PixelsPerScanline * 4; // One Pixel has 4 bytes
		uint64_t fbHeight = (uint64_t)TargetFramebuffer->Height;
		uint64_t fbSize = (uint64_t)TargetFramebuffer->BufferSize;


		for (int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++)
		{
			uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline); // Points to the first pixel in every row
			for (uint32_t* pixPtr /* Pixel Pointer */ = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++)
			{
				*pixPtr = DefaultClearColour;
			}
		}
	}



	void BasicRenderer::ClearColour(uint32_t colour)
	{
		uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
		uint64_t bytesPerScanline = (uint64_t)TargetFramebuffer->PixelsPerScanline * 4; // One Pixel has 4 bytes
		uint64_t fbHeight = (uint64_t)TargetFramebuffer->Height;
		uint64_t fbSize = (uint64_t)TargetFramebuffer->BufferSize;


		for (int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline++)
		{
			uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline); // Points to the first pixel in every row
			for (uint32_t* pixPtr /* Pixel Pointer */ = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr++)
			{
				*pixPtr = colour;
			}
		}
	}


	void BasicRenderer::ClearChar(){

		if (CursorPosition.X == 0){
			CursorPosition.X = TargetFramebuffer->Width;
			CursorPosition.Y -= 16;
			if (CursorPosition.Y < 0) CursorPosition.Y = 0;
		}

		unsigned int xOff = CursorPosition.X;
		unsigned int yOff = CursorPosition.Y;

		unsigned int* pixPtr = (unsigned int*)TargetFramebuffer->BaseAddress;
		for (unsigned long y = yOff; y < yOff + 16; y++){
			for (unsigned long x = xOff - 8; x < xOff; x++){
						*(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanline)) = DefaultClearColour;
			}
		}

		CursorPosition.X -= 8;

		if (CursorPosition.X < 0){
			CursorPosition.X = TargetFramebuffer->Width;
			CursorPosition.Y -= 16;
			if (CursorPosition.Y < 0) CursorPosition.Y = 0;
		}

	}
	void BasicRenderer::NextLine()
	{
		CursorPosition.X = 0;
		CursorPosition.Y += 16;
	}


	// Sets cursor to middle of screen
	void BasicRenderer::SetCursorToCenterOfScreen()
	{
		unsigned int halfWidth =  GlobalRenderer->TargetFramebuffer->Width / 2;
        unsigned int halfHeight = GlobalRenderer->TargetFramebuffer->Height / 2;

		GlobalRenderer->CursorPosition = {halfWidth, halfHeight};
	}

	// Next Line, but it stays in the middle of the screen
	void BasicRenderer::NextLineCenterOfScreen()
	{
		unsigned int halfWidth = TargetFramebuffer->Width / 2;
        
		CursorPosition = {halfWidth, CursorPosition.Y + 16};
		return;
	}

	void BasicRenderer::PutPixel(uint32_t x, uint32_t y, uint32_t colour)
	{
		*(uint32_t*)((uint64_t)TargetFramebuffer->BaseAddress + (x*4) + (y * TargetFramebuffer->PixelsPerScanline * 4)) = colour;
	}

	uint32_t BasicRenderer::GetPixel(uint32_t x, uint32_t y)
	{
		return *(uint32_t*)((uint64_t)TargetFramebuffer->BaseAddress + (x*4) + (y * TargetFramebuffer->PixelsPerScanline * 4));
	}
	
	void BasicRenderer::ClearMouseCursor(uint8_t* mouseCursor, Point position)
	{
		if (!IsMouseDrawn) return;



		int xMax = 16;
		int yMax = 16;
		int differenceX = TargetFramebuffer->Width - position.X;
		int differenceY = TargetFramebuffer->Height - position.Y;

		if (differenceX < 16) xMax = differenceX;
		if (differenceY < 16) yMax = differenceY;

		for (int y = 0; y < yMax; y++)
		{
			for (int x = 0; x < xMax; x++)
			{
				int bit = y * 16 + x;
				int byte = bit / 8;
				if ((mouseCursor[byte] & (0b10000000 >> (x & 8))))
				{
					if (GetPixel(position.X  + x, position.Y + y) == MouseCursorBufferAfter[x + y * 16])
					{
						PutPixel(position.X + x, position.Y + y, MouseCursorBuffer[x,y * 16]);						
					}
				}
			}
		}
	}

	void BasicRenderer::DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t colour)
	{
		int xMax = 16;
		int yMax = 16;
		int differenceX = TargetFramebuffer->Width - position.X;
		int differenceY = TargetFramebuffer->Height - position.Y;

		if (differenceX < 16) xMax = differenceX;
		if (differenceY < 16) yMax = differenceY;

		for (int y = 0; y < yMax; y++)
		{
			for (int x = 0; x < xMax; x++)
			{
				int bit = y * 16 + x;
				int byte = bit / 8;
				if ((mouseCursor[byte] & (0b10000000 >> (x & 8))))
				{
					MouseCursorBuffer[x + y * 16] = GetPixel(position.X + x, position.Y + y);
					PutPixel(position.X + x, position.Y + y, colour);
					MouseCursorBufferAfter[x + y * 16] = GetPixel(position.X + x, position.Y + y);
				}
			}
		}
		IsMouseDrawn = true;
	}
}