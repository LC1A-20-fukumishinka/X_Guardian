#pragma once
#include "DxLib.h"
#include "Vector3.h"

//MATRIX 型を MATRIX4 型として扱う ※互換性確保&拡張のため
using Matrix4 = MATRIX;

//単位行列を求める
Matrix4 identity();

//拡大縮小行列の設定
Matrix4 scale(const Vector3 &s);

//回転行列の設定
Matrix4 rotationX(float angle);
Matrix4 rotationY(float angle);
Matrix4 rotationZ(float angle);

//平行移動
Matrix4 translate(const Vector3 &t);

//座標変換(ベクトルと行列の掛け算)
Vector3 transform(const Vector3 &v, const Matrix4 &m);

//代入演算所のオーバーロード
Matrix4 &operator *=(Matrix4 &m1, const Matrix4 &m2);

//2項演算子オーバーロード
Matrix4 operator * (const Matrix4 &m1, const Matrix4 &m2);
Vector3 operator * (const Vector3 &v, const Matrix4 &m);