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
vector<vector<int>> CTank::_Move = { {1,0},{0,1},{-1,0},{0,-1} }; //Vector 2D
CTank::CTank() :Width(32), Height(32) {
	_FrameTime = 0;							// 計時器
	_Frameindex = 4;						// 動畫偵 (0 is east,1 south, 2 west,3 north)*2
	                                                            /*why not enum and what is `*2` ?*/
	_FrameSecond = 2;						// 動畫變換速度
	_LocationDistance = Height/4;			// 定位點距離
	_MovementSpeed = 4;						// 移動速度
	_IfFire = false;
	_IfBattle = false;
	_IfExploded = true; // the first One that cant explode
	_IfGetShip = false;
	_IfGetTimeStop = false;
	_IfRespawnAnimationDone = false;
	_RespawnAnimationNum = 0;
	_FrontXY = { {0,0},{0,0} };						// 移動方向前方兩格子的XY
	_BulletFlySpeed = 15;
	_TankState = Death;
}
void CTank::LoadBitmap() {
	_SpawnAnimation.LoadBitmapByString({ "resources/Spawn_1.bmp",
										"resources/Spawn_2.bmp",
										"resources/Spawn_3.bmp",
										"resources/Spawn_4.bmp" }, RGB(0, 0, 0));
	_SpawnAnimation.SetAnimation(100, true);
	_TankBrokenAnimation.LoadBitmapByString({ "resources/Boom0.bmp",
											 "resources/Boom1.bmp",
											 "resources/Boom2.bmp",
											 "resources/Boom3.bmp",
											 "resources/Boom4.bmp" }, RGB(0, 0, 0));
}
void CTank::Move() {
	if (_IfGetTimeStop) {
		return;
	}
	int tempx = _MovementSpeed * _Move[_OriginAngle][0];
	int tempy = _MovementSpeed * _Move[_OriginAngle][1];
	_X += tempx;
	_Y += tempy;
	_OffsetXY[0] += tempx;
	_OffsetXY[1] += tempy;
	for (int i = 0; i < 2; i++) {
		if (abs(_OffsetXY[i]) >= Width) {	//當坦克持續移動到下一格時 偏移要歸零 不然_NowGrid會加太多次
			_OffsetXY[i] = 0;
		}
		else if (abs(_OffsetXY[i]) == _LocationDistance) {	//當坦克移動超過_LocationDistance 代表定位點要往下一格走
			if (_OffsetXY[i] < 0) {							//正負號會影響格子定位
				_NowGrid[i] -= 1;
			}
			else if (_OffsetXY[i] > 0) {
				_NowGrid[i] += 1;
			}
		}
	}
}
CTank::~CTank() {

}
int CTank::GetX1(){
	return _X;
}
int CTank::GetY1() {
	return _Y;
}
int CTank::GetHeight() {
	return _Tank.GetHeight();
}
int CTank::GetWidth() {
	return _Tank.GetWidth();
}
int CTank::GetOriginAngle() {
	return _OriginAngle;
}
int CTank::GetLife() {
	return _Life;
}
int CTank::GetLevel() {
	return _Level;
}
bool CTank::GetIfGetShip() {
	return _IfGetShip;
}
int CTank::GetTankState() {
	return _TankState;
}
CMovingBitmap CTank::GetTankBitmap() {
	return _Tank;
}
CMovingBitmap CTank::GetBulletBitmap() {
	return _Bullet.GetBitmap();
}

bool CTank::GetIfRespawnanimationdone() {
	return _IfRespawnAnimationDone;
}
bool CTank::GetIfexploded() {
	return _IfExploded;
}
void CTank::SetXY(int _x, int _y) {
	_X = _x;
	_Y = _y;
}
void CTank::SetLife(int num) {
	if (_Life > 0) {
		_Life = num;
	}
	if (_Life == 0) {
		_TankState = Death;
		_IfExploded = false;
	}
}
void CTank::SetIfBattle(bool Status) {
	_IfBattle = Status;
}

void CTank::SetTankState(TankState State) {
	switch (State)
	{
	case Spawn:
		_TankState = Spawn;
		break;
	case Alive:
		_TankState = Alive;
		break;
	case Death:
		_TankState = Death;
		break;
	}
}
//Bullet
void CTank::SetBulletOwner(int who) {
	_Bullet.SetOwner(who);
}
int CTank::GetBulletOwner() {
	return _Bullet.GetOwner();
}

//Location and Direction
vector<vector<int>> CTank::GetTankFront() {
	return _FrontXY;
}

void CTank::LocationPoint() {
	SetXY(_NowGrid[0] * Width + 100, _NowGrid[1] * Height);
	for (int i = 0; i < 2; i++) _OffsetXY[i] = 0;			//轉向後坦克的定位回正 偏移數值歸零
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
}
void CTank::TankFront() {		// 對坦克前方的兩格格子做XY定位
	if (_OriginAngle == Right) {
		_FrontXY[0][0] = _X + Width * 2;
		_FrontXY[0][1] = _Y + Height / 2;
		_FrontXY[1][0] = _X + Width * 2;
		_FrontXY[1][1] = _Y + Height * 3 / 2;
	}
	else if (_OriginAngle == Left) {
		_FrontXY[0][0] = _X;
		_FrontXY[0][1] = _Y + Height / 2;
		_FrontXY[1][0] = _X;
		_FrontXY[1][1] = _Y + Height * 3 / 2;
	}
	else if (_OriginAngle == Up) {
		_FrontXY[0][0] = _X + Width / 2;
		_FrontXY[0][1] = _Y;
		_FrontXY[1][0] = _X + Width * 3 / 2;
		_FrontXY[1][1] = _Y;
	}
	else if (_OriginAngle == Down) {
		_FrontXY[0][0] = _X + Width / 2;
		_FrontXY[0][1] = _Y + Height * 2;
		_FrontXY[1][0] = _X + Width * 3 / 2;
		_FrontXY[1][1] = _Y + Height * 2;
	}
}

//show

void CTank::ShowSpawnAnimation() {
	if (_SpawnAnimation.IsAnimationDone()) {
		++_RespawnAnimationNum;
	}
	if (_RespawnAnimationNum > 3) { // Already done all animation
		_IfRespawnAnimationDone = true;
	}
	_SpawnAnimation.ToggleAnimation();
}
