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
		for (int i = 0; i < 4; i++) {
			EnemyList[i].SetIfBattle(true);
		}
	}
	if ((CMovingBitmap::IsOverlap(_PlayerTank.GetTankBitmap(), Prop.GetPropBitmap()) || Prop.GetIfTouched()) 
		&& Prop.GetIfExist()) {
		event.TrigGetProps(Prop, Stage1, _PlayerTank);
	}


	CGameStateRun::PlayerTankMove(&_PlayerTank);
	for (int i = 0; i < 4; i++) {
		if (_TimerFinish >= 11000 && !(EnemyList[0].isBreak())) {
			EnemyList[0].SetLife(0);
			/*EnemyList[0].TankbeHit();*/
			//if (!EnemyList[0].GetSpawnAnimationDone()){
			//	EnemyList[0].EnemyRespawn(3);
			//}
		}
		else if(!(EnemyList[i].isBreak())) {
			EnemyTankMove(&EnemyList[i]);
		}
	}
	if (_PlayerTank.GetSpawnAnimationDone()){
		CGameStateRun::BulletHitCollision(&_PlayerTank, EnemyList);
	}
	_TimerFinish = clock();
	

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
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	,{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
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
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1 }
,{ 1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 4, 4, 4, 4, 4, 4, 1, 1 }
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
					,{ 4, 4, 4, 4, 5, 1, 1, 1, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }
					,{ 4, 4, 4, 4, 5, 1, 1, 1, 1, 1, 1, 4, 7, 7, 4, 1, 1, 1, 4, 4, 1, 1, 4, 4, 1, 1 }//25
	};
	
	Stage1.OnInit(tempstage1);
	
	_MouseX = 0;
	_MouseY = 0;
	
	_PlayerTank.LoadBitmap();
	_PlayerTankFrontX = 0;
	_PlayerTankFrontY = 0;
	Prop.OnInit();
	event.TrigSetProps(Prop);
	EnemyList.push_back(_EnemyTank1);
	EnemyList.push_back(_EnemyTank2);
	EnemyList.push_back(_EnemyTank3);
	EnemyList.push_back(_EnemyTank4);
	for (int i = 0; i < 4; i++){
		EnemyList[i].SetEnemyType(i);
		EnemyList[i].SetEnemyInit();
		EnemyList[i].LoadBitmap();
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 0x5A || nChar == 0x58) {
		_PlayerTank.SetIfFire(true);
	}
	if (nChar == VK_DOWN)	_isHoldDownKey = true;
	if (nChar == VK_UP)		_isHoldUpKey   = true;
	if (nChar == VK_LEFT)	_isHoldLeftKey = true;
	if (nChar == VK_RIGHT)	_isHoldLeftKey = true;
	if (nChar == VK_DOWN || nChar == VK_RIGHT|| nChar == VK_LEFT|| nChar == VK_UP) _HoldKey = nChar;
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
	for (int i = 0; i < 4; i++) {
		//EnemyList[i].OnShow();
	}
	OnShowText();
}
void CGameStateRun::OnShowText() {
	CDC *pDC = CDDraw::GetBackCDC();
	CFont *fp;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 180, 0));
	_TimerFinish = clock();
	CTextDraw::Print(pDC, 0, 0, (to_string(_TimerStart / CLOCKS_PER_SEC)+" "+ to_string(_TimerFinish )));
	//CTextDraw::Print(pDC, 0, 50, (to_string(EnemyList[0].isBreak())));
	//CTextDraw::Print(pDC, 0, 70, (to_string(EnemyList[1].isBreak())));
	//CTextDraw::Print(pDC, 0, 90, (to_string(EnemyList[2].isBreak())));
	//CTextDraw::Print(pDC, 0, 110, (to_string(EnemyList[3].isBreak())));
	CTextDraw::Print(pDC, 0, 25, (to_string(_MouseX) + " " + to_string(_MouseY).c_str()));

	CTextDraw::Print(pDC, 0, 50, (to_string(_PlayerTank.GetLife())));
	/*
	vector<vector<int>> _tempcollision;
	_tempcollision = Stage1.GetFrontGridsIndex(_PlayerTank._Bullet.GetNowFrontPlace());
	CTextDraw::Print(pDC, 0, 50, (to_string(_tempcollision[0][0]) + "," + to_string(_tempcollision[0][1]).c_str()));
	CTextDraw::Print(pDC, 0, 75, (to_string(_tempcollision[1][0]) + "," + to_string(_tempcollision[1][1]).c_str()));
	*/
	ChooseStageScreen.OnShowText(pDC,fp);
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
		TankCollisionMap(tank);
	}
	TankShoot(tank);
}

void CGameStateRun::EnemyTankMove(Enemy *tank) {
	if (tank->GetSpawnAnimationDone()){
		tank->EnemyRandomDirection();
		TankCollisionMap(tank);
		tank->FireBullet();
		TankShoot(tank);
	}
}

void  CGameStateRun::TankShoot(CTank *tank) {
	if (tank->GetIfFire()) {
		tank->FireBullet();
		if (Stage1.GetIfBoardEdge(tank->_Bullet.GetNowBackPlace()[0][0], tank->_Bullet.GetNowBackPlace()[0][1]
			, tank->_Bullet.GetHeight(), tank->_Bullet.GetWidth(), tank->_Bullet.GetDirection()) == true) {

			_tempcollision = Stage1.GetFrontGridsIndex(tank->_Bullet.GetNowFrontPlace());
			if (Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 1) == true || Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 1) == true) {
				if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 4) {
					Stage1.ShootWall(tank->_Bullet.GetDirection(), 1, _tempcollision[0][1], _tempcollision[0][0]);
				}
				if (Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 4) {
					Stage1.ShootWall(tank->_Bullet.GetDirection(), 1, _tempcollision[1][1], _tempcollision[1][0]);
				}
				tank->SetBulletStatus(false);
				tank->SetIfFire(false);
			}
		}
		else {
			tank->SetBulletStatus(false);
			tank->SetIfFire(false);
		}
	}
}
void CGameStateRun::TankCollisionMap(CTank *tank) {
	tank->TankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 0) && Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 0)) && \
		Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		tank->Move();
	}

	/*
	if ((Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 2 || 
			Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 2) && tank->GetIfGetShip() == true) {
			tank->Move();
	}
	*/

	tank->Animation();
}
void CGameStateRun::BulletHitCollision(CPlayer *tank, vector<Enemy> &enemy) {
	for (auto& enemyTank:enemy){
		if (CMovingBitmap::IsOverlap(tank->GetTankBulletBitmap(), enemyTank.GetTankBitmap())) {
			enemyTank.SetLife(0);
		}
		else if (CMovingBitmap::IsOverlap(enemyTank.GetTankBulletBitmap(), tank->GetTankBitmap())) {
			tank->SetLife(0);
		}
		else if (CMovingBitmap::IsOverlap(tank->GetTankBulletBitmap(), enemyTank.GetTankBulletBitmap())){
			tank->SetBulletStatus(false);
			tank->SetIfFire(false);
			enemyTank.SetBulletStatus(false);
			enemyTank.SetIfFire(false);
		}
	}
}