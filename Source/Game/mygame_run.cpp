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
	if (_NowStage == -1 && !_IfBattling) { // NowStage == -1  代表正在選, == 1 ~ 35代表已經選完了
		event.TrigSelectingStage(ChooseStageScreen);
		return;
	}
	
	if (_NowStage >= 1 && !_IfBattling) {
		event.TrigSetBattleMap(_AllStage[_NowStage-1],Stage1, _EnemyNum,ChooseStageScreen);
		EnemyTypeList.assign(_AllStageEnemy[_NowStage - 1].begin(), _AllStageEnemy[_NowStage - 1].end());
		for (int i = 0; i < 4; i++){
			tempIndex = rand() % 4;
			while (EnemyTypeList[tempIndex] == 0) {
				tempIndex = rand() % 4;
				if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0) {
					break;
				}
			}
			EnemyTypeList[tempIndex] -= 1;
			EnemyList[i].SetEnemyType(tempIndex);
		}
		_IfBattling = true;
		_PlayerTank.SetIfBattle(true);
		for (int i = 0; i < 4; i++) {
			EnemyList[i].SetIfBattle(true);
		}
		return;
	}

	if ((CMovingBitmap::IsOverlap(_PlayerTank.GetTankBitmap(), Prop.GetPropBitmap()) || Prop.GetIfTouched())
		&& Prop.GetIfExist()) {
		event.TrigGetProps(Prop, Stage1, _PlayerTank,EnemyList);
	}
	if (_PlayerTank.GetTankState() == Live){
		PlayerTankMove(&_PlayerTank);
	}
	for (int i = 0; i < 4; i++) {
		if(EnemyList[i].GetTankState() == Live && !EnemyList[i].GetIfGetTimeStop()) {
			EnemyTankMove(&EnemyList[i]);
			if (EnemyList[i].GetIfFire(1) == false && clock() - EnemyFireLastTime[i] >= 1000) {
				EnemyList[i].FireBullet(1);
				EnemyFireLastTime[i] = clock();
			}
		}
		else if (EnemyList[i].GetTankState() == Spawn && !EnemyList[i].GetEnemySetInit()) {
			tempIndex = rand() % 4;
			while (EnemyTypeList[tempIndex] == 0) {
				tempIndex = rand() % 4;
				if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0) {
					break;
				}
			}
			EnemyTypeList[tempIndex] -= 1;
			EnemyList[i].SetEnemyType(tempIndex);
			_EnemyQuantity += 1;
			if (_EnemyQuantity % 4 == 1){
				event.TrigUnshowProps(Prop);
				EnemyList[i].SetEnemyHaveItem(true);
			}
		}
		else if (EnemyList[i].GetTankState() == Death){
			if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0) {
				EnemyList[i].SetIfBattle(false);
			}
		}
	}
	AllBulletCollision();
	AllBulletFly();
	_TimerFinish = clock();
	if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0 &&
		EnemyList[0].GetTankState() == Death && EnemyList[1].GetTankState() == Death && EnemyList[2].GetTankState() == Death && EnemyList[3].GetTankState() == Death) {
		GotoGameState(GAME_STATE_OVER);
	}
}
void CGameStateRun::OnInit()                                  // 遊戲的初值及圖形設定
{
	srand((unsigned)time(NULL));
	_NowStage = -1;
	_IfBattling = false;
	ChooseStageScreen.LoadBitMap();
	_isHoldDownKey = _isHoldUpKey = _isHoldLeftKey = _isHoldRightKey = false;
	
	_MouseX = 0;
	_MouseY = 0;
	_OnIceCountDown = 0;
	_PlayerTank.LoadBitmap();
	_PlayerTankFrontX = 0;
	_PlayerTankFrontY = 0;
	Prop.OnInit();
	//event.TrigSetProps(Prop);
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
	}	
	else {
		_NowStage = ChooseStageScreen.OnKeyDown(nChar, nRepCnt, nFlags);
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
	ChooseStageScreen.OnShow();
	if (_IfBattling) {
		Stage1.OnShow();
		Prop.OnShow();
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
	CTextDraw::Print(pDC, 0, 0, (to_string(_TimerStart / CLOCKS_PER_SEC) + " " + to_string(_TimerFinish)));
	CTextDraw::Print(pDC, 0, 25, (to_string(_EnemyQuantity)));
	/*CTextDraw::Print(pDC, 0, 50, (to_string(EnemyList[0].isEnemyHaveItem())));
	CTextDraw::Print(pDC, 0, 75, (to_string(EnemyList[1].isEnemyHaveItem())));
	CTextDraw::Print(pDC, 0, 100, (to_string(EnemyList[2].isEnemyHaveItem())));
	CTextDraw::Print(pDC, 0, 125, (to_string(EnemyList[3].isEnemyHaveItem())));*/
	if ((_NowStage - 1)>=0){
		CTextDraw::Print(pDC, 0, 50, (to_string(EnemyTypeList[0]) + " " + to_string(_AllStageEnemy[_NowStage - 1][0])));
		CTextDraw::Print(pDC, 0, 75, (to_string(EnemyTypeList[1]) + " " + to_string(_AllStageEnemy[_NowStage - 1][1])));
		CTextDraw::Print(pDC, 0, 100, (to_string(EnemyTypeList[2]) + " " + to_string(_AllStageEnemy[_NowStage - 1][2])));
		CTextDraw::Print(pDC, 0, 125, (to_string(EnemyTypeList[3]) + " " + to_string(_AllStageEnemy[_NowStage - 1][3])));
	}
	CTextDraw::Print(pDC, 0, 150, (to_string(_NowStage - 1)));
	ChooseStageScreen.OnShowText(pDC, fp);
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
		TankCollisionMap(tank);
	}
	else if ( _OnIceCountDown > 0) {
		tank->TurnFace(_HoldKey);
		TankCollisionMap(tank);
		_OnIceCountDown -= 4;
	}
}
void CGameStateRun::EnemyTankMove(Enemy *tank) {
	tank->EnemyRandomDirection();
	TankCollisionMap(tank);
}
void CGameStateRun::AllBulletCollision() {
	for (int i = 0; i < 6; i++) {
		if (_AllBullet[i]->GetAlreadyFire()) {
			if (i <= 1) {
				for (auto& enemy : EnemyList) {
					if (CMovingBitmap::IsOverlap(_AllBullet[i]->GetBitmap(), enemy.GetTankBitmap())
						//&& _PlayerTank.GetBulletOwner() == 1
						&& enemy.GetTankState() == Live) {
						enemy.SetLife(0);
						if (enemy.isEnemyHaveItem()){
							event.TrigSetProps(Prop);
							enemy.SetEnemyHaveItem(false);
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
						&& _PlayerTank.GetTankState() == Live) {
						_PlayerTank.SetLife(0);
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
		if (Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 1) == true
			|| Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 1) == true) {
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
void CGameStateRun::TankCollisionMap(CTank *tank) {
	tank->TankFront();
	_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
	if (Stage1.GetIfBoardEdge(tank->GetX1(), tank->GetY1(), tank->GetHeight(), tank->GetWidth(), tank->GetOriginAngle())) {
		if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 0) &&
			Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 0))) {
			tank->Move();
		}
		if (tank->GetIfGetShip()) {
			if (((Stage1.GetType(_tempcollision[0][1], _tempcollision[0][0]) == 2 ||
				Stage1.GetType(_tempcollision[1][1], _tempcollision[1][0]) == 2))) {
				if ((Stage1.GetMapItemInfo(_tempcollision[0][1], _tempcollision[0][0], 0) ||
					Stage1.GetMapItemInfo(_tempcollision[1][1], _tempcollision[1][0], 0))) {
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