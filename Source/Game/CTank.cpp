#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <vector>
#include "CTank.h"

// Tank Parent
using namespace game_framework;

CTank::CTank() :Width(32), Height(32) {
	_X = Width * 8 + 100;
	_Y = Height * 24;
	_Level = 1;								// 等級
	_Life = 1;								// 生命
	_OriginAngle = Up;						// 面對角度 0 is east,1 south, 2 west,3 north
	_TurnAngle = Up;						// 轉向角度
	_FrameTime = 0;							// 計時器
	_Frameindex = 4;						// 動畫偵 (0 is east,1 south, 2 west,3 north)*2
	_FrameSecond = 2;						// 動畫變換速度
	_LocationDistance = Height/4;			// 定位點距離
	_MovementSpeed = 2;						// 移動速度
	_AttackSpeedUP = false;
	_CanBreakIron = false;
	_DoubleAttack = false;
	_IfFire = false;
	_IfBattle = false;
	_FrontXY = { {0,0},{0,0} };						// 移動方向前方兩格子的XY
	_NowGrid = { (_X-100) / Width, _Y / Height };	// 坦克現在的格子
	_OffsetXY = { 0,0 };							// 偏移的XY距離
	_SpawnAnimationDone = false;					// 重生動畫結束撥放
	_BulletFlySpeed = 15;
	_isTankBrokenAnimationDone = false;
	_isNeedRespawn = false;
}
bool CTank::GetIfFire() {
	return _IfFire;
}
int CTank::GetX1(){
	return _X;
}
int CTank::GetY1() {
	return _Y;
}
int CTank::GetOriginAngle() {
	return _OriginAngle;
}
int CTank::GetLife() {
	return _Life;
}
void CTank::SetLife(int num) {
	_Life = num;
}
bool CTank::isBreak() {
	if (_Life == 0){
		return true;
	}
	return false;
}
bool CTank::GetEnemyisNeedRespawn() {
	return _isNeedRespawn;
}
void CTank::SetXY(int _x, int _y) {
	_X = _x;
	_Y = _y;
}

void CTank::SetIfBattle(bool Status) {
	_IfBattle = Status;
}


void CTank::Move() {
	if (_OriginAngle == Right){
		_X += _MovementSpeed;
		_OffsetXY[0] += _MovementSpeed;
	}
	else if (_OriginAngle == Left){
		_X -= _MovementSpeed;
		_OffsetXY[0] -= _MovementSpeed;
	}
	else if (_OriginAngle == Up){
		_Y -= _MovementSpeed;
		_OffsetXY[1] -= _MovementSpeed;
	}
	else if (_OriginAngle == Down) {
		_Y += _MovementSpeed;
		_OffsetXY[1] += _MovementSpeed;
	}
	for (int i = 0; i < 2; i++){
		if (abs(_OffsetXY[i]) >= Width){	//當坦克持續移動到下一格時 偏移要歸零 不然_NowGrid會加太多次
			_OffsetXY[i] = 0;
		}
		else if (abs(_OffsetXY[i]) == _LocationDistance){	//當坦克移動超過_LocationDistance 代表定位點要往下一格走
			if (_OffsetXY[i] < 0){							//正負號會影響格子定位
				_NowGrid[i] -= 1;
			}
			else if (_OffsetXY[i] > 0){
				_NowGrid[i] += 1;
			}
		}
	}
}

void CTank::TankbeHit() {
	if (_FrameTime == 26){
		_isTankBrokenAnimationDone = true;
		_SpawnAnimationDone = false;
		_isNeedRespawn = true;
	}
	else {
		if (_FrameTime > 26){
			_FrameTime = 0;
		}
		else {
			/*if (_FrameTime % 26 == 0) {
				_TankBrokenAnimation.SetFrameIndexOfBitmap(0);
			}
			else*/ 
			if (_FrameTime % 26 == 5) {
				_TankBrokenAnimation.SetFrameIndexOfBitmap(1);
			}
			else if (_FrameTime % 26 == 10) {
				_TankBrokenAnimation.SetFrameIndexOfBitmap(2);
			}
			else if (_FrameTime % 26 == 15) {
				_TankBrokenAnimation.SetFrameIndexOfBitmap(3);
			}
			else if (_FrameTime % 26 == 20) {
				_TankBrokenAnimation.SetFrameIndexOfBitmap(4);
			}
			else if (_FrameTime % 26 == 25) {
				_TankBrokenAnimation.SetFrameIndexOfBitmap(0);
			}
		}
		_FrameTime += 1;
		_TankBrokenAnimation.ShowBitmap();
	}
}

void CTank::Animation() {
	if (_FrameTime%_FrameSecond==0){
		if (_Frameindex%2==0){
			_Tank.SetFrameIndexOfBitmap(_Frameindex + 1);
			_Frameindex += 1;
		}
		else {
			_Tank.SetFrameIndexOfBitmap(_Frameindex - 1);
			_Frameindex -= 1;
		}
	}
	_FrameTime += 1;
}
void CTank::LocationPoint() {
	SetXY( _NowGrid[0]*Width+100 , _NowGrid[1]*Height );
	for (int i = 0; i < 2; i++) _OffsetXY[i] = 0;			//轉向後坦克的定位回正 偏移數值歸零
} 

void CTank::OnShow() {
	if (_IfBattle && !isBreak()) {
		if (!CTank::GetSpawnAnimationDone()) {
			CTank::LoadBitmap();
			CTank::ShowSpawnAnimation();
		}
		else if(CTank::GetSpawnAnimationDone()){
			_Tank.SetFrameIndexOfBitmap(_Frameindex);
			_Tank.SetTopLeft(_X, _Y);
			_Tank.ShowBitmap();
		}
		_Bullet.OnShow();
	}
	/*else if (isBreak() && _IfBattle){
		_Tank.UnshowBitmap();
		_FrameTime = 0;
		_IfBattle = false;
		_isNeedRespawn = true;
		_TankBrokenAnimation.SetTopLeft(_X, _Y);
	}*/
	else if (isBreak() && !_isTankBrokenAnimationDone) {
		_TankBrokenAnimation.SetTopLeft(_X, _Y);
		TankbeHit();
	}
}


/*Tank Front & Direction*/
void CTank::SetFaceDirection() {
	if (_OriginAngle == Right) {
		_Frameindex = 0;
	}
	else if (_OriginAngle == Left) {
		_Frameindex = 2;
	}
	else if (_OriginAngle == Up) {
		_Frameindex = 4;
	}
	else if (_OriginAngle == Down) {
		_Frameindex = 6;
	}
}
void CTank::TurnFace(UINT nChar) {
	if (nChar == VK_RIGHT) {
		_TurnAngle = Right;
	}
	else if (nChar == VK_LEFT) {
		_TurnAngle = Left;
	}
	else if (nChar == VK_UP) {
		_TurnAngle = Up;
	}
	else if (nChar == VK_DOWN) {
		_TurnAngle = Down;
	}
	if (_TurnAngle != _OriginAngle) {
		LocationPoint();
		_OriginAngle = _TurnAngle;
		SetFaceDirection();
		_FrameTime= 0;
	}
	//_Tank.SetFrameIndexOfBitmap(_Frameindex);
}
void CTank::TankFront() {		// 對坦克前方的兩格格子做XY定位
	if (_OriginAngle == Right) {
		_FrontXY[0][0] = _X + Width * 2;
		_FrontXY[0][1] = _Y + Height / 2;
		_FrontXY[1][0] = _X + Width * 2;
		_FrontXY[1][1] = _Y + Height * 3 / 2;
	}
	else if (_OriginAngle == Left) {
		_FrontXY[0][0] = _X ;
		_FrontXY[0][1] = _Y + Height / 2;
		_FrontXY[1][0] = _X ;
		_FrontXY[1][1] = _Y + Height * 3 / 2;
	}
	else if (_OriginAngle == Up) {
		_FrontXY[0][0] = _X + Width / 2;
		_FrontXY[0][1] = _Y ;
		_FrontXY[1][0] = _X + Width * 3 / 2;
		_FrontXY[1][1] = _Y ;
	}
	else if (_OriginAngle == Down) {
		_FrontXY[0][0] = _X + Width / 2;
		_FrontXY[0][1] = _Y + Height * 2;
		_FrontXY[1][0] = _X + Width * 3 / 2;
		_FrontXY[1][1] = _Y + Height * 2;
	}
}
vector<vector<int>> CTank::GetTankFront(){
	return _FrontXY;
}
/*Tank Spawn*/
bool CTank::GetSpawnAnimationDone() {
	return _SpawnAnimationDone;
}
void CTank::LoadBitmap() {
	_SpawnAnimation.LoadBitmapByString({"resources/Spawn_1.bmp",
										"resources/Spawn_2.bmp",
										"resources/Spawn_3.bmp", 
										"resources/Spawn_4.bmp"}, RGB(0, 0, 0));
	_TankBrokenAnimation.LoadBitmapByString({"resources/Boom0.bmp",
											 "resources/Boom1.bmp",
											 "resources/Boom2.bmp",
											 "resources/Boom3.bmp",
											 "resources/Boom4.bmp" },RGB(0 ,0 ,0));
}
void CTank::ShowSpawnAnimation() {
	if (_FrameTime % 12 == 0) {
		_SpawnAnimation.SetFrameIndexOfBitmap(0);
	}
	else if (_FrameTime % 12 == 3) {
		_SpawnAnimation.SetFrameIndexOfBitmap(1);
	}
	else if (_FrameTime % 12 == 6) {
		_SpawnAnimation.SetFrameIndexOfBitmap(2);
	}
	else if (_FrameTime % 12 == 9) {
		_SpawnAnimation.SetFrameIndexOfBitmap(3);
	}
	_FrameTime += 1;
	if (_FrameTime == 60) {
		_SpawnAnimationDone = true;
	}
	_SpawnAnimation.SetTopLeft(_X, _Y);
	_SpawnAnimation.ShowBitmap();
}

/*Bullet*/
void CTank::FireBullet() {
	//if (clock() - _Last_time >= 1) {
		if (_OriginAngle == Right || _OriginAngle == Left) {
			_Bullet.SetBulletFire(_X, _Y + 25, _OriginAngle,_BulletFlySpeed);
		}
		else {
			_Bullet.SetBulletFire(_X + 25, _Y, _OriginAngle, _BulletFlySpeed);
		}
		_Last_time = clock();
		_IfFire = _Bullet.GetAlreadyFire();
	//}
}
vector<vector<int>> CTank::GetBulletPlace() {
	return _Bullet._GetNowPlace();
}
void CTank::SetBulletStatus(bool Status) {
	if (_Bullet.GetAlreadyFire() == true && Status == false) {
		_Bullet.SetIfBoom(true);
	}
	_Bullet.SetBulletAlreadyFire(Status);
}
void CTank::SetIfFire(bool Status) {
	_IfFire = Status;
}
int CTank::GetBulletDirection() {
	return _Bullet.GetDirection();
}
int CTank::GetHeight() {
	return _Tank.GetHeight();
}
int CTank::GetWidth() {
	return _Tank.GetWidth();
}
int CTank::GetBulletX() {
	return _Bullet._GetNowPlace()[0][0];
}
int CTank::GetBulletY() {
	return _Bullet._GetNowPlace()[0][1];
}
int CTank::GetBulletHeight() {
	return _Bullet.GetHeight();
}
int CTank::GetBulletWidth() {
	return _Bullet.GetWidth();
}

CMovingBitmap CTank::GetTankBulletBitmap() {
	return _Bullet.GetBulletBitmap();
}
CMovingBitmap CTank::GetTankBitmap() {
	return _Tank;
}