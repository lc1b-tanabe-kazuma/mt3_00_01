#include <Novice.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

const char kWindowTitle[] = "LE2C_19_タナベ_カズマ_MT3_00_03";

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;
};

// 行列の加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
};

// 行列の減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
};

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
};

// 逆行列
// 3x3行列式（余因子計算用）
float Det3(float a1, float a2, float a3,
	float b1, float b2, float b3,
	float c1, float c2, float c3) {
	return a1 * (b2 * c3 - b3 * c2)
		- a2 * (b1 * c3 - b3 * c1)
		+ a3 * (b1 * c2 - b2 * c1);
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result = {};

	float det =
		m.m[0][0] * Det3(m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]) -
		m.m[0][1] * Det3(m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]) +
		m.m[0][2] * Det3(m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]) -
		m.m[0][3] * Det3(m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]);

	if (det == 0.0f) {
		// 行列が正則でない場合、単位行列または0行列などを返してもOK
		return result;
	}

	float invDet = 1.0f / det;

	// 余因子 + 転置で逆行列を一気に展開（各要素ベタ書き）
	result.m[0][0] = Det3(m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]) * invDet;
	result.m[1][0] = -Det3(m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]) * invDet;
	result.m[2][0] = Det3(m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]) * invDet;
	result.m[3][0] = -Det3(m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]) * invDet;

	result.m[0][1] = -Det3(m.m[0][1], m.m[0][2], m.m[0][3], m.m[2][1], m.m[2][2], m.m[2][3], m.m[3][1], m.m[3][2], m.m[3][3]) * invDet;
	result.m[1][1] = Det3(m.m[0][0], m.m[0][2], m.m[0][3], m.m[2][0], m.m[2][2], m.m[2][3], m.m[3][0], m.m[3][2], m.m[3][3]) * invDet;
	result.m[2][1] = -Det3(m.m[0][0], m.m[0][1], m.m[0][3], m.m[2][0], m.m[2][1], m.m[2][3], m.m[3][0], m.m[3][1], m.m[3][3]) * invDet;
	result.m[3][1] = Det3(m.m[0][0], m.m[0][1], m.m[0][2], m.m[2][0], m.m[2][1], m.m[2][2], m.m[3][0], m.m[3][1], m.m[3][2]) * invDet;

	result.m[0][2] = Det3(m.m[0][1], m.m[0][2], m.m[0][3], m.m[1][1], m.m[1][2], m.m[1][3], m.m[3][1], m.m[3][2], m.m[3][3]) * invDet;
	result.m[1][2] = -Det3(m.m[0][0], m.m[0][2], m.m[0][3], m.m[1][0], m.m[1][2], m.m[1][3], m.m[3][0], m.m[3][2], m.m[3][3]) * invDet;
	result.m[2][2] = Det3(m.m[0][0], m.m[0][1], m.m[0][3], m.m[1][0], m.m[1][1], m.m[1][3], m.m[3][0], m.m[3][1], m.m[3][3]) * invDet;
	result.m[3][2] = -Det3(m.m[0][0], m.m[0][1], m.m[0][2], m.m[1][0], m.m[1][1], m.m[1][2], m.m[3][0], m.m[3][1], m.m[3][2]) * invDet;

	result.m[0][3] = -Det3(m.m[0][1], m.m[0][2], m.m[0][3], m.m[1][1], m.m[1][2], m.m[1][3], m.m[2][1], m.m[2][2], m.m[2][3]) * invDet;
	result.m[1][3] = Det3(m.m[0][0], m.m[0][2], m.m[0][3], m.m[1][0], m.m[1][2], m.m[1][3], m.m[2][0], m.m[2][2], m.m[2][3]) * invDet;
	result.m[2][3] = -Det3(m.m[0][0], m.m[0][1], m.m[0][3], m.m[1][0], m.m[1][1], m.m[1][3], m.m[2][0], m.m[2][1], m.m[2][3]) * invDet;
	result.m[3][3] = Det3(m.m[0][0], m.m[0][1], m.m[0][2], m.m[1][0], m.m[1][1], m.m[1][2], m.m[2][0], m.m[2][1], m.m[2][2]) * invDet;

	return result;
}

// 転置行列
Matrix4x4 Transepose(const Matrix4x4& m) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
};

// 単位行列の作成
Matrix4x4 MakeIndetity4x4() {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1.0f;
			} else {
				result.m[i][j] = 0.0f;
			}
		}
	}
	return result;
};

// 平行移動行列
Matrix4x4 MakeTransMatrix(const Vector3& v ){
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	result.m[3][0] = v.x;
	result.m[3][1] = v.y;
	result.m[3][2] = v.z;
	return result;
}

//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& v) {
	Matrix4x4 result = {};
	result.m[0][0] = v.x;
	result.m[1][1] = v.y;
	result.m[2][2] = v.z;
	result.m[3][3] = 1.0f;
	return result;
}

// 座標変換
Vector3 Transform(const Matrix4x4& m, const Vector3& v) {
	Vector3 result = {};
	result.x = m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0];
	result.y = m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1];
	result.z = m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2];
	return result;
}

//
static const int kColumnWidth = 60;

//
static const int kRowHeight = 20;

// 4x4の結果を表示する関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {

	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int colum = 0; colum < 4; ++colum) {
			Novice::ScreenPrintf(x + colum * kColumnWidth, y + (row + 1) * kRowHeight, "%6.2f", m.m[row][colum]);
		}

	}
}

// Vectorの結果を表示する関数
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
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 tranlate = { 4.1f,2.6f,0.8f };

	Vector3 scale = { 1.5f,5.2f,7.3f };

	Matrix4x4 transelatematrix = MakeTransMatrix(tranlate);

	Matrix4x4 scalematrix = MakeScaleMatrix(scale);

	Vector3 point{ 2.3f,3.8f,1.4f };

	Matrix4x4 transforMatrix = {
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f
	};

	Vector3 transformed = Transform(transforMatrix,point);

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

		VectorScreenPrintf(0, 0, transformed, "transformMatrix");

		MatrixScreenPrintf(0,kRowHeight*2,transelatematrix,"transelateMatrix");

		MatrixScreenPrintf(0, kRowHeight * 5*2, scalematrix, "scaleMatrix");

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
