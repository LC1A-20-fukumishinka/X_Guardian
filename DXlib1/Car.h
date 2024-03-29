#pragma once
#include "Vector3.h"
#include "Primitive.h"
#include <memory>
#include "SoundManager.h"

enum class MoveType
{
	STRAIGHT,
	RIGHTTURN,
	STOP,
	ALLOK,
};
enum class ModelType
{
	NORMAL,
	TRACK,
	EMERGENCY,
};

enum class Color
{
	BLUE,
	GREEN,
	PINK,
	PURPLE,
	YELLOW,
	RANDUM,
};
struct CarInitializeDesc
{
	Vector3 startPos = Vector3();
	Vector3 angle = Vector3(0, 0, 1.0f);
	float length = 10.0f;
	float radius = 3.0f;
	float speed = 0.05f;
	bool isPlayer = true;
	MoveType type = MoveType::STRAIGHT;
	ModelType model = ModelType::NORMAL;
	Color color = Color::RANDUM;
};
class Car
{
public:

	Car();
	~Car();
	void Init(Vector3 startPos, Vector3 angle, float length, float radius, float speed, bool isPlayer, MoveType type);
	void Init(CarInitializeDesc desc);
	void Update();
	void Finalize();
	void Draw();

	bool GetIsAlive();

	Vector3 GetTailPos();

	Vector3 GetFrontPos();

	Vector3 GetCenterPos();

	MoveType GetMoveType();

	ModelType GetModelType();

	Capsule *GetCapsule();
	void SetFrontCar(std::weak_ptr<Car> frontCar);

	void SetBackCar(std::weak_ptr<Car> backCar);


	void Dead();
	void Count();

	int GetCarColor();

	//通過しきったフラグ
	bool GetIsPass();

	bool GetIsSignalStop();

	bool GetIsFront();

	void ArrowSubstantiation();
private:
	static const float sTurnStartPos;
	static const float sStopPos;
	static const float sCarDistanceLimit;
	static const float sStopLength;
	static const float sPassWidth;
	static const float sEraseWidth;
	static const float sEraseDepth;
	static MoveType sInputSignal;
	static const int sMaxEnemyStopTimer;
	static const int sMaxDerayTimer;
	static SoundManager *sSounds;
	//Modelハンドル
	static std::vector<int> sNormalCarModelHandles;
	static std::vector<int> sTrackCarModelHandles;
	static int sAmbulanceCarModelHandle;
	static std::vector<int> sArrowModelHandle;
	static float sGameSpeed;

	static float pressAnimationRate;
public:
	static void SetSignal(MoveType isStopSignal);
	static void LoadModel(int arrow, int turnArrow);

	static void SetGameSpeed(float speed);

	static void SetPressAnimationRate(float rate);

	static void SetSoundManager(SoundManager *sounds);
private:

	void CapsuleMove();

	void StraightMove();

	void RightTurnMove();

	bool JudgmentToStop(bool isCrossIn);

	//先頭車両かどうか
	bool isFront = false;
	int enemyStopTimer_ = 60;
	//カプセル状の当たり判定使わないかも
	std::unique_ptr<Capsule> colObject_;
	//車両の先頭部分の座標
	Vector3 frontPos_;
	//車両の方向
	Vector3 angle_;

	Vector3 centerPos_;
	float carLength_;

	float speed_;

	bool isAlive_ = false;
	bool isPlayer_ = true;
	bool isCrossIn_ = false;
	bool isCounted_ = false;
	bool isCrossSound_ = false;

	bool isNowStop_ =false;
	bool isOldStop_ = false;
	bool isPlayedSirenSound_ = false;
	int derayTimer_ = 0;

	int color_ = 0;
	MoveType type_ = MoveType::STRAIGHT;
	ModelType model_ = ModelType::NORMAL;

	std::weak_ptr<Car> frontCar_;
	std::weak_ptr<Car> backCar_;


	bool isArrowSubstantiation = false;

	bool isBackArrowSubstantiation = false;
private:
	float clearAnimationRate_ = 0.0f;
};

