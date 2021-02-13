#include "userinput/Keyboard.h"

namespace Visionizer
{
	// In here, all macros, typedef etc that are needed everywhere will be defined
	#define VNONE nullptr // TODO Change
	#define COMPILER_BYTE

	// Compilers // TODO ADdd
	#define COMPILER_GCC 0
	#define COMPILER_CLANG 0
	#define COMPILER_VISUAL 0
	#define COMPILER_VISIONIZER 0

	// TYPEDEFS
	#define TRIGGER_DOUBLE_FAULT() return 0;
	#define TRIGGER_PAGE_FAULT() return 0;
	#define TRIGGER_GP_FAULT() return 0;
}
