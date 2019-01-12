#include "DX9MapEditor.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

using namespace DX9ENGINE;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	static DX9MapEditor g_MapEditor;

	if (DX_FAILED(g_MapEditor.Create(800, 600)))
		return -1;

	g_MapEditor.LoadMap(L"map01.jwm");

	g_MapEditor.Run();

	return 0;
}