#include <DxLib.h>
#include"vector3.h"
#include"matrix4.h"
#include<cstring>
//球の描画
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);
//関数プロトタイプ宣言
//int DrawCircle(Vector2 vec, int r, unsigned int color);

//線分の描画
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int color);

int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
	const int DivNum, const unsigned int DivColor, const unsigned int SpcColor, const int FillFlag);

//カメラの位置と姿勢の設定
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,//カメラの位置
	const Vector3& cameraTarget,//カメラの注視点
	const Vector3& cameraUp
);

//モデルの座標変換用行列をセットする
// DxLib => int MV1SetMatrix(int MHandle,MATRIX matrix);
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix);

//関数プロトタイプ宣言
void DrawAxis3D(const float length);
void DrawKeyOperation();

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LE2B_12_タカハシコウタ:";



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウィンドウ横幅
	const int WIN_WIDTH = 1600;

	// ウィンドウ縦幅
	const int WIN_HEIGHT = 900;
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0, 0, 64);

	// DXlibの初期化
	if (DxLib_Init() == -1)  return -1;

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み

	SetUseZBuffer3D(TRUE);//Zバッファを有効にする
	SetWriteZBuffer3D(TRUE);//Zバッファへの書き込みを有効にする


	// ゲームループで使う変数の宣言

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };
	//クリップ面
	SetCameraNearFar(1.0f, 10000.0f);//カメラの有効範囲を設定
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);//画面の中心をカメラの中心に合わせる
	Vector3 cameraPosition(50.0f, 50.0f, -400.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);
	SetCameraPositionAndTargetAndUpVec(
		cameraPosition,//カメラの位置
		cameraTarget,//カメラの注視点
		cameraUp//カメラの上の向き
	);

	//アプリで使用する変数
	int model;
	
	model = MV1LoadModel("car/car.mqo");
	//model = MV1LoadModel("fighter/fighter.mqo");

	//xyz軸の回転角度
	const float ROT_UINT = 0.01f;
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		if (CheckHitKey(KEY_INPUT_A)) rotY += ROT_UINT;
		if (CheckHitKey(KEY_INPUT_D)) rotY -= ROT_UINT;

		if (CheckHitKey(KEY_INPUT_W)) rotX += ROT_UINT;
		if (CheckHitKey(KEY_INPUT_S)) rotX -= ROT_UINT;

		if (CheckHitKey(KEY_INPUT_E)) rotZ += ROT_UINT;
		if (CheckHitKey(KEY_INPUT_Z)) rotZ -= ROT_UINT;

		//[R]でリセット
		if (CheckHitKey(KEY_INPUT_R))
		{
			rotX = rotY = rotZ = 0;
		}

		//各種変換行列の計算
		//Matrix4 matScale = scale(Vector3(5.0f, 5.0f, 5.0f));
		Matrix4 matScale = scale(Vector3(0.5f, 0.5f, 0.5f));

		Matrix4 matRotX = rotateX(0.0f);

		Matrix4 matRotY = rotateY(0.0f);

		Matrix4 matRotZ = rotateZ(0.0f);

		Matrix4 matRot = matRotX * matRotY * matRotZ;

		Matrix4 matWorld = matScale * matRot;

		MV1SetMatrix(model, matWorld);

		// 描画処理
		ClearDrawScreen();//画面を消去
		DrawAxis3D(200.0f);//xyz軸の描画
		MV1DrawModel(model);//モデルの描画


		DrawKeyOperation();//キー操作の描画

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}

//xyzの描画
void DrawAxis3D(const float length)
{
	//軸の線の描画
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));//x軸
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));//y軸
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));//z軸

	//軸の先の描画   xyzの先端に、円錐を付ける
	const float coneSize = 10.0f;
	DrawCone3D(Vector3(length, 0, 0), Vector3(length - coneSize, 0, 0), coneSize / 2, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), true);
	DrawCone3D(Vector3(0, length, 0), Vector3(0, length - coneSize, 0), coneSize / 2, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), true);
	DrawCone3D(Vector3(0, 0, length), Vector3(0, 0, length - coneSize), coneSize / 2, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), true);
}
//キー操作の描画
void DrawKeyOperation()
{
	const unsigned white = GetColor(255, 255, 255);

	DrawFormatString(10, 20 * 1, white, "  [W][E][R] R : リセット");
	DrawFormatString(10, 20 * 2, white, "[A][S][D]   AD : y軸まわりの回転");
	DrawFormatString(10, 20 * 3, white, " [Z]        WS : x軸まわりの回転");
	DrawFormatString(10, 20 * 4, white, "            EZ : z軸まわりの回転");
}
//オーバーロード関数
//表示位置にベクトル(Vector2)を指定できるようにする
//int DrawCircle(Vector2 vec, int r, unsigned int color)
//{
//	return DrawCircle(static_cast<int>(vec.x), static_cast<int>(vec.y), r, color);
//}
//弾の描画
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR centerPos = { CenterPos.x,CenterPos.y,CenterPos.z };//構造体 初期化子リスト

	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);//DxLib関数の呼び出し
}
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
	const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR topPos = { TopPos.x,TopPos.y,TopPos.z };
	VECTOR bottomPos = { BottomPos.x,BottomPos.y,BottomPos.z };

	return DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);
}
//線分の描画
//DXLib=> int DrawLine3D(VECTOR Pos1, VECTOR Pos2, unsigned int color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int color)
{
	VECTOR p1 = { Pos1.x,Pos1.y,Pos1.z };//構造体 初期化子リスト
	VECTOR p2 = { Pos2.x,Pos2.y,Pos2.z };//構造体 初期化子リスト

	return DrawLine3D(p1, p2, color);
}
//カメラの位置と姿勢の設定
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,//カメラの位置
	const Vector3& cameraTarget,//カメラの注視点
	const Vector3& cameraUp
)
{
	VECTOR position = { cameraPosition.x,cameraPosition.y,cameraPosition.z };
	VECTOR target = { cameraTarget.x,cameraTarget.y,cameraTarget.z };
	VECTOR up = { cameraUp.x,cameraUp.y,cameraUp.z };

	return SetCameraPositionAndTargetAndUpVec(position, target, up);
}
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix)
{
	MATRIX matrix;
	std::memcpy(&matrix, &Matrix, sizeof MATRIX);

	return MV1SetMatrix(MHandle, matrix);
}
