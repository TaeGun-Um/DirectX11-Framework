#include "PrecompileHeader.h"
#include <Windows.h>
#include <GameEngineBase\GameEngineFile.h>
#include <GameEngineBase\GameEngineDirectory.h>
#include <GameEngineBase\GameEngineRandom.h>
#include <GameEngineCore\GameEngineCore.h>
#include <GameEngineContents\ContentsCore.h>

#pragma comment(lib, "GameEngineCore.lib")
#pragma comment(lib, "GameEngineContents.lib")

#pragma comment(lib, "zlib-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "libfbxsdk-md.lib")


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	static float Gau[7][7] =
	{
		{ 0.000f, 0.000f, 0.001f, 0.001f, 0.001f, 0.000f, 0.000f },
		{ 0.000f, 0.002f, 0.012f, 0.002f, 0.012f, 0.002f, 0.000f },
		{ 0.001f, 0.012f, 0.068f, 0.109f, 0.068f, 0.012f, 0.001f },
		{ 0.001f, 0.020f, 0.109f, 0.172f, 0.109f, 0.020f, 0.001f },
		{ 0.001f, 0.012f, 0.068f, 0.109f, 0.068f, 0.012f, 0.001f },
		{ 0.000f, 0.002f, 0.012f, 0.020f, 0.012f, 0.002f, 0.000f },
		{ 0.000f, 0.000f, 0.001f, 0.001f, 0.001f, 0.000f, 0.000f },
	};

	float Value = 0.0f;

	for (size_t i = 0; i < 7; i++)
	{
		for (size_t j = 0; j < 7; j++)
		{
			Value += Gau[i][j];
		}
	}
	


	GameEngineCore::Start(hInstance, 
		ContentsCore::GameStart, 
		ContentsCore::GameEnd,
		{0, 0}
	);
}