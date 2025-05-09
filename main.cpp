#include <Novice.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>
#include <imgui.h>

const char kWindowTitle[] = "LE2C_19_タナベ_カズマ_MT3_01_01";

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Sphere {

	// 中心座標
	Vector3 center;

	// 半径
	float radius;
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
Matrix4x4 MakeTransMatrix(const Vector3& v) {
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

	// w成分
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	// wが0でない場合、結果をwで割る
	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}
	return result;
}

// X軸の回転行列
Matrix4x4 MakeRotXMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[1][2] = std::sinf(radian);
	result.m[2][1] = -std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

// Y軸の回転行列
Matrix4x4 MakeRotYMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][2] = -std::sinf(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

// Z軸の回転行列
Matrix4x4 MakeRotZMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = std::sinf(radian);
	result.m[1][0] = -std::sinf(radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	// スケーリング行列の作成
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	Matrix4x4 matRotX = MakeRotXMatrix(rotate.x);
	Matrix4x4 matRotY = MakeRotYMatrix(rotate.y);
	Matrix4x4 matRotZ = MakeRotZMatrix(rotate.z);

	// 回転行列の合成
	Matrix4x4 matRot = Multiply(Multiply(matRotY, matRotX), matRotZ);

	// 平行移動行列の作成
	Matrix4x4 matTrans = MakeTransMatrix(translate);

	// スケーリング、回転、平行移動の合成
	Matrix4x4 matTransform = Multiply(Multiply(matScale, matRot), matTrans);

	return matTransform;
}

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};
	float f = 1.0f / std::tanf(fovY / 2.0f);
	result.m[0][0] = (f * (1.0f / aspectRatio));
	result.m[1][1] = f;
	result.m[2][2] = (farClip) / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = -nearClip * farClip / (farClip - nearClip);
	return result;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {};
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[3][0] = (right + left) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = (nearClip) / (nearClip - farClip);
	result.m[3][3] = 1.0f;
	return result;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float mindepth, float maxDepth) {
	Matrix4x4 result = {};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = (maxDepth - mindepth);
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = mindepth;
	result.m[3][3] = 1.0f;
	return result;
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

// Gridを描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {

	const float kGridhalfWidth = 2.0f;
	const uint32_t kSubdvision = 10;
	const float kGridEvery = (kGridhalfWidth * 2.0f) / float(kSubdvision);

	// 奥から手前に描画
	for (uint32_t xIndex = 0; xIndex <= kSubdvision; ++xIndex) {
		float x = -kGridhalfWidth + kGridEvery * float(xIndex);
		Vector3 a = { x, 0.0f, -kGridhalfWidth };
		Vector3 b = { x, 0.0f, kGridhalfWidth };
		Vector3 aScreen = Transform(viewProjectionMatrix, a);
		Vector3 bScreen = Transform(viewProjectionMatrix, b);
		aScreen = Transform(viewportMatrix, aScreen);
		bScreen = Transform(viewportMatrix, bScreen);
		Novice::DrawLine(int(aScreen.x), int(aScreen.y), int(bScreen.x), int(bScreen.y), 0xAAAAAAFF);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdvision; ++zIndex) {
		float z = -kGridhalfWidth + kGridEvery * float(zIndex);
		Vector3 a = { -kGridhalfWidth, 0.0f, z };
		Vector3 b = { kGridhalfWidth, 0.0f, z };
		Vector3 aScreen = Transform(viewProjectionMatrix, a);
		Vector3 bScreen = Transform(viewProjectionMatrix, b);
		aScreen = Transform(viewportMatrix, aScreen);
		bScreen = Transform(viewportMatrix, bScreen);
		Novice::DrawLine(int(aScreen.x), int(aScreen.y), int(bScreen.x), int(bScreen.y), 0xAAAAAAFF);
	}
}

// Sphereを描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 20;
	const float kLoneEvery = float(M_PI * 2.0f / float(kSubdivision));  // 経度1区分
	const float kLatEvery = float(M_PI / float(kSubdivision));         // 緯度1区分

	// === 緯線（横方向） ===
	for (uint32_t latIndex = 1; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI / 2.0f) + kLatEvery * float(latIndex);
		Vector3 prevPoint = {};
		for (uint32_t lonIndex = 0; lonIndex <= kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLoneEvery;

			Vector3 point = {
				sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon)
			};

			Vector3 screenPoint = Transform(viewProjectionMatrix, point);
			screenPoint = Transform(viewportMatrix, screenPoint);

			if (lonIndex > 0) {
				Novice::DrawLine(
					static_cast<int>(prevPoint.x), static_cast<int>(prevPoint.y),
					static_cast<int>(screenPoint.x), static_cast<int>(screenPoint.y),
					color
				);
			}

			prevPoint = screenPoint;
		}
	}

	// === 経線（縦方向） ===
	for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
		float lon = lonIndex * kLoneEvery;
		Vector3 prevPoint = {};
		for (uint32_t latIndex = 0; latIndex <= kSubdivision; ++latIndex) {
			float lat = -float(M_PI / 2.0f) + kLatEvery * float(latIndex);

			Vector3 point = {
				sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon)
			};

			Vector3 screenPoint = Transform(viewProjectionMatrix, point);
			screenPoint = Transform(viewportMatrix, screenPoint);

			if (latIndex > 0) {
				Novice::DrawLine(
					static_cast<int>(prevPoint.x), static_cast<int>(prevPoint.y),
					static_cast<int>(screenPoint.x), static_cast<int>(screenPoint.y),
					color
				);
			}

			prevPoint = screenPoint;
		}
	}
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

//ウィンドウの幅
int kWindowWidth = 1280;

//ウィンドウの高さ
int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };
	Vector3 SphereCenter = { 0.0f, 0.0f, 0.0f };
	float SphereRadius = 1.0f;

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

		// ビュー行列
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1, 1, 1 }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		// プロジェクション
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

		// ビューポート
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// リセット
		if (keys[DIK_R]) {
			cameraTranslate = { 0.0f, 1.9f, -6.49f };
			cameraRotate = { 0.26f, 0.0f, 0.0f };
			SphereCenter = { 0.0f, 0.0f, 0.0f };
			SphereRadius = 1.0f;
		}

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &SphereCenter.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &SphereRadius, 0.01f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 描画
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		Sphere sphere = { SphereCenter , SphereRadius };
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, BLACK);

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
