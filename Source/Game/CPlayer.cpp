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
	PlayerInit();
}
void CPlayer::PlayerInit() {
	_IfGetShip = false;
	_X = Width * 8 + 100;
	_Y = Height * 24;
	_Life = 1;
	_OriginAngle = Up;
	_TurnAngle = Up;
	_NowGrid = { (_X - 100) / Width, _Y / Height };
	_OffsetXY = { 0,0 };
	_AttackSpeedUP = false;
	_CanBreakIron = false;
	_DoubleAttack = false;
	_IfGetShip = false;
	_IfSecondFire = false;
	SetFaceDirection();
}
//int CPlayer::GetPlayerScore() {
//	return _PlayerScore;
//}

void CPlayer::SpawnAfter5Second() {
	_Invicible = true;
}

void CPlayer::LoadBitmap() {
	_Tank.LoadBitmapByString({  "resources/Tank_Right_1.bmp" ,"resources/Tank_Right_2.bmp",
							    "resources/Tank_Left_1.bmp"  ,"resources/Tank_Left_2.bmp", 
								"resources/Tank_Top_1.bmp"   ,"resources/Tank_Top_2.bmp", 
								"resources/Tank_Bottom_1.bmp","resources/Tank_Bottom_2.bmp"},RGB(0,0,0));
	_Bullet.LoadBitmap();
	_SecondBullet.LoadBitmap();
}
void CPlayer::FireBullet(int BulletOrder) {
	if (BulletOrder == 1 && _Bullet.GetIfBoom() == false) {
		if (_OriginAngle == Right || _OriginAngle == Left) {
			_Bullet.SetBulletFire(_X, _Y + 25, _OriginAngle, _BulletFlySpeed);
		}
		else {
			_Bullet.SetBulletFire(_X + 25, _Y, _OriginAngle, _BulletFlySpeed);
		}
		_IfFire = true;
	}
	else if(BulletOrder == 2 && _SecondBullet.GetIfBoom() == false){
		if (_OriginAngle == Right || _OriginAngle == Left) {
			_SecondBullet.SetBulletFire(_X, _Y + 25, _OriginAngle, _BulletFlySpeed);
		}
		else {
			_SecondBullet.SetBulletFire(_X + 25, _Y, _OriginAngle, _BulletFlySpeed);
		}
		_IfSecondFire = true;
	}
}
void CPlayer::TankbeHit() {
	if (_FrameTime == 26){
		if (true){
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
			}
		}
		_FrameTime += 1;
		_TankBrokenAnimation.ShowBitmap();
	}
}
//void CPlayer::KillEnemy(int type) {
//	_KillEnemyList.push_back(type);
//	if (type == LightTank) {
//		PlusPlayerScore(100);						
//	}
//	else if (type == QuickTank) {
//		PlusPlayerScore(300);						
//	}
//	else if (type == ArmorTank) {					
//		PlusPlayerScore(200);						
//	}
//	else if (type == HeavyTank) {					
//		PlusPlayerScore(400);						
//	}
//}

//void CPlayer::PlusPlayerScore(int score) {
//	_PlayerScore += score;
//}

void CPlayer::SetBulletStatus(int BulletOrder, bool Status) { // 1 is first bullet , 2 is second bullet 
	if (BulletOrder == 1) {
		if (_Bullet.GetAlreadyFire() == true && Status == false) {
			_Bullet.SetIfBoom(true);
		}
		_Bullet.SetBulletAlreadyFire(Status);
	}
	else if (BulletOrder == 2) {
		if (_SecondBullet.GetAlreadyFire() == true && Status == false) {
			_SecondBullet.SetIfBoom(true);
		}
		_SecondBullet.SetBulletAlreadyFire(Status);
	}
}
void CPlayer::SetIfFire(int FireOrder, bool Status) {
	if (FireOrder == 1) {
		_IfFire = Status;
	}
	else if(FireOrder == 2){
		_IfSecondFire = Status;
	}
}
void CPlayer::SetIfGetShip(bool Status) {
	_IfGetShip = Status;
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
void CPlayer::SetFaceDirection() {
	if (_OriginAngle == Right) {
		_Frameindex = 0 ;
	}
	else if (_OriginAngle == Left) {
		_Frameindex = 2 ;
	}
	else if (_OriginAngle == Up) {
		_Frameindex = 4 ;
	}
	else if (_OriginAngle == Down) {
		_Frameindex = 6 ;
	}
}
void CPlayer::SetMoveOnIce(bool IfOnIce) {
	
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

void CPlayer::OnShow() {
	if (_IfBattle) {
		if (_TankState == Spawn) {
			if (!_Setinit) {
				//SetEnemyType();
				//SetEnemyInit();
				PlayerInit();
			}
			else if(_Setinit){
				CTank::LoadBitmap();
				ShowSpawnAnimation();
			}
		}
		else if (_TankState == Live) {
			_Tank.SetFrameIndexOfBitmap(_Frameindex);
			_Tank.SetTopLeft(_X, _Y);
			_Tank.ShowBitmap();
			_Bullet.OnShow();
			_SecondBullet.OnShow();
		}
		else if (_TankState == Death) {
			_TankBrokenAnimation.SetTopLeft(_X, _Y);
			TankbeHit();
		}
	}
}


