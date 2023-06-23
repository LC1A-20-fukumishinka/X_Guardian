#include "GameInput.h"
#include "Input.h"
void GameInput::Update()
{
	Input::Update();
}

bool GameInput::Straight(GameNum num)
{
	switch (num)
	{
	case GameNum::SOLO:
		return (Input::isKey(KEY_INPUT_W) || Input::isKey(KEY_INPUT_UP) || Input::isPad(XINPUT_BUTTON_DPAD_UP) || Input::isPad(XINPUT_BUTTON_Y) || Input::isPadThumb(XINPUT_THUMB_LEFTVERT) >= 0.5f);
		break;
	case GameNum::PLAYER1:
		return (Input::isKey(KEY_INPUT_W));
		break;
	case GameNum::PLAYER2:
		return (Input::isKey(KEY_INPUT_UP));
		break;
	default:
		break;
	}	return false;
}

bool GameInput::TurnRight(GameNum num)
{
	switch (num)
	{
	case GameNum::SOLO:
		return (Input::isKey(KEY_INPUT_D) || Input::isKey(KEY_INPUT_RIGHT) || Input::isPad(XINPUT_BUTTON_DPAD_RIGHT) || Input::isPad(XINPUT_BUTTON_B) || Input::isPadThumb(XINPUT_THUMB_LEFTSIDE) >= 0.5f);

		break;
	case GameNum::PLAYER1:
		return (Input::isKey(KEY_INPUT_D));
		break;
	case GameNum::PLAYER2:
		return ( Input::isKey(KEY_INPUT_RIGHT));
		break;
	default:
		break;
	}
	return false;
}

bool GameInput::Pause(GameNum num)
{
	switch (num)
	{
	case GameNum::SOLO:

		break;
	case GameNum::PLAYER1:
		break;
	case GameNum::PLAYER2:
		break;
	default:
		break;
	}
	return (Input::isKeyTrigger(KEY_INPUT_ESCAPE) || Input::isPad(XINPUT_BUTTON_START));
}

bool GameInput::Done(GameNum num)
{
	switch (num)
	{
	case GameNum::SOLO:
		return (Input::isKeyTrigger(KEY_INPUT_SPACE) || Input::isKeyTrigger(KEY_INPUT_RETURN) || Input::isPad(XINPUT_BUTTON_A));
		break;
		
	case GameNum::PLAYER1:
		return (Input::isKeyTrigger(KEY_INPUT_SPACE) || Input::isKeyTrigger(KEY_INPUT_RETURN));
		break;
	case GameNum::PLAYER2:
		return (Input::isKeyTrigger(KEY_INPUT_SPACE) || Input::isKeyTrigger(KEY_INPUT_RETURN));
		break;
	default:
		break;
	}
	return false;
}

bool GameInput::ChangeMode()
{
	return Input::isKeyTrigger(KEY_INPUT_1);
}
