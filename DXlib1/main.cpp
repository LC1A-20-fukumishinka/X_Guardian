#include"DxLib.h"
#include "playManager.h"
enum class Mode
{
	Setting,
	Moving,
};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	playManager game;

	game.Init();


	//ゲームループ
	while (ProcessMessage() == 0 && !game.GameEnd())
	{
		game.Update();

		//描画
		game .Draw();

		//フリップする
		ScreenFlip();
		WaitTimer(20);

	}
	game.Finalize();
	DxLib_End();
	return 0;
}


