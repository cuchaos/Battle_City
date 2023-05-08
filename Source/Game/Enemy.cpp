#include <vector>
#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <random>
#include <string>
#include "Enemy.h"

// Tank Child
using namespace game_framework;

Enemy::Enemy() : CTank() {
	_IfGetTimeStop = false;
	_IfGetShip = false;
	_TimeStart = clock();
	_TimeFinish = clock();
	_ChooseClock = clock();
	_UpClock = clock();
	_SuccessClock = clock();
	_Success = true;
	_RandomLR = rand() % 3;
	_EnemyType = 0;
	_Setinit = false;
	_EnemyHaveItem = false;
	_Times = 0;
	//_RandomMoveTime = (rand() % 5 + 1) * 500;
}
void Enemy::LoadBitmap() {
	_Tank.LoadBitmapByString({ //LightTank
								"resources/Enemy_LightTank_Right1.bmp" ,"resources/Enemy_LightTank_Right2.bmp",
								"resources/Enemy_LightTank_Left1.bmp"  ,"resources/Enemy_LightTank_Left2.bmp",
								"resources/Enemy_LightTank_Top1.bmp"   ,"resources/Enemy_LightTank_Top2.bmp",
								"resources/Enemy_LightTank_Bottom1.bmp","resources/Enemy_LightTank_Bottom2.bmp",
								//QuickTank
								"resources/Enemy_QuickTank_Right1.bmp" ,"resources/Enemy_QuickTank_Right2.bmp",
								"resources/Enemy_QuickTank_Left1.bmp"  ,"resources/Enemy_QuickTank_Left2.bmp",
								"resources/Enemy_QuickTank_Top1.bmp"   ,"resources/Enemy_QuickTank_Top2.bmp",
								"resources/Enemy_QuickTank_Bottom1.bmp","resources/Enemy_QuickTank_Bottom2.bmp" ,
								//ArmorTank
								"resources/Enemy_ArmorTank_Right1.bmp" ,"resources/Enemy_ArmorTank_Right2.bmp",
								"resources/Enemy_ArmorTank_Left1.bmp"  ,"resources/Enemy_ArmorTank_Left2.bmp",
								"resources/Enemy_ArmorTank_Top1.bmp"   ,"resources/Enemy_ArmorTank_Top2.bmp",
								"resources/Enemy_ArmorTank_Bottom1.bmp","resources/Enemy_ArmorTank_Bottom2.bmp",
								//HeavyTank
								"resources/Enemy_HeavyTank_Right1.bmp" ,"resources/Enemy_HeavyTank_Right2.bmp",
								"resources/Enemy_HeavyTank_Left1.bmp"  ,"resources/Enemy_HeavyTank_Left2.bmp",
								"resources/Enemy_HeavyTank_Top1.bmp"   ,"resources/Enemy_HeavyTank_Top2.bmp",
								"resources/Enemy_HeavyTank_Bottom1.bmp","resources/Enemy_HeavyTank_Bottom2.bmp",
								//ItemTank LightTank
								"resources/Enemy_LightTank_Red_Right1.bmp","resources/Enemy_LightTank_Red_Right2.bmp",
								"resources/Enemy_LightTank_Red_Left1.bmp","resources/Enemy_LightTank_Red_Left2.bmp",
								"resources/Enemy_LightTank_Red_Top1.bmp","resources/Enemy_LightTank_Red_Top2.bmp",
								"resources/Enemy_LightTank_Red_Bottom1.bmp","resources/Enemy_LightTank_Red_Bottom2.bmp",
								//ItemTank QuickTank
								"resources/Enemy_QuickTank_Red_Right1.bmp","resources/Enemy_QuickTank_Red_Right2.bmp",
								"resources/Enemy_QuickTank_Red_Left1.bmp","resources/Enemy_QuickTank_Red_Left2.bmp",
								"resources/Enemy_QuickTank_Red_Top1.bmp","resources/Enemy_QuickTank_Red_Top2.bmp",
								"resources/Enemy_QuickTank_Red_Bottom1.bmp","resources/Enemy_QuickTank_Red_Bottom2.bmp",
								//ItemTank ArmorTank
								"resources/Enemy_ArmorTank_Red_Right1.bmp","resources/Enemy_ArmorTank_Red_Right2.bmp",
								"resources/Enemy_ArmorTank_Red_Left1.bmp","resources/Enemy_ArmorTank_Red_Left2.bmp",
								"resources/Enemy_ArmorTank_Red_Top1.bmp","resources/Enemy_ArmorTank_Red_Top2.bmp",
								"resources/Enemy_ArmorTank_Red_Bottom1.bmp","resources/Enemy_ArmorTank_Red_Bottom2.bmp",
								//ItemTank HeavyTank
								"resources/Enemy_HeavyTank_Red_Right1.bmp","resources/Enemy_HeavyTank_Red_Right2.bmp",
								"resources/Enemy_HeavyTank_Red_Left1.bmp","resources/Enemy_HeavyTank_Red_Left2.bmp",
								"resources/Enemy_HeavyTank_Red_Top1.bmp","resources/Enemy_HeavyTank_Red_Top2.bmp",
								"resources/Enemy_HeavyTank_Red_Bottom1.bmp","resources/Enemy_HeavyTank_Red_Bottom2.bmp" }, RGB(0, 0, 0));
	_Bullet.LoadBitmap();
}
int Enemy::GetEnemyScore() {
	return _EnemyScore;
}
bool Enemy::isEnemyHaveItem() {
	return _EnemyHaveItem;
}
bool Enemy::GetIfGetTimeStop() {
	return _IfGetTimeStop;
}
CMovingBitmap Enemy::GetEnemyBitmap() {
	return _Tank;
}
bool Enemy::GetEnemySetInit() {
	return _Setinit;
}
bool Enemy::GetIfFire(int FireOrder) {
	return _IfFire;
}
int Enemy::GetEnemyType() {
	return _EnemyType;
}
bool Enemy::GetIfBattle() {
	return _IfBattle;
}
void Enemy::SetEnemyHaveItem(bool has) {
	_EnemyHaveItem = has;
}
void Enemy::SetEnemyInit() {
	_X = Width * (rand()%4*8) + 100;
	_Y = Height * 0;
	_OriginAngle = Down;
	_TurnAngle = Down;
	_NowGrid = { (_X - 100) / Width, _Y / Height };
	_OffsetXY = { 0,0 };
	_Life = 1;
	_AttackSpeedUP = false;
	_CanBreakIron = false;
	_DoubleAttack = false;
	_IfGetShip = false;
	_IfGetTimeStop = false;
	SetFaceDirection();
	if (_EnemyType == LightTank){
		_EnemyScore = 100;						// 坦克分數
	}
	else if (_EnemyType == QuickTank) {
		_AttackSpeedUP = true;					// 射速
		_EnemyScore = 300;						// 坦克分數
	}
	else if (_EnemyType == ArmorTank) {
		_MovementSpeed = 4;						// 移動速度
		_EnemyScore = 200;						
	}
	else if (_EnemyType == HeavyTank) {
		_Life = 4;								// 生命
		_EnemyScore = 400;						
	}
	//SetFaceDirection();
	_Setinit = true;
}
void Enemy::SetEnemyType(int num) {
	_EnemyType = num;
}
void Enemy::SetOriginAngle(int direction) {
	_OriginAngle = direction;
}
void Enemy::SetIfGetTimeStop(bool IfGetTimeStop) {
	_IfGetTimeStop = IfGetTimeStop;
}
void Enemy::SetGetTimeStop(int Status) {
	if (Status == -1) {
		_IfGetTimeStop = false;
	}
}
void Enemy::SetFaceDirection() {
	switch (_OriginAngle){
	case Right:
		_Frameindex = 0 + _EnemyType * 8;
		break;
	case Left:
		_Frameindex = 2 + _EnemyType * 8;
		break;
	case Up:
		_Frameindex = 4 + _EnemyType * 8;
		break;
	case Down:
		_Frameindex = 6 + _EnemyType * 8;
		break;
	}
}
void Enemy::Animation() {
	if (_EnemyHaveItem){
		if (_FrameTime%15 < 7) {
			_Tank.SetFrameIndexOfBitmap(_Frameindex + _FrameTime % 2);
		}
		else if (_FrameTime%15 > 7) {
			_Tank.SetFrameIndexOfBitmap(_Frameindex + _FrameTime % 2 + 32);
		}
	}
	else if (!_EnemyHaveItem) {
		_Tank.SetFrameIndexOfBitmap(_Frameindex + _FrameTime % 6 / 3);
	}
	++_FrameTime;
}

void Enemy::SetBulletStatus(int BulletOrder, bool Status) {
	if (_Bullet.GetAlreadyFire() == true && Status == false) {
		_Bullet.SetIfBoom(true);
	}
	_Bullet.SetBulletAlreadyFire(Status);
}
void Enemy::SetIfFire(int FireOrder, bool Status) {
	_IfFire = Status;
}

int Enemy::GetEnemyDirectionInfo(int num) {
	switch (num)
	{
	case 0:
		return _OriginAngle;
		break;
	case 1:
		return _RandomLR;
		break;
	case 2:
		return _Success;
		break;
	case 3:
		return  _UpClock;
		break;
	case 4:
		return _KeepUP;
		break;
	case 5:
		return _Times;
		break;
	}
	return -1;
}
void Enemy::EnemyMove() {
	if (clock() -_ChooseClock >= 500 /*_RandomMoveTime*/) {
		_RandomFuncChoose = rand() % 2;
		 _RandomDirection = rand() % 4;
		//_RandomFuncChoose = 1;
		//_RandomMoveTime = (rand() % 5 + 1) *500;	// 移動時間 1~6sec
		//_TimeStart = clock();			// 重新開始計時
		_ChooseClock = clock();
	}
	else if (_RandomFuncChoose % 2 == 0) {
		EnemyRandomDirection();
	}
	else if (_RandomFuncChoose % 2 == 1) {
		ENemyMoveDown();
	}
}
void Enemy::EnemyRandomDirection(){
	switch (_RandomDirection) {
	case Right:
		TurnFace(VK_RIGHT);
		break;
	case Up:
		TurnFace(VK_UP);
		break;
	case Down:
		TurnFace(VK_DOWN);
		break;
	case Left:
		TurnFace(VK_LEFT);
		break;
	}
}

void Enemy::ENemyMoveDown() {
	if (clock() - _SuccessClock > 500){
		_Success = SuccessMove();
		_SuccessClock = clock();
	}
	if (_KeepUP){
		TurnFace(VK_UP);
		if (clock() - _UpClock >= 1000) {
			_KeepUP = false;
		}
	}
	else{
		if (_Success) {
			TurnFace(VK_DOWN);
		}
		else {
			if (_Times >=20) {
				_RandomLR = rand() % 3;
				_Times = 0;
			}
			else{
				switch (_RandomLR) {
					case 0:
						TurnFace(VK_LEFT);
						break;
					case 1:
						TurnFace(VK_RIGHT);
						break;
					/*case 2:
						TurnFace(VK_DOWN);
						break;*/
				}
			}
			++_Times;
		}
	}
}
bool Enemy::SuccessMove() {
	if (_MoveDownDistance > 832 - _Y || _MoveLRDistance != _X){
		_StopClock = clock();
		_MoveLRDistance = _X;
		_MoveDownDistance = 832 - _Y;
		return true;
	}
	else if (clock() - _StopClock >= 1000){
		_UpClock = clock();
		_KeepUP = true;
	}
	return false;
}
void Enemy::TankbeHit() {
	if (_FrameTime == 26){
		if (clock() - _SpawnClock >= 2500){
			_TankState = Spawn;
			_Setinit = false;
		}
		_Tank.SetTopLeft(0, 0);
	}
	else {
		if (_FrameTime > 26) {
			_FrameTime = 0;
		}
		else {
			_TankBrokenAnimation.SetFrameIndexOfBitmap((_FrameTime % 26) / 5 % 5);
			if (_FrameTime % 26 == 25){
				_SpawnClock = clock();
			}
		}
		_FrameTime += 1;
		_TankBrokenAnimation.ShowBitmap();
	}
}
void Enemy::FireBullet(int BulletOrder) {
	if (_OriginAngle == Right || _OriginAngle == Left) {
		_Bullet.SetBulletFire(_X, _Y + 25, _OriginAngle, _BulletFlySpeed);
	}
	else {
		_Bullet.SetBulletFire(_X + 25, _Y, _OriginAngle, _BulletFlySpeed);
	}
	_IfFire = _Bullet.GetAlreadyFire();
}
void Enemy::OnShow() {
	if (_IfBattle) {
		if (_TankState == Spawn) {
			if (!_Setinit) {
				SetEnemyInit();
			}
			else if (_Setinit) {
				CTank::LoadBitmap();
				ShowSpawnAnimation();
			}
		}
		else if(_TankState == Alive) {
			//_Tank.SetFrameIndexOfBitmap(_Frameindex);
			_Tank.SetTopLeft(_X, _Y);
			_Tank.ShowBitmap();
			_Bullet.OnShow();
		}
		else if (_TankState == Death) {
			_TankBrokenAnimation.SetTopLeft(_X, _Y);
			Enemy::TankbeHit();
		}
	}
}

void Enemy::OnShowScore(CDC *pDC, CFont* &fp) {
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CTextDraw::ChangeFontLog(pDC, 48, "STZhongsong", RGB(255,255,255));
	if (clock() - _SpawnClock <= 500) {
		CTextDraw::Print(pDC, _X, _Y, to_string(_EnemyScore));
	}
}
