#include "stdafx.h"
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
	_Type = NoneLoad;
	_IfGetShovel = false;
}
MapItem::MapItem(int ItemType) { // Item Type = Raw Data
	_Type = GridType(ItemType);
	_IfGetShovel = false;
	_IfShine = false;
	switch (_Type){
		case BlackGrid:
			_IfShoot = false;
			_IfBreak = false;
			_IfWalk = true;
			break;
		case Water:
			_IfShoot = false;
			_IfBreak = false;
			_IfWalk = false;
			_OneGrid.LoadBitmapByString({ "resources/Water1.bmp","resources/Water2.bmp" ,"resources/Water3.bmp" }, RGB(0, 0, 0));
			_OneGrid.SetAnimation(1000, false);
			break;
		case Ice:
			_IfShoot = false;
			_IfBreak = false;
			_IfWalk = true;
			_OneGrid.LoadBitmapByString({ "resources/Ice.bmp" }, RGB(0, 0, 0));
			break;
		case BrickGrid:
			_Health = 2;
			_IfShoot = true;
			_IfBreak = true;
			_IfWalk = false;
			_OneGrid.LoadBitmapByString({ "resources/wall.bmp","resources/WallLeftBreak.bmp","resources/WallTopBreak.bmp", "resources/WallRightBreak.bmp","resources/WallBottomBreak.bmp" }, RGB(0, 0, 0));
			_ShovelAnimaetion.LoadBitmapByString({ "resources/wall.bmp","resources/IronWall.bmp" }, RGB(0, 0, 0));
			// I add the Iron wall because the Item Shovel must change the type ,but bitmap was load at the first
			// so i add Iron wall bitmap in it. 
			_ShovelAnimaetion.SetAnimation(500, true);
			break;
		case SteelGrid:
			_Health = 3;
			_IfShoot = true;
			_IfBreak = false;
			_IfWalk = false;
			_OneGrid.LoadBitmapByString({ "resources/IronWall.bmp" }, RGB(0, 0, 0));
			break;
		case GrassGrid:
			_IfShoot = false;
			_IfBreak = false;
			_IfWalk = true;
			_OneGrid.LoadBitmapByString({ "resources/Grass.bmp" }, RGB(0, 0, 0));
			break;
		case FlagHome:
			_Health = 1;
			_IfShoot = true;
			_IfBreak = true;
			_IfWalk = false;
			_OneGrid.LoadBitmapByString({ "resources/Home.bmp" }, RGB(0, 0, 0));
			_OneGrid.SetTopLeft(484, 768);
			break;
	}
}
void MapItem::ChangeGridState(int Direction,int TankLevel) { 
	// direction  1 是磚牆左邊被打到(子彈往右飛) 
	//, 2 是上面被打(子彈往下) , 3是右邊被打(子彈往左飛) , 4是 下面被打(子彈往上飛)
	if (_Type == BrickGrid) {
		if (TankLevel < 4 && _Health > 1) {//如果是磚牆,生命值不變 (最高是2) 變圖片就好
			_OneGrid.SetFrameIndexOfBitmap(Direction + 1);
			_Health -= 1;
		}
		else {
			_Type = BlackGrid;
			_IfShoot = false;
			_IfBreak = false;
			_IfWalk = true;
		}
	}
	else if (_Type == SteelGrid && TankLevel == 4) { //如果Level = 4 (玩家的攻擊力夠)
		_Type = BlackGrid;
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
bool MapItem::GetIfShine() {
	return _IfShine;
}
void MapItem::SetTopLeft(int x, int y) {
	_OneGrid.SetTopLeft(x, y);
	if (_Type == BrickGrid) {
		_ShovelAnimaetion.SetTopLeft(x, y);
	}
}
void MapItem::GrassOnsShow() {
	if (_Type == GrassGrid) {
		_OneGrid.ShowBitmap();
	}
}
void MapItem::OnShow() {
	if (_IfGetShovel) {
		if (_IfShine && _ShovelAnimaetion.IsAnimationDone()) {
			_ShovelAnimaetion.ToggleAnimation();
		}
		_ShovelAnimaetion.ShowBitmap();
		return;
	}
	if (_Type != BlackGrid && _Type != GrassGrid) {
		_OneGrid.ShowBitmap();
	}
	
}

CMovingBitmap MapItem::GetMapItmeBitmap() {
	return _OneGrid;
}

void MapItem::SetShovelChangeType(GridType Type,bool IfShine) {
	if (Type == SteelGrid) {
		_IfGetShovel = true;
		_Type = SteelGrid;
		_Health = 3;
		_IfShoot = true;
		_IfBreak = false;
		_IfWalk = false;
		_IfShine = IfShine;
		if (!_IfShine) {
			_ShovelAnimaetion.SetFrameIndexOfBitmap(1);
		}
	}
	else if(Type == BrickGrid){
		_IfGetShovel = false;
		_Type = BrickGrid;
		_Health = 2;
		_IfShoot = true;
		_IfBreak = true;
		_IfWalk = false;
		_IfShine = false;
		_OneGrid.SetFrameIndexOfBitmap(0);
	}
}