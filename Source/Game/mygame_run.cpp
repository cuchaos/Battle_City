#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <string>
#include <algorithm>
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
	if (_NowStage == -1 && !_IfBattling) { // NowStage == -1  代表正在選, == 1 ~ 35代表已經選完了
		event.TrigSelectingStage(_Menu);
		return;
	}
	
	if (_NowStage >= 1 && !_IfBattling && !_IfSettling) {
		event.TrigSetBattleMap(_AllStage[_NowStage-1],Stage1, _EnemyNum,_Menu);
		EnemyTypeList.assign(_AllStageEnemy[_NowStage - 1].begin(), _AllStageEnemy[_NowStage - 1].end());
		_IfBattling = true;
		_PlayerTank.SetIfBattle(true);
		return;
	}	
	if (!_IfBattling) {
		return;
	}
	for (int i = _NowProp - 1; i > -1; i--) {
		if ((CMovingBitmap::IsOverlap(_PlayerTank.GetTankBitmap(), _Prop[i].GetPropBitmap()) || _Prop[i].GetIfTouched()) 
			&& _Prop[i].GetIfExist()) {
			if (i == _NowProp - 1 && _Prop[i].count(_Prop[i].GetType()) > 1
				&& (_Prop[i].GetType() == 5 || _Prop[i].GetType() == 3)) {
				_Prop[_Prop[i].find(_Prop[i].GetType())].SetIfCountDown(false);
				_Prop[i].SetIfExist(false);
				continue;
			}
			_IfEatItem[0] = 1;
			_IfEatItem[1] = _Prop[i].GetX();
			_IfEatItem[2] = _Prop[i].GetY();
			_IfEatItem[3] = clock();
			event.TrigGetProps(_Prop[i], Stage1, _PlayerTank, EnemyList);
		}
	}
	if (_PlayerTank.GetTankState() == Live){
		PlayerTankMove(&_PlayerTank);
	}
	for (int i = 0; i < 4; i++) {
		// 當TankState == Spawn 時 檢查EnemyTypeList 是否都生成完畢
		// 都生成完畢則將Enemy設定為 _IfBattle = false 避免重生
		if (EnemyList[i].GetTankState() == Spawn){
			if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0) {
				EnemyList[i].SetIfBattle(false);
			}
		}
		// 當Enemy 為 _IfBattle = false 而且 EnemyTypeList 都沒生成完畢
		// 將Enemy 設定 _IfBattle = true 並重新計時
		if (!EnemyList[i].GetIfBattle() && !(EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0)){
			if (!EnemyList[i].GetIfBattle() && clock() - _TimerSpawn >= 2000) {
				RandomSpawnTank(i);
				EnemyList[i].SetIfBattle(true);
				_TimerSpawn = clock();
			}
		}
		else if (EnemyList[i].GetIfBattle()){
			if(EnemyList[i].GetTankState() == Live && !EnemyList[i].GetIfGetTimeStop()) {
				EnemyTankMove(&EnemyList[i]);
				if (EnemyList[i].GetIfFire(1) == false && clock() - EnemyFireLastTime[i] >= 1000) {
					EnemyList[i].FireBullet(1);
					EnemyFireLastTime[i] = clock();
				}
			}
			else if (EnemyList[i].GetTankState() == Spawn && !EnemyList[i].GetEnemySetInit()) {
				RandomSpawnTank(i);
				_EnemyQuantity += 1;
				if (_EnemyQuantity % 4 == 1){
					event.TrigReSetProps(_Prop);
					EnemyList[i].SetEnemyHaveItem(true);
				}
			}
		}
	}
	AllBulletCollision();
	AllBulletFly();
	_TimerFinish = clock();
	if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0 &&
		EnemyList[0].GetTankState() == Spawn && EnemyList[1].GetTankState() == Spawn && EnemyList[2].GetTankState() == Spawn && EnemyList[3].GetTankState() == Spawn &&
		!(EnemyList[0].GetIfBattle()  && EnemyList[1].GetIfBattle() && EnemyList[2].GetIfBattle() && EnemyList[3].GetIfBattle())) {
		_IfBattling = false;
		_IfSettling = true;
		Stage1.SetIfShowMap(false);
		event.TrigSettlement(_Menu, _AllStageEnemy[_NowStage - 1], _NowTotalScore, _TheHighestScore,_NowStage);
		//GotoGameState(GAME_STATE_RUN);
	}
}
void CGameStateRun::OnInit()                                  // 遊戲的初值及圖形設定
{
	srand((unsigned)time(NULL));
	_NowStage = -1;
	_IfBattling = false;
	_IfSettling = false;
	_Menu.LoadBitMap();
	_isHoldDownKey = _isHoldUpKey = _isHoldLeftKey = _isHoldRightKey = false;
	_IfEatItem = {0,0,0,0};

	_MouseX = 0;
	_MouseY = 0;
	_OnIceCountDown = 0;
	_PlayerTank.LoadBitmap();
	_PlayerTankFrontX = 0;
	_PlayerTankFrontY = 0;
	
	_NowProp = 0;
	for (int i = 0; i < 5; i++) {
		_Prop.push_back(GameProps());
		_Prop[i].OnInit();
	}

	_TimerSpawn = clock();
	EnemyList.push_back(_EnemyTank1);
	EnemyList.push_back(_EnemyTank2);
	EnemyList.push_back(_EnemyTank3);
	EnemyList.push_back(_EnemyTank4);
	for (int i = 0; i < 4; i++){
		EnemyList[i].LoadBitmap();
		EnemyFireLastTime.push_back(clock());
	}
	_EnemyQuantity = 4;
	for (int i = 0; i < 6; i++) {
		if (i == 0) {
			_AllBullet.push_back(&_PlayerTank._Bullet);
		}
		else if (i == 1) {
			_AllBullet.push_back(&_PlayerTank._SecondBullet);
		}
		else if (i >= 2) {
			_AllBullet.push_back(&EnemyList[i - 2]._Bullet);
		}
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (_IfBattling) {
		if (nChar == 0x5A || nChar == 0x58) {
			if (_PlayerTank.GetLevel() > 3 && _PlayerTank.GetIfFire(1) == true) {
				_PlayerTank.FireBullet(2);
			}
			_PlayerTank.FireBullet(1);
		}
		if (nChar == VK_DOWN)	_isHoldDownKey = true;
		if (nChar == VK_UP)		_isHoldUpKey = true;
		if (nChar == VK_LEFT)	_isHoldLeftKey = true;
		if (nChar == VK_RIGHT)	_isHoldRightKey = true;
		if (nChar == VK_DOWN || nChar == VK_RIGHT || nChar == VK_LEFT || nChar == VK_UP) _HoldKey = nChar;
		if (nChar == 0x41) {
			_IfBattling = false;
			_IfSettling = true;
			Stage1.SetIfShowMap(false);
			event.TrigSettlement(_Menu, _AllStageEnemy[_NowStage - 1], _NowTotalScore, _TheHighestScore,_NowStage);
		}
	}	
	else {
		if (!_IfSettling) {
			_NowStage = _Menu.OnKeyDown(nChar, nRepCnt, nFlags);
		}
		else {
			if (_Menu.OnKeyDown(nChar, nRepCnt, nFlags) == 1) {
				_NowStage += 1;
				_IfSettling = false;
			}
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (_IfBattling) {
		if (nChar == VK_DOWN)	_isHoldDownKey = false;
		if (nChar == VK_UP)		_isHoldUpKey = false;
		if (nChar == VK_LEFT)	_isHoldLeftKey = false;
		if (nChar == VK_RIGHT)	_isHoldRightKey = false;

		if (!_isHoldDownKey && !_isHoldUpKey && !_isHoldLeftKey && !_isHoldRightKey) {
			_tempcollision = Stage1.GetFrontGridsIndex(_PlayerTank.GetTankFront());
			if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 3 &&
				Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 3) {
				_OnIceCountDown = 64;
			}
		}
	}
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
	
	_Menu.OnShow();
	if (_IfBattling) {
		Stage1.OnShow();
		if (_NowProp != 0) {
			_Prop[_NowProp-1].OnShow();
		}
		_PlayerTank.OnShow();
		for (int i = 0; i < 4; i++) {
			EnemyList[i].OnShow();
		}
		if (Stage1.GetIfGrassInMap()) {
			Stage1.OnShowGrass();
		}
	}
	OnShowText();
}
void CGameStateRun::OnShowText() {
	CDC *pDC = CDDraw::GetBackCDC();
	CFont *fp;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 180, 0));
	_TimerFinish = clock();
	CTextDraw::Print(pDC, 0, 0, (to_string(_TimerSpawn / CLOCKS_PER_SEC) + " " + to_string(_TimerFinish)));
	CTextDraw::Print(pDC, 0, 25, (to_string(_EnemyQuantity)));
	CTextDraw::Print(pDC, 0, 50, (to_string(_MouseX) + " " + to_string(_MouseY)));
	
	CTextDraw::Print(pDC, 0, 150, (to_string(Stage1.GetEnemySignNum())));
	_tempcollision = Stage1.GetFrontGridsIndex(_PlayerTank.GetTankFront());
	CTextDraw::Print(pDC, 0, 75, (to_string(_tempcollision[0][0])+ "," + to_string(_tempcollision[0][1]) +" "+ to_string(NowXGrid(EnemyList[0].GetX1())) +"," +to_string(NowYGrid(EnemyList[0].GetY1()) + 1)));
	CTextDraw::Print(pDC, 0, 100, (to_string(_tempcollision[1][0]) + "," + to_string(_tempcollision[1][1]) + " " + to_string(NowXGrid(EnemyList[0].GetX1()) + 1) + "," + to_string(NowYGrid(EnemyList[0].GetY1()) + 1)));
	CTextDraw::Print(pDC, 0, 125, (to_string(EnemyTankCollision(&_PlayerTank))));
	_Menu.OnShowText(pDC, fp);
	for (int i = 0; i < 4; i++){
		if (EnemyList[i].GetTankState() == Death) {
			EnemyList[i].OnShowScore(pDC, fp);
		}
	}
	if (_IfEatItem[0] && clock() - _IfEatItem[3] <= 300) {
		CTextDraw::ChangeFontLog(pDC, 48, "STZhongsong", RGB(255, 255, 255));
		CTextDraw::Print(pDC, _IfEatItem[1], _IfEatItem[2], to_string(500));
	}
	CDDraw::ReleaseBackCDC();
}
void CGameStateRun::PlayerTankMove(CPlayer *tank) {
	if ((_isHoldRightKey == true || 
		_isHoldLeftKey == true || 
		_isHoldDownKey == true || 
		_isHoldUpKey == true) && 
		tank->GetTankState() == Live)
	{
		tank->TurnFace(_HoldKey);
		PlayerTankCollisionMap(tank);
	}
	else if ( _OnIceCountDown > 0) {
		tank->TurnFace(_HoldKey);
		PlayerTankCollisionMap(tank);
		_OnIceCountDown -= 4;
	}
}
void CGameStateRun::EnemyTankMove(Enemy *tank) {
	tank->EnemyRandomDirection();
	EnemyTankCollisionMap(tank);
}

void CGameStateRun::AllBulletCollision() {
	for (int i = 0; i < 6; i++) {
		if (_AllBullet[i]->GetAlreadyFire()) {
			if (i <= 1) {
				for (auto& enemy : EnemyList) {
					if (CMovingBitmap::IsOverlap(_AllBullet[i]->GetBitmap(), enemy.GetTankBitmap())
						//&& _PlayerTank.GetBulletOwner() == 1
						&& enemy.GetTankState() == Live) {
						event.TrigUpDateMap(Stage1, _EnemyNum);
						enemy.SetLife(0);
						_EnemyNum -= 1;
						if (enemy.isEnemyHaveItem()){
							if (_NowProp < 5) {
								event.TrigSetProps(_Prop, _NowProp);
								enemy.SetEnemyHaveItem(false);
							}
							_NowProp += 1;
						}
						_PlayerTank.SetBulletStatus(1 + i, false);
						_PlayerTank.SetIfFire(1 + i, false);
						break;
					}
					else if (CMovingBitmap::IsOverlap(_AllBullet[i]->GetBitmap(), enemy.GetBulletBitmap())
						&& enemy.GetIfFire(1) && _PlayerTank.GetIfFire(1+i)) {
						_PlayerTank.SetBulletStatus(1 + i, false);
						_PlayerTank.SetIfFire(1 + i, false);
						enemy.SetBulletStatus(1, false);
						enemy.SetIfFire(1, false);
						_NowTotalScore += enemy.GetEnemyScore();
						break;
					}
				}
				if (ShootCollision(*_AllBullet[i], _PlayerTank.GetLevel()) && _AllBullet[i]->GetAlreadyFire()) {
					_PlayerTank.SetBulletStatus(1+i, false);
					_PlayerTank.SetIfFire(1+i, false);
				}
			}
			else {
				if (CMovingBitmap::IsOverlap(_AllBullet[i]->GetBitmap(), _PlayerTank.GetTankBitmap())
						&& _PlayerTank.GetTankState() == Live ) {
					if (!_PlayerTank.GetIfInvicible()) {
						//_PlayerTank.SetLife(0);
					}
					EnemyList[i - 2].SetBulletStatus(1, false);
					EnemyList[i - 2].SetIfFire(1,false);
					continue;
				}
				if (ShootCollision(*_AllBullet[i], EnemyList[i-2].GetLevel())) {
					EnemyList[i - 2].SetBulletStatus(1, false);
					EnemyList[i - 2].SetIfFire(1, false);
				}
			}
		}
	}
}

void CGameStateRun::AllBulletFly() {
	for (int i = 0; i < 6; i++) {
		if (_AllBullet[i]->GetAlreadyFire()) {
			_AllBullet[i]->BulletFly();
		}
	}
}

bool CGameStateRun::ShootCollision(CBullet Bullet, int TankLevel) {
	if (Stage1.GetIfBoardEdge(Bullet.GetNowBackPlace()[0][0], Bullet.GetNowBackPlace()[0][1]
		, Bullet.GetHeight(), Bullet.GetWidth(), Bullet.GetDirection()) == true) {
		_tempcollision = Stage1.GetFrontGridsIndex(Bullet.GetNowFrontPlace());
		if (Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0],Stage1.CanShoot) == true
			|| Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanShoot) == true) {
			if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 4 || Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 5) {
				Stage1.ShootWall(Bullet.GetDirection(), TankLevel, _tempcollision[0][1], _tempcollision[0][0]);
			}
			if (Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 4 || Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 5) {
				Stage1.ShootWall(Bullet.GetDirection(), TankLevel, _tempcollision[1][1], _tempcollision[1][0]);
			}
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}

void CGameStateRun::PlayerTankCollisionMap(CPlayer *tank) {
	tank->TankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if (Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0],Stage1.CanWalk) &&
			Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
			if (!EnemyTankCollision(tank)){
				tank->Move();
			}
		}
		if (tank->GetIfGetShip()) {
			if ((Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 2 ||
				Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 2)) {
				if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], Stage1.CanWalk) ||
					Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
					tank->Move();
				}
				else if (Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 2 &&
					Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 2) {
					tank->Move();
				}
			}
		}
	}
	tank->Animation();
}
void CGameStateRun::EnemyTankCollisionMap(Enemy *tank) {
	tank->TankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if (Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], Stage1.CanWalk) &&
			Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], Stage1.CanWalk))) {
			if (!EnemyTankCollision(tank)) {
				tank->Move();
			}
		}
	}
	tank->Animation();
}

//bool CGameStateRun::PlayerTankCollision() {
//	for (int i = 0; i < 4; i++) {
//		return TankCollision(&_PlayerTank, &EnemyList[i]);
//	}
//}

bool CGameStateRun::EnemyTankCollision(CTank *tank) {
	bool _collision = false;
	for (int i = 0; i < 4; i++) {
		_collision |= TankCollision(tank, &EnemyList[i]);
	}
	return _collision || TankCollision(tank, &_PlayerTank);
}

bool CGameStateRun::TankCollision(CTank *tank, CTank *who) {
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	//_Tanktempcollision = Stage1.GetFrontGridsIndex(EnemyList[i].GetTankFront());
	if (tank->GetOriginAngle() == Right) {
		if ((_tempcollision[0][0] == NowXGrid(who->GetX1()) && _tempcollision[0][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) && _tempcollision[1][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1)||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[1][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1)) {
			return true;
		}
	}
	else if (tank->GetOriginAngle() == Down) {
		if ((_tempcollision[0][0] == NowXGrid(who->GetX1()) && _tempcollision[0][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) && _tempcollision[1][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1()))||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1 ) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) && _tempcollision[1][1] == NowYGrid(who->GetY1()) + 1) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1)) {
			return true;
		}
	}
	else if (tank->GetOriginAngle() == Left) {
		if ((_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[1][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1)||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 2 && _tempcollision[0][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) + 2 && _tempcollision[1][1] == NowYGrid(who->GetY1())) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 2 && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1)) {
			return true;
		}
	}
	else if (tank->GetOriginAngle() == Up) {
		if ((_tempcollision[0][0] == NowXGrid(who->GetX1()) && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 1) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) && _tempcollision[1][1] == NowYGrid(who->GetY1()) + 1)||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 2) ||
			(_tempcollision[0][0] == NowXGrid(who->GetX1()) + 1 && _tempcollision[0][1] == NowYGrid(who->GetY1()) + 2) ||
			(_tempcollision[1][0] == NowXGrid(who->GetX1()) && _tempcollision[1][1] == NowYGrid(who->GetY1()) + 2)) {
			return true;
		}
	}
	/*
	00   00  00
	11  11    11 

		0
	01  01   1
	01   1  01
			0    ->三種可能(X)->六種可能 因為移動會讓坦克的定位往前一點點但圖片看不出來所以會變成穿模 所以避免穿模就必須判斷六次
	*/
	return false;
}
// tempIndex先亂數決定要生成的TankType 
//如果TankType已生成完畢就繼續亂數生成直到尚未生成完畢的Tank
//生成完畢就break跳出while
//尚未生成完畢重設定SetEnemyType
void CGameStateRun::RandomSpawnTank(int num) {
	tempIndex = rand() % 4;							
	while (EnemyTypeList[tempIndex] == 0) {
		tempIndex = rand() % 4;
		if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0) {
			break;
		}
	}
	EnemyTypeList[tempIndex] -= 1;
	EnemyList[num].SetEnemyType(tempIndex);
}
int CGameStateRun::NowXGrid(int x) {
	return (x - 100) / 32;
}

int CGameStateRun::NowYGrid(int y) {
	return y / 32;
}