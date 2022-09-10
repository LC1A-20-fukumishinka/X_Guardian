#include"DxLib.h"
#include"Game.h"

enum class Mode
{
	Setting,
	Moving,
};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	Game game;

	game.Init();
	//�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		game.Update();
		//�`��

		game.Draw();
		//�t���b�v����
		ScreenFlip();
		WaitTimer(20);

	}
	game.Finalize();
	DxLib_End();
	return 0;
}


