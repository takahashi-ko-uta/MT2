#include <DxLib.h>
#include"vector3.h"
#include"matrix4.h"
#include<cstring>
//���̕`��
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);
//�֐��v���g�^�C�v�錾
//int DrawCircle(Vector2 vec, int r, unsigned int color);

//�����̕`��
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int color);

int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
	const int DivNum, const unsigned int DivColor, const unsigned int SpcColor, const int FillFlag);

//�J�����̈ʒu�Ǝp���̐ݒ�
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,//�J�����̈ʒu
	const Vector3& cameraTarget,//�J�����̒����_
	const Vector3& cameraUp
);

//���f���̍��W�ϊ��p�s����Z�b�g����
// DxLib => int MV1SetMatrix(int MHandle,MATRIX matrix);
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix);

//�֐��v���g�^�C�v�錾
void DrawAxis3D(const float length);
void DrawKeyOperation();

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "LE2B_12_�^�J�n�V�R�E�^:";



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �E�B���h�E����
	const int WIN_WIDTH = 1600;

	// �E�B���h�E�c��
	const int WIN_HEIGHT = 900;
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0, 0, 64);

	// DXlib�̏�����
	if (DxLib_Init() == -1)  return -1;

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���

	SetUseZBuffer3D(TRUE);//Z�o�b�t�@��L���ɂ���
	SetWriteZBuffer3D(TRUE);//Z�o�b�t�@�ւ̏������݂�L���ɂ���


	// �Q�[�����[�v�Ŏg���ϐ��̐錾

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };
	//�N���b�v��
	SetCameraNearFar(1.0f, 10000.0f);//�J�����̗L���͈͂�ݒ�
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);//��ʂ̒��S���J�����̒��S�ɍ��킹��
	Vector3 cameraPosition(50.0f, 50.0f, -400.0f);
	Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);
	SetCameraPositionAndTargetAndUpVec(
		cameraPosition,//�J�����̈ʒu
		cameraTarget,//�J�����̒����_
		cameraUp//�J�����̏�̌���
	);

	//�A�v���Ŏg�p����ϐ�
	int model;
	
	model = MV1LoadModel("car/car.mqo");
	//model = MV1LoadModel("fighter/fighter.mqo");

	//xyz���̉�]�p�x
	const float ROT_UINT = 0.01f;
	float rotX = 0.0f;
	float rotY = 0.0f;
	float rotZ = 0.0f;

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
		if (CheckHitKey(KEY_INPUT_A)) rotY += ROT_UINT;
		if (CheckHitKey(KEY_INPUT_D)) rotY -= ROT_UINT;

		if (CheckHitKey(KEY_INPUT_W)) rotX += ROT_UINT;
		if (CheckHitKey(KEY_INPUT_S)) rotX -= ROT_UINT;

		if (CheckHitKey(KEY_INPUT_E)) rotZ += ROT_UINT;
		if (CheckHitKey(KEY_INPUT_Z)) rotZ -= ROT_UINT;

		//[R]�Ń��Z�b�g
		if (CheckHitKey(KEY_INPUT_R))
		{
			rotX = rotY = rotZ = 0;
		}

		//�e��ϊ��s��̌v�Z
		//Matrix4 matScale = scale(Vector3(5.0f, 5.0f, 5.0f));
		Matrix4 matScale = scale(Vector3(0.5f, 0.5f, 0.5f));

		Matrix4 matRotX = rotateX(0.0f);

		Matrix4 matRotY = rotateY(0.0f);

		Matrix4 matRotZ = rotateZ(0.0f);

		Matrix4 matRot = matRotX * matRotY * matRotZ;

		Matrix4 matWorld = matScale * matRot;

		MV1SetMatrix(model, matWorld);

		// �`�揈��
		ClearDrawScreen();//��ʂ�����
		DrawAxis3D(200.0f);//xyz���̕`��
		MV1DrawModel(model);//���f���̕`��


		DrawKeyOperation();//�L�[����̕`��

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}

//xyz�̕`��
void DrawAxis3D(const float length)
{
	//���̐��̕`��
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));//x��
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));//y��
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));//z��

	//���̐�̕`��   xyz�̐�[�ɁA�~����t����
	const float coneSize = 10.0f;
	DrawCone3D(Vector3(length, 0, 0), Vector3(length - coneSize, 0, 0), coneSize / 2, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), true);
	DrawCone3D(Vector3(0, length, 0), Vector3(0, length - coneSize, 0), coneSize / 2, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), true);
	DrawCone3D(Vector3(0, 0, length), Vector3(0, 0, length - coneSize), coneSize / 2, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), true);
}
//�L�[����̕`��
void DrawKeyOperation()
{
	const unsigned white = GetColor(255, 255, 255);

	DrawFormatString(10, 20 * 1, white, "  [W][E][R] R : ���Z�b�g");
	DrawFormatString(10, 20 * 2, white, "[A][S][D]   AD : y���܂��̉�]");
	DrawFormatString(10, 20 * 3, white, " [Z]        WS : x���܂��̉�]");
	DrawFormatString(10, 20 * 4, white, "            EZ : z���܂��̉�]");
}
//�I�[�o�[���[�h�֐�
//�\���ʒu�Ƀx�N�g��(Vector2)���w��ł���悤�ɂ���
//int DrawCircle(Vector2 vec, int r, unsigned int color)
//{
//	return DrawCircle(static_cast<int>(vec.x), static_cast<int>(vec.y), r, color);
//}
//�e�̕`��
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR centerPos = { CenterPos.x,CenterPos.y,CenterPos.z };//�\���� �������q���X�g

	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);//DxLib�֐��̌Ăяo��
}
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
	const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR topPos = { TopPos.x,TopPos.y,TopPos.z };
	VECTOR bottomPos = { BottomPos.x,BottomPos.y,BottomPos.z };

	return DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);
}
//�����̕`��
//DXLib=> int DrawLine3D(VECTOR Pos1, VECTOR Pos2, unsigned int color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int color)
{
	VECTOR p1 = { Pos1.x,Pos1.y,Pos1.z };//�\���� �������q���X�g
	VECTOR p2 = { Pos2.x,Pos2.y,Pos2.z };//�\���� �������q���X�g

	return DrawLine3D(p1, p2, color);
}
//�J�����̈ʒu�Ǝp���̐ݒ�
int SetCameraPositionAndTargetAndUpVec(
	const Vector3& cameraPosition,//�J�����̈ʒu
	const Vector3& cameraTarget,//�J�����̒����_
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
