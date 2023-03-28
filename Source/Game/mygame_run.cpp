#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}



void CGameStateRun::OnMove()                            // 移動遊戲元素
{
	if (_NowStage == -1) { // NowStage == -1  代表正在選 , == 0 代表正在戰鬥(已經選地圖了), == 1 ~ 35代表已經選完了
		event.TrigSelectingStage(ChooseStageScreen);
	}
	if (_NowStage >= 1) {
		event.TrigSetBattleMap(_NowStage,Stage1, _EnemyNum,ChooseStageScreen);
		_PlayerTank.SetIfBattle(true);
		_EnemyTank.SetIfBattle(true);
	}
	if (CMovingBitmap::IsOverlap(_PlayerTank.GetTankBitmap(), Prop.GetPropBitmap()) || Prop.GetIfTouched() ) {
		event.TrigGetProps(Prop, Stage1, _PlayerTank);
	}

	PlayerTankMove(&_PlayerTank);
	EnemyTankMove(&_EnemyTank);
	_TimerFinish = clock();

	if (_PlayerTank.GetIfFire()) {
		_PlayerTank.FireBullet();
		_tempcollision = Stage1.GetFrontGridsIndex(_PlayerTank.GetBulletPlace());
		if (Stage1.GetIfBoardEdge(_PlayerTank.GetBulletX(), _PlayerTank.GetBulletY(), _PlayerTank.GetBulletHeight(), _PlayerTank.GetBulletWidth(), _PlayerTank.GetBulletDirection()) == true) {
			if (Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 1) == true || Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 1) == true) {
				if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 4) {
					Stage1.ShootWall(_PlayerTank.GetBulletDirection(), 1, _tempcollision[0][1], _tempcollision[0][0]);
				}
				if (Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 4) {
					Stage1.ShootWall(_PlayerTank.GetBulletDirection(), 1, _tempcollision[1][1], _tempcollision[1][0]);
				}
				_PlayerTank.SetBulletStatus(false);
				_PlayerTank.SetIfFire(false);
			}
		}
		else {
			_PlayerTank.SetBulletStatus(false);
			_PlayerTank.SetIfFire(false);
		}
	}
}
void CGameStateRun::OnInit()                                  // 遊戲的初值及圖形設定
{
	srand((unsigned)time(NULL));
	_NowStage = -1;
	ChooseStageScreen.LoadBitMap();
	vector<vector<int>> tempstage1,tempstage2,tempstage5,tempstage17;
	tempstage1= { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 5, 5, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 5, 5, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4 }
	,{ 5, 5, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 5, 5 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 5, 5, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 5, 5, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};
	tempstage2 = { { 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4, 5, 5, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4, 5, 5, 4, 4, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1 }
,{ 6, 6, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 4, 4, 6, 6, 4, 4, 5, 5 }
,{ 6, 6, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1, 4, 4, 6, 6, 4, 4, 5, 5 }
,{ 6, 6, 6, 6, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 5, 5, 1, 1, 6, 6, 1, 1, 1, 1 }
,{ 6, 6, 6, 6, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 5, 5, 1, 1, 6, 6, 1, 1, 1, 1 }
,{ 1, 1, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 6, 6, 5, 5, 1, 1, 1, 1, 6, 6, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 6, 6, 5, 5, 1, 1, 1, 1, 6, 6, 4, 4, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 5, 5, 6, 6, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 5, 5, 6, 6, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
,{ 5, 5, 4, 4, 1, 1, 5, 5, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1 }
,{ 5, 5, 4, 4, 1, 1, 5, 5, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 5, 5, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 5, 5, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 4, 5, 5, 4, 1, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 4, 5, 5, 4, 1, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1 }
	};
	tempstage5 = { { 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1 } 
,{ 5, 5, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1, 1 }
,{ 5, 5, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 5, 5, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2 }
,{ 4, 4, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2 }
,{ 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 4, 4, 4, 4 }
,{ 1, 1, 1, 1, 4, 4, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 4, 4, 4, 4 }
,{ 4, 4, 4, 4, 1, 1, 1, 1, 2, 2, 4, 4, 1, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1 } 
,{ 4, 4, 4, 4, 1, 1, 1, 1, 2, 2, 4, 4, 1, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 1, 1, 1 }
,{ 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 5, 5, 1, 1, 4, 4, 1, 1, 1, 5, 1, 1, 1, 1 }
,{ 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 5, 5, 1, 1, 4, 4, 1, 1, 1, 5, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 4, 4, 4, 4 }
,{ 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 4, 4, 4, 4 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1 }
,{ 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1 } 
,{ 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1 }
,{ 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};
	tempstage17 = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
					,{ 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1 }
					,{ 1, 1, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 1, 1 }//2
					,{ 1, 1, 4, 4, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 1, 1 }
					,{ 1, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1 }
					,{ 1, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1 }//5
					,{ 3, 3, 3, 3, 3, 3, 5, 1, 4, 4, 1, 1, 1, 1, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1 }
					,{ 3, 3, 3, 3, 3, 3, 5, 1, 4, 4, 1, 1, 1, 1, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1 }
					,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1 }//8
					,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1 }
					,{ 1, 1, 1, 1, 1, 5, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 1, 4, 4, 1, 1, 1, 5, 5, 5, 5 }
					,{ 1, 1, 1, 1, 1, 5, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1 }//11
					,{ 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4 }
					,{ 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4 }
					,{ 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 5, 1, 1, 1, 1, 1, 1, 1 }//14
					,{ 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 5, 1, 1, 1, 1, 1, 1, 1 }
					,{ 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 1, 1, 4, 4, 1, 1 }
					,{ 1, 1, 4, 4, 4, 4, 4, 4, 1, 1, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 1, 1, 4, 4, 1, 1 }//17
					,{ 3, 3, 3, 3, 3, 3, 4, 4, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
					,{ 3, 3, 3, 3, 3, 3, 4, 4, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
					,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 1, 1, 5, 5, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1 }//20
					,{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1 }
					,{ 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1 }
					,{ 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 1, 4, 4, 4, 4, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1 }//23
					,{ 4, 4, 4, 4, 5, 1, 1, 1, 1, 1, 1, 4, 5, 5, 4, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
					,{ 4, 4, 4, 4, 5, 1, 1, 1, 1, 1, 1, 4, 5, 5, 4, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }//25
	};
	
	Stage1.OnInit(tempstage5);
	
	_MouseX = 0;
	_MouseY = 0;
	
	_PlayerTank.LoadBitmap();
	_PlayerTank.LoadSpawnBitmap();
	_PlayerTankFrontX = 0;
	_PlayerTankFrontY = 0;
	Prop.OnInit();
	event.TrigSetProps(Prop);
	/*for (auto num:EnemyTypeList){
		EnemyTank.SetEnemyType(num);
		EnemyTank.SetEnemyInit();
		EnemyTank.SetXY(num*64,0);
		EnemyTank.LoadBitmap();
		EnemyList.push_back(EnemyTank);
	}*/
	_EnemyTank.SetEnemyType(0);
	_EnemyTank.SetEnemyInit();
	_EnemyTank.LoadBitmap();
	_TimerStart = clock();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 0x5A || nChar == 0x58) {
		_PlayerTank.FireBullet();
	}
	if (nChar == VK_DOWN)	_isHoldDownKey = true;
	if (nChar == VK_UP)		_isHoldUpKey   = true;
	if (nChar == VK_LEFT)	_isHoldLeftKey = true;
	if (nChar == VK_RIGHT)	_isHoldLeftKey = true;
	if (nChar == VK_DOWN || nChar == VK_RIGHT|| nChar == VK_LEFT|| nChar == VK_UP) _HoldKey = nChar;
	/*if (GetKeyState(VK_UP) || GetKeyState(VK_DOWN) || GetKeyState(VK_RIGHT) || GetKeyState(VK_LEFT)) {
		_HoldKey = nChar;
		_PlayerTank.TurnFace(_HoldKey);
		_PlayerTank.Move();
		_PlayerTank.Animation();
	}*/
	_NowStage = ChooseStageScreen.OnKeyDown(nChar, nRepCnt, nFlags);
	
	
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN)	_isHoldDownKey = false;
	if (nChar == VK_UP)		_isHoldUpKey   = false;
	if (nChar == VK_LEFT)	_isHoldLeftKey = false;
	if (nChar == VK_RIGHT)	_isHoldLeftKey = false;
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)    // 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)    // 處理滑鼠的動作
{
	_MouseX = point.x;
	_MouseY = point.y;
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)    // 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	ChooseStageScreen.OnShow();
	Stage1.OnShow();
	Prop.OnShow();
	_PlayerTank.OnShow();
	_EnemyTank.OnShow();
	/*for (auto _Enemy:EnemyList){
		_Enemy.OnShow();
	}*/
	OnShowText();
}
void CGameStateRun::OnShowText() {
	CDC *pDC = CDDraw::GetBackCDC();
	CFont *fp;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 180, 0));

	CTextDraw::Print(pDC, 0, 0, (to_string(_TimerStart / CLOCKS_PER_SEC)+" "+ to_string(_TimerFinish / CLOCKS_PER_SEC)));
	CTextDraw::Print(pDC, 0, 25, (to_string(_MouseX) + " " + to_string(_MouseY).c_str()));
	ChooseStageScreen.OnShowText(pDC,fp);
	/*
	CTextDraw::ChangeFontLog( pDC, 10, "TRANSPARENT", RGB(0, 180, 0));
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			CTextDraw::Print(pDC, j * 32 + 110, i * 32, (to_string(i) + "," + to_string(j).c_str()));
		}
	}
	*/

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::PlayerTankMove(CPlayer *tank) {
	if ((_isHoldRightKey == true || \
		_isHoldLeftKey == true || \
		_isHoldDownKey == true || \
		_isHoldUpKey == true) && \
		tank->GetSpawnAnimationDone())
	{
		tank->TurnFace(_HoldKey);
		tank->TankFront();
		_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 0) && Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 0)) /*|| \
			(Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 1) && Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 1))*/ && \
			Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
			tank->Move();
		}
		tank->Animation();
	}
}

void CGameStateRun::EnemyTankMove(Enemy *tank) {
	int _RandomDirection = rand()%4;
	int _RandomMoveTime = rand() % 13 * 7 * 23 % 17 * 29 % 3 +1;
	if (tank->GetSpawnAnimationDone()){
		if ((_TimerFinish - _TimerStart) / CLOCKS_PER_SEC >_RandomMoveTime){
			switch (_RandomDirection)
			{
			case Right:
				tank->TurnFace(VK_RIGHT);
				break;
			case Up:
				tank->TurnFace(VK_UP);
				break;
			case Down:
				tank->TurnFace(VK_DOWN);
				break;
			case Left:
				tank->TurnFace(VK_LEFT);
				break;
			}
			_TimerStart = clock();
		}
		tank->TankFront();
		_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 0) && Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 0)) /*|| \
			(Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 1) && Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 1))*/ && \
			Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
			tank->Move();
		}
		tank->Animation();
	}
	
}