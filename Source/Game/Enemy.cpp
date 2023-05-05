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
	_EnemyType = 0;
	_TimeStart = clock();
	_TimeFinish = clock();
	_IfSetinit = false;
	_IfGetTimeStop = false;
	_IfGetShip = false;
	_EnemyHaveItem = false;
}
void Enemy::LoadBitmap() {
	CTank::LoadBitmap();
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
bool Enemy::GetEnemyHaveItem() {
	return _EnemyHaveItem;
}
bool Enemy::GetIfGetTimeStop() {
	return _IfGetTimeStop;
}
CMovingBitmap Enemy::GetEnemyBitmap() {
	return _Tank;
}
bool Enemy::GetEnemySetInit() {
	return _IfSetinit;
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
	_IfRespawnAnimationDone = false;
	_IfGetShip = false;
	_IfGetTimeStop = false;
	_IfFire = false;
	SetFaceDirection();
	switch (_EnemyType)
	{
	case LightTank:
		_EnemyScore = 100;
		break;
	case QuickTank:
		_EnemyScore = 300;					
		break;
	case ArmorTank:
		_MovementSpeed = 4;					
		_EnemyScore = 200;
		break;
	case HeavyTank:
		_Life = 4;							
		_EnemyScore = 400;
		break;
	}
	
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
void Enemy::EnemyRandomDirection(){
	_RandomDirection = rand() % 4;		// 隨機移動四個方向
	_RandomMoveTime = rand() % 5 + 1;	// 移動時間 1~6sec
	_TimeFinish = clock();
	if ((_TimeFinish - _TimeStart) / CLOCKS_PER_SEC > _RandomMoveTime) {	// > :因為要播放重生動畫
		switch (_RandomDirection){
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
void Enemy::TankExpolsion() {
	if (_Ifblasting) {
		_TankBrokenAnimation.SetFrameIndexOfBitmap((_FrameTime % 26) / 5 % 5);
		if (_FrameTime % 26 == 25) {
			_SpawnClock = clock();
			_Ifblasting = false;
		}
		++_FrameTime;
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
void Enemy::SetEnemyReSpawn() {
	if (_IfRespawnAnimationDone) {
		SetEnemyInit();
		_TankState = Alive;
		_Tank.SetTopLeft(0, 0);
	}
}
void Enemy::OnShow() {
	if (_IfBattle) {
		switch (_TankState)
		{
		case Spawn:
			ShowSpawnAnimation();
			break;
		case Alive:
			_Tank.SetTopLeft(_X, _Y);
			_Tank.ShowBitmap();
			_Bullet.OnShow();
			break;
		case Death:
			_TankBrokenAnimation.SetTopLeft(_X, _Y);
			Enemy::TankExpolsion();
			break;
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
