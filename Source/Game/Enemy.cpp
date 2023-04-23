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
	_EnemyType = 0;
	_Setinit = false;
	_EnemyHaveItem = false;
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
												"resources/Enemy_HeavyTank_Bottom1.bmp","resources/Enemy_HeavyTank_Bottom2.bmp" }, RGB(0, 0, 0));
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
	if (_OriginAngle == Right) {
		_Frameindex = 0 + _EnemyType * 8;
	}
	else if (_OriginAngle == Left) {
		_Frameindex = 2 + _EnemyType * 8;
	}
	else if (_OriginAngle == Up) {
		_Frameindex = 4 + _EnemyType * 8;
	}
	else if (_OriginAngle == Down) {
		_Frameindex = 6 + _EnemyType * 8;
	}
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
void Enemy::EnemyRandomDirection(){
	_RandomDirection = rand() % 4;		// 隨機移動四個方向
	_RandomMoveTime = rand() % 5 + 1;	// 移動時間 1~6sec
	_TimeFinish = clock();
	if ((_TimeFinish - _TimeStart) / CLOCKS_PER_SEC > _RandomMoveTime) {	// > :因為要播放重生動畫
		switch (_RandomDirection)
		{
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
		_TimeStart = clock();			// 重新開始計時
	}
}
void Enemy::TankbeHit() {
	if (_FrameTime == 26){
		if (clock() - _SpawnClock >= 2500){
			_TankState = Spawn;
			_Setinit = false;
		}
	}
	else {
		if (_FrameTime > 26){
			_FrameTime = 0;
		}
		else { 
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
		else if(_TankState == Live) {
			_Tank.SetFrameIndexOfBitmap(_Frameindex);
			_Tank.SetTopLeft(_X, _Y);
			_Tank.ShowBitmap();
			_Bullet.OnShow();
		}
		else if (_TankState == Death) {
			_TankBrokenAnimation.SetTopLeft(_X, _Y);
			TankbeHit();
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