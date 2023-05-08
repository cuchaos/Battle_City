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
	_TankState = Spawn;
	_Setinit = false;
	PlayerInit();
}
void CPlayer::PlayerInit() {
	_IfGetShip = false;
	_X = Width * 8 + 100;
	_Y = Height * 24;
	_Life = 1;
	_OriginAngle = Up;
	_IfInvicible = false;
	_TurnAngle = Up;
	_NowGrid = { (_X - 100) / Width, _Y / Height };
	_OffsetXY = { 0,0 };
	_AttackSpeedUP = false;
	_CanBreakIron = false;
	_DoubleAttack = false;
	_IfGetShip = false;
	_IfSecondFire = false;
	SetFaceDirection();
	_Setinit = true;
}
void CPlayer::LoadBitmap() {
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
			_AttackSpeedUP = true;
			_BulletFlySpeed = 20;
		}
		else if (_Level == 3) {
			_DoubleAttack = true;
		}
		else if (_Level == 4) {
			_CanBreakIron = true;
		}
	}
}
void CPlayer::TankbeHit() {
	if (_FrameTime == 26){
		_TankState = Spawn;
		_Setinit = false;
	}
	else {
		if (_FrameTime > 26){
			_FrameTime = 0;
		}
		else { 
			_TankBrokenAnimation.SetFrameIndexOfBitmap((_FrameTime % 26) / 5 % 5);
		}
		_FrameTime += 1;
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
	return _IfInvicible;
}
void CPlayer::Animation() {
	_Tank.SetFrameIndexOfBitmap(_Frameindex + _FrameTime % 6 / 3);
	_FrameTime += 1;
}

void CPlayer::InvicibleAnimation() {
	_Invicible.SetTopLeft(_X, _Y);
	_Invicible.SetAnimation(50,false);
	_Invicible.ShowBitmap(); 
}
void CPlayer::OnShow() {
	if (_IfBattle) {
		if (_TankState == Spawn) {
			if (!_Setinit) {
				PlayerInit();
			}
			else if(_Setinit){
				CTank::LoadBitmap();
				ShowSpawnAnimation();
				_InvicibleClock = clock();
				_IfInvicible = true;
			}
			_Tank.SetFrameIndexOfBitmap(_Frameindex);
		}
		else if (_TankState == Alive) {
			_Tank.SetTopLeft(_X, _Y);
			_Tank.ShowBitmap();
			if (_IfInvicible){
				InvicibleAnimation();
			}
			if (clock() - _InvicibleClock >= 2500 && clock() - _InvicibleClock <= 2550){
				_IfInvicible = false;
			}
			if (_IfGetShip){
				_Ship.SetTopLeft(_X,_Y);
				_Ship.ShowBitmap();
			}
			_Bullet.OnShow();
			_SecondBullet.OnShow();
		}
		else if (_TankState == Death) {
			_TankBrokenAnimation.SetTopLeft(_X, _Y);
			CPlayer::TankbeHit();
		}
	}
}


