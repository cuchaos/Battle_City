#include "stdafx.h"
#include "../Core/Resource.h"
#include <string>
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "./MapItem.h"

using namespace game_framework;

MapItem::MapItem(int Type) { //�ǬY�@�Ӯ�l type �i�h
	_Type = Type;
	if (_Type == 1) { // 1�O �¦��l(�i�H��,����g��(��L), ����}�a)
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = true;
	}
	else if (_Type == 2) { //2�O �� (�L�k�樫(���D��i��), ����g��(��L) ,����}�a )
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = false;
		_OneGrid.LoadBitmapByString({ "resources/Water1.bmp","resource/Water2" ,"resources/Water3.bmp" }, RGB(0, 0, 0));
	}
	else if (_Type == 3) { //3�O �׭� (�i�樫 , ����g��(��L) ,����}�a )
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = true;
		_OneGrid.LoadBitmapByString({ "resources/Ice"}, RGB(0, 0, 0));
	}
	else if (_Type == 4) { //4�O �j��(�L�k�樫(�i�}�a�ᨫ), �i�g��(�L�k��L) ,�i�}�a )
		_IfShoot = true;
		_IfBreak = true;
		_IfWalk = false;
		_OneGrid.LoadBitmapByString({ "resources/Wall", "resources/WallBottomBreak","resources/WallLeftBreak","resources/WallRightBreak","resources/WallTopBreak" }, RGB(0, 0, 0));
	}
	else if (_Type == 5) { //5�O �K��(�L�k�樫(�i�}�a�ᨫ), �i�g��(�L�k��L) ,���i�}�a(���D���i�}�a) )
		_IfShoot = true;
		_IfBreak = false;
		_IfWalk = false;
		_OneGrid.LoadBitmapByString({ "resources/IronWall" }, RGB(0, 0, 0));
	}
	else if (_Type == 6) { //6�O ���O(�i�樫, ���i�g��(��L) ,���i�}�a )
		_IfShoot = false;
		_IfBreak = false;
		_IfWalk = true;
		_OneGrid.LoadBitmapByString({ "resources/Grass" }, RGB(0, 0, 0));
	}
}
void MapItem::OnShow() {
	if (_Type != 1) {
		_OneGrid.ShowBitmap();
	}
}