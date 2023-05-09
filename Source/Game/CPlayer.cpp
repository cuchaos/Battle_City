#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <vector>
#include "CPlayer.h"

// Tank Child
using namespace game_framework;

CPlayer::CPlayer() : CTank(){
	_PlayerScore = 0;
	_KillEnemyList = {0,0,0,0};
	_Level = 1;
	_Life = 1;
	_TankState = Death;
}
void CPlayer::SetPlayerInit() {
	_TankState = Alive;
	_OriginAngle = Up;
	_TurnAngle = Up;
	_NowGrid = { (_X - 100) / Width, _Y / Height };
	_OffsetXY = { 0,0 };
	_IfInvicible = false;
	_IfSpawnInvicible = true;
	_IfGetShip = false;
	_IfSecondFire = false;
	_RespawnAnimationNum = 0;
	_SpawnInvicibleClock = clock();
	_Tank.SetFrameIndexOfBitmap(_Frameindex);
	SetFaceDirection();
}
void CPlayer::SetPlayerReSpawn() {
	if (_IfExploded) {
		_TankState = Spawn;
		_IfRespawnAnimationDone = false;
		_X = Width * 8 + 100;
		_Y = Height * 24;
		_Tank.SetTopLeft(_X, _Y);
		_SpawnAnimation.SetTopLeft(_X, _Y);
	}
}
void CPlayer::LoadBitmap() {
	CTank::LoadBitmap();
	_Tank.LoadBitmapByString({ "resources/Tank_Right_1.bmp" ,"resources/Tank_Right_2.bmp",
								"resources/Tank_Left_1.bmp"  ,"resources/Tank_Left_2.bmp",
								"resources/Tank_Top_1.bmp"   ,"resources/Tank_Top_2.bmp",
								"resources/Tank_Bottom_1.bmp","resources/Tank_Bottom_2.bmp" }, RGB(0, 0, 0));
	_Invicible.LoadBitmapByString({"resources/Spawn1.bmp","resources/Spawn2.bmp"}, RGB(0, 0, 0));
	_Ship.LoadBitmapByString({"resources/Ship.bmp"}, RGB(0, 0, 0));
	_Bullet.LoadBitmap();
	_SecondBullet.LoadBitmap();
}

void CPlayer::SetIfInvicible(bool Status) {
	_IfInvicible = Status;
}
void CPlayer::SetFaceDirection() {
	switch (_OriginAngle) {
	case Right: 
		_Frameindex = 0;
		break;
	case Left: 
		_Frameindex = 2;
		break;
	case Up: 
		_Frameindex = 4;
		break;
	case Down: 
		_Frameindex = 6;
		break;
	default:
		throw "unpog";
	}
}
void CPlayer::SetBulletStatus(int BulletOrder, bool Status) { // 1 is first bullet , 2 is second bullet 
	if (!(BulletOrder == 1 || BulletOrder == 2)) throw "wtf";
	auto& currentBullet = BulletOrder == 1 ? _Bullet : _SecondBullet;
	
	if (currentBullet.GetAlreadyFire() == true && Status == false) {
		currentBullet.SetIfBoom(true);
	}
	currentBullet.SetBulletAlreadyFire(Status);
}
void CPlayer::SetIfFire(int FireOrder, bool Status) {
	if (!(FireOrder == 1 || FireOrder == 2)) throw "wtf";
	auto& currentFire = FireOrder == 1 ? _IfFire : _IfSecondFire;
	currentFire = Status;
}
void CPlayer::SetIfGetShip(bool Status) {
	_IfGetShip = Status;
}

void CPlayer::FireBullet(int BulletOrder) {
	if (!(BulletOrder == 1 || BulletOrder == 2)) throw "wtf";
	auto& currentBullet = BulletOrder == 1 ? _Bullet : _SecondBullet;
	auto& currentFire = BulletOrder == 1 ? _IfFire : _IfSecondFire;
	if (currentBullet.GetIfBoom() == false) {
		if (_OriginAngle == Right || _OriginAngle == Left) {
			currentBullet.SetBulletFire(_X, _Y + 25, _OriginAngle, _BulletFlySpeed);
		}
		else {
			currentBullet.SetBulletFire(_X + 25, _Y, _OriginAngle, _BulletFlySpeed);
		}
		currentFire = true;
	}
}
void CPlayer::LevelUP() {
	if (_Level < 4) {
		_Level += 1;
		if (_Level == 2) {
			_BulletFlySpeed = 20;
		}
	}
}
void CPlayer::TankExpolsion() {
	if (!_IfExploded) {
		_TankBrokenAnimation.SetFrameIndexOfBitmap((_FrameTime % 26) / 5 % 5);
		if (_FrameTime % 26 == 25) {
			_IfExploded = true;
		}
		++_FrameTime;
		_TankBrokenAnimation.ShowBitmap();
	}
}

bool CPlayer::GetIfFire(int FireOrder) {
	if (FireOrder == 1) {
		return _IfFire;
	}
	else if (FireOrder == 2) {
		return _IfSecondFire;
	}
	return false;
}
bool CPlayer::GetIfInvicible() {
	return _IfInvicible || _IfSpawnInvicible;
}
void CPlayer::Animation() {
	_Tank.SetFrameIndexOfBitmap(_Frameindex + _FrameTime % 6 / 3);
	_FrameTime += 1;
}

void CPlayer::InvicibleAnimation() {
	_Invicible.SetTopLeft(_X, _Y);
	_Invicible.SetAnimation(50,false);
	_Invicible.ShowBitmap(); 
	if (clock() - _SpawnInvicibleClock >= 2500) {
		_IfSpawnInvicible = false;
	}
}
void CPlayer::OnShow() {
	if (_IfBattle) {
		switch (_TankState)
		{
		case Spawn:
			if (!_IfRespawnAnimationDone && _SpawnAnimation.IsAnimationDone()) {
				ShowSpawnAnimation();
			}
			_SpawnAnimation.ShowBitmap();
			break;
		case Alive:
			_Tank.SetTopLeft(_X, _Y);
			_Tank.ShowBitmap();
			if (_IfInvicible || _IfSpawnInvicible) {
				InvicibleAnimation();
			}
			if (_IfGetShip) {
				_Ship.SetTopLeft(_X, _Y);
				_Ship.ShowBitmap();
			}
			_Bullet.OnShow();
			_SecondBullet.OnShow();
			break;
		case Death:
			if (!_IfExploded) {
				_TankBrokenAnimation.SetTopLeft(_X, _Y);
				CPlayer::TankExpolsion();
			}
			break;
		}
	}
}


