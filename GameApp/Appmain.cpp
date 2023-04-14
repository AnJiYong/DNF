#include "PreCompile.h"

#include <conio.h>
#include "GameEngine/GameEngineCore.h"
#include "UserGame.h"



int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	GameEngineCore::Start<UserGame>();
}