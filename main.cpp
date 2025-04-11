#include <Novice.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

const char kWindowTitle[] = "LE2C_18_タナベ_カズマ_MT3_00_01";

struct Vector3 {
	float x, y, z;
};

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {

	Vector3 result = {};

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
};

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 result = {};

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
};

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result = {};

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;

	return result;
};

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {

	float result = {};

	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return result;
};

// 長さ
float Length(const Vector3& v) {

	float result = {};

	result = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	return result;
};

// 正規化
Vector3 Nomalize(const Vector3& v) {

	Vector3 result = {};

	result.x = v.x / Length(v);
	result.y = v.y / Length(v);
	result.z = v.z / Length(v);

	return result;
};

//
static const int kColumnWidth = 60;

// 結果を表示する関数
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 v1{ 1.0f,3.0f,-5.0f };
	Vector3 v2{ 4.0f,-1.0f,2.0f };
	float k = { 4.0f };

	// v1とv2を加算
	Vector3 resultAdd = Add(v1, v2);

	// v1とv2を減算
	Vector3 resultSubtract = Subtract(v1, v2);

	//v1をk倍
	Vector3 resultMultiply = Multiply(k, v1);

	// v1とv2を内積
	float resultDot = Dot(v1, v2);

	// v1の長さ
	float resultLength = Length(v1);

	// v2を正規化
	Vector3 resultNomalize = Nomalize(v2);

	// 表示する文字のy座標の幅
	int kRowHeight = 15;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 加算を表示
		VectorScreenPrintf(0, 0, resultAdd, " : Add");

		// 減算を表示
		VectorScreenPrintf(0, kRowHeight, resultSubtract, " : Subtract");

		// スカラー倍を表示
		VectorScreenPrintf(0, kRowHeight * 2, resultMultiply, " : Multiply");

		// 内積を表示
		Novice::ScreenPrintf(0, kRowHeight * 3, "%0.2f : Dot", resultDot);

		// 長さを表示
		Novice::ScreenPrintf(0, kRowHeight * 4, "%0.2f : Length", resultLength);

		// 内積を表示
		VectorScreenPrintf(0, kRowHeight * 5, resultNomalize, " : Nomalize");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
