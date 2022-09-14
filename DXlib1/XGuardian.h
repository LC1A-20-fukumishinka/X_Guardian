#pragma once
#include "Vector3.h"
#include "carManager.h"

class XGuardian
{
private:
	//メンバ変数
	Vector3 centerPos_;			//座標
	int XGuardianModelHandle_;		//十字路の守護神本体のモデルハンドル
	int XGuardiansStopModelHandle_;//十字路の守護神の盾(停止)のモデルハンドル
	int XGuardiansStraightModelHandle_;//十字路の守護神の盾(直進)のモデルハンドル
	int XGuardiansTurnRightModelHandle_;//十字路の守護神の盾(右折)のモデルハンドル
	int var;

public:
	MoveType inputSignal_;					//入力情報

	void LoadModel();


	//メンバ関数

	//コンストラクタ
	XGuardian();
	//デストラクタ
	~XGuardian();
	//初期化
	void Init();
	//更新
	void Update(MoveType inputSignal);
	//終了
	void Finalize();
	//描画
	void Draw();
};