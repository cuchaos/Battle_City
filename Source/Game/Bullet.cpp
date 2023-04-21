#include <vector>
#include <string>
#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "Bullet.h"


using namespace game_framework;

vector<vector<int>> CBullet::_Move = { {1,0},{0,1},{-1,0},{0,-1} }; //Vector 2D

CBullet::CBullet() {
	//"resources/Bullet0.bmp","resources/Bullet1.bmp","resources/Bullet2.bmp","resources/Bullet3.bmp"
	//_Boom.LoadBitmapByString({ "resources/Boom0.bmp","resources/Boom1.bmp","resources/Boom2.bmp" }, RGB(0, 0, 0));
	_AlreadyFire = false;
	_IfBoom = false;
	_NowBackPlace = { {0,0},{0,0} };
	_NowFrontPlace = { {0,0},{0,0} };
	_Owner = 0;
}
void CBullet::LoadBitmap() {
	_Bulletimage.LoadBitmapByString({ "resources/Bullet0.bmp","resources/Bullet1.bmp","resources/Bullet2.bmp","resources/Bullet3.bmp" }, RGB(0,0,0));
	_Boom.LoadBitmapByString({ "resources/Boom0.bmp","resources/Boom1.bmp","resources/Boom2.bmp" }, RGB(0,0,0));
	_Boom.SetAnimation(50, true);
}
void CBullet::BulletFly() {
	_NowBackPlace[0][0] += _Move[_Direction][0] * _BulletSpeed;
	_NowBackPlace[0][1] += _Move[_Direction][1] * _BulletSpeed;
	_NowBackPlace[1][0] += _Move[_Direction][0] * _BulletSpeed;
	_NowBackPlace[1][1] += _Move[_Direction][1] * _BulletSpeed;

	_NowFrontPlace[0][0] += _Move[_Direction][0] * _BulletSpeed;
	_NowFrontPlace[0][1] += _Move[_Direction][1] * _BulletSpeed;
	_NowFrontPlace[1][0] += _Move[_Direction][0] * _BulletSpeed;
	_NowFrontPlace[1][1] += _Move[_Direction][1] * _BulletSpeed;

	_Bulletimage.SetTopLeft(_NowBackPlace[0][0], _NowBackPlace[0][1]);

	if (_IfBoom == false) {
		if (_Direction == Right || _Direction == Left) {
			_Boom.SetTopLeft(_NowBackPlace[0][0], _NowBackPlace[0][1] - 18);
		}
		else {
			_Boom.SetTopLeft(_NowBackPlace[0][0] - 18, _NowBackPlace[0][1]);
		}
	}
}

void CBullet::SetOwner(int who) {
	_Owner = who;
}
void CBullet::SetIfBoom(bool Status) {
	_Boom.ToggleAnimation();
	_IfBoom = Status;
}
void CBullet::SetBulletAlreadyFire(bool BulletAlreadyFire) {
	_AlreadyFire = BulletAlreadyFire;
}
void CBullet::SetBulletFire(int TankX,int TankY,int TankDirect,int BulletSpeed) {
	if (_AlreadyFire == false) {
		_Bulletimage.SetFrameIndexOfBitmap(TankDirect);
		_AlreadyFire = true;
		_BulletSpeed = BulletSpeed;
		_Direction = TankDirect;
		_NowBackPlace[0][0] = TankX;
		_NowBackPlace[0][1] = TankY;
		if (_Direction == Right || _Direction == Left) {
			_NowBackPlace[1][0] = TankX;
			_NowBackPlace[1][1] = TankY + _Bulletimage.GetHeight();	
			
			_NowFrontPlace[0][0] = TankX + _Move[_Direction][0] * _Bulletimage.GetWidth();
			_NowFrontPlace[0][1] = TankY;

			_NowFrontPlace[1][0] = _NowFrontPlace[0][0];
			_NowFrontPlace[1][1] = TankY + _Bulletimage.GetHeight();

			_Boom.SetTopLeft(_NowBackPlace[0][0], _NowBackPlace[0][1] - 18);
		}
		else if(_Direction == Down || _Direction == Up){
			_NowBackPlace[1][0] = TankX + _Bulletimage.GetWidth();
			_NowBackPlace[1][1] = TankY;

			_NowFrontPlace[0][0] = TankX;
			_NowFrontPlace[0][1] = TankY + _Move[_Direction][1] * _Bulletimage.GetHeight();

			_NowFrontPlace[1][0] = TankX + _Bulletimage.GetWidth();
			_NowFrontPlace[1][1] = _NowFrontPlace[0][1];

			_Boom.SetTopLeft(_NowBackPlace[0][0] - 18, _NowBackPlace[0][1]);
		}
		_Bulletimage.SetTopLeft(_NowBackPlace[0][0], _NowBackPlace[0][1]);
	}
}

int CBullet::GetHeight() {
	return _Bulletimage.GetHeight();
}
int CBullet::GetWidth() {
	return _Bulletimage.GetWidth();
}
bool CBullet::GetAlreadyFire() {
	return _AlreadyFire;
}
vector<vector<int>> CBullet::GetNowBackPlace() {
	return _NowBackPlace;
}
vector<vector<int>> CBullet::GetNowFrontPlace() {
	return _NowFrontPlace;
}
int CBullet::GetDirection() {
	return _Direction;
}
bool CBullet::GetIfBoom() {
	return _IfBoom;
}
int CBullet::GetOwner() {
	return _Owner;
}
CMovingBitmap CBullet::GetBitmap() {
	return _Bulletimage;
}

void CBullet::OnShow() {
	if (_AlreadyFire == true) {
		_Bulletimage.ShowBitmap();
	}
	if (_IfBoom == true) {
		if (_Boom.IsAnimationDone() == false) {
			_Boom.ShowBitmap();
		}
		else {
			_IfBoom = false;
		}
	}
}
