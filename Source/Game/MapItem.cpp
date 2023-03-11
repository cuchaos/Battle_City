﻿#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "./MapItem.h"
#include <string>

using namespace game_framework;

MapItem::MapItem() {
	_Type = 0;
}
MapItem::MapItem(int ItemType) { //傳某一個格子 type 進去
	_Type = ItemType;
	if (_Type == 1) { // 1是 黑色格子(可以走,不行射擊(穿過), 不行破壞)
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = true;
		//_OneGrid.LoadBitmapByString({ "resources/BlackGrid.bmp" }, RGB(180, 180, 180));
	}
	else if (_Type == 2) { //2是 水 (無法行走(拿道具可走), 不行射擊(穿過) ,不行破壞 )
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = false;
		_OneGrid.LoadBitmapByString({ "resources/Water1.bmp","resources/Water2.bmp" ,"resources/Water3.bmp" }, RGB(0, 0, 0));
		_OneGrid.SetAnimation(1000, false);
	}
	else if (_Type == 3) { //3是 斜面 (可行走 , 不行射擊(穿過) ,不行破壞 )
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = true;
		_OneGrid.LoadBitmapByString({ "resources/Ice.bmp" }, RGB(0, 0, 0));
	}
	else if (_Type == 4) { //4是 磚牆(無法行走(可破壞後走), 可射擊(無法穿過) ,可破壞 )
		_Health = 2;
		_IfShoot = true;
		_IfBreak = true;
		_IfWalk = false;
		_OneGrid.LoadBitmapByString({ "resources/wall.bmp","resources/WallTopBreak.bmp", "resources/WallBottomBreak.bmp","resources/WallLeftBreak.bmp","resources/WallRightBreak.bmp" }, RGB(0, 0, 0));
	}
	else if (_Type == 5) { //5是 鐵牆(無法行走(可破壞後走), 可射擊(無法穿過) ,不可破壞(拿道具後可破壞) )
		_Health = 3;
		_IfShoot = true;
		_IfBreak = false;
		_IfWalk = false;
		_OneGrid.LoadBitmapByString({ "resources/IronWall.bmp" }, RGB(0, 0, 0));
	}
	else if (_Type == 6) { //6是 草叢(可行走, 不可射擊(穿過) ,不可破壞 )
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = true;
		_OneGrid.LoadBitmapByString({ "resources/Grass.bmp" }, RGB(0, 0, 0));
	}
}
void MapItem::ChangeGridState(int Direction,int Attack) { // direction  1 是磚牆上面被打到 , 2 是下 , 3 是左 , 4 是右
	if (_Type == 4) {
		if (_Health > 1) {  //如果是磚牆,生命值不變 (最高是2) 變圖片就好
			_OneGrid.SetFrameIndexOfBitmap(Direction);
			_Health -= Attack;
		}
		else {  //如果沒生命了變Type 1
			_Type = 1; 
			_IfShoot = false;
			_IfBreak = false;
			_IfWalk = true;
		}
	}
	else if (_Type == 5 && Attack > 3) { //如果ItemBuff = true (玩家的攻擊力夠)
		_Type = 1;
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = true;
	}
}
int MapItem::GetType() {
	return _Type;
}
bool MapItem::GetIfShoot() {
	return _IfShoot;
}
bool MapItem::GetIfBreak() {
	return _IfBreak;
}
bool MapItem::GetIfWalk() {
	return _IfWalk;
}
void MapItem::SetTopLeft(int x, int y) {
	_OneGrid.SetTopLeft(x, y);
}
void MapItem::OnShow() {
	if (_Type != 1) {
		_OneGrid.ShowBitmap();
	}
}
CMovingBitmap MapItem::GetMapItmeBitmap() {
	return _OneGrid;
}