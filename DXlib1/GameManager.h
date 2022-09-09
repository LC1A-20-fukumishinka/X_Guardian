#pragma once
class GameManager
{
public:
GameManager();
~GameManager();
void Init();
void Update();
void Finalize();
void Draw();

void PassCar();

void StopCar();

public: 
const int baseScore = 100;
private:

int score = 0;
int combo = 0;


};

