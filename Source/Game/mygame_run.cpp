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
	switch(state) {
		case Menu:
			event.TrigSelectingStage(_Menu);
			break;
		case prebattle:
			event.TrigSetBattleMap(_AllStage[_NowStage-1],Stage1, _EnemyNum,_Menu,_PlayerTank,_Prop);
			EnemyTypeList.assign(_AllStageEnemy[_NowStage - 1].begin(), _AllStageEnemy[_NowStage - 1].end());
			break;
		case battle:
			TrigAllProp();
			if (_PlayerTank.GetTankState() == _PlayerTank.Alive){
				PlayerTankMove(&_PlayerTank);
			}
			AllEnemyMove();
			AllBulletCollision();
			AllBulletFly();
			_TimerFinish = clock();
			EnemyReSpawn();
			break;
	}
<<<<<<< Updated upstream
	// state machine transformation
	switch(state) {
		case Menu:
			if ( _NowStage != -1 ) state = prebattle;
			break;
		case prebattle:
			state = battle;
			break;
		case battle:
			if( _IfSettling == true ) state = prebattle;
			break;
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
	
	_NowPropSize = 0;
	for (int i = 0; i < 5; i++) {
		_Prop.push_back(GameProps());
		_Prop[i].OnInit();
	}

	_TimerSpawn = clock();

	for (int i=0; i<4; ++i) {
		EnemyList[i].LoadBitmap();
		EnemyFireLastTime[i] = clock();
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
		if (nChar == 'A') {
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
		if (_NowPropSize != 0) {
			_Prop[_NowPropSize-1].OnShow();
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
	/*CTextDraw::Print(pDC, 0, 0, (to_string(_TimerSpawn / CLOCKS_PER_SEC) + " " + to_string(_TimerFinish)));*/
	CTextDraw::Print(pDC, 0, 25, (to_string(_EnemyQuantity)));
	CTextDraw::Print(pDC, 0, 50, (to_string(_MouseX) + " " + to_string(_MouseY)));
	CTextDraw::Print(pDC, 0, 150, (to_string(Stage1.GetEnemySignNum())));
	CTextDraw::Print(pDC, 0, 125, (to_string(EnemyTankCollision(&_PlayerTank))));
	_Menu.OnShowText(pDC, fp);
	
	/*CTextDraw::Print(pDC, 0, 50, (to_string(_MouseX) + " " + to_string(_MouseY)));*/
	CTextDraw::ChangeFontLog(pDC, 15, "STZhongsong", RGB(255, 255, 255));
	//CTextDraw::Print(pDC, 0, 150, (to_string(Stage1.GetEnemySignNum())));
	//CTextDraw::Print(pDC, 0, 125, (to_string(_EnemyNum)));

	for (int i = 0; i < 4; i++) {
		CTextDraw::Print(pDC, 0 + i * 25, 125, (to_string(EnemyTypeList[i])));
		CTextDraw::Print(pDC, 0 + i * 25, 150, (to_string(EnemyList[i].GetTankState())));
	}
	_Menu.OnShowText(pDC, fp);
	
	for (int i = 0; i < 4; i++){
		if (EnemyList[i].GetTankState() == EnemyList[i].Death) {
			EnemyList[i].OnShowScore(pDC, fp);
		}
	}
	if (_IfEatItem[0] && clock() - _IfEatItem[3] <= 300) {
		CTextDraw::ChangeFontLog(pDC, 48, "STZhongsong", RGB(255, 255, 255));
		CTextDraw::Print(pDC, _IfEatItem[1], _IfEatItem[2], to_string(500));
	}
	CDDraw::ReleaseBackCDC();
}
void CGameStateRun::AllEnemyReSpawn() {
	for (int i = 0; i < 4; i++) {
		auto& enemy = EnemyList[i];
		if (_EnemyNum > 0 && enemy.GetTankState() == enemy.Death) {
			RandomSpawnTank(i);
			_EnemyNum -= 1;
		}
	}
}

bool CGameStateRun::IfNoEnemy() {
	for (int i = 0; i < 4; i++) {
		auto& enemy = EnemyList[i];
		if (EnemyTypeList[i] != 0 || enemy.GetTankState() != enemy.Death) {
			return false;
		}
	}
	return true;
}
bool CGameStateRun::IfResetPropTime(int NowPropIndex, GameProps NowProp) {
	GameProps::ItemType NowPropType = _Prop[NowPropIndex].GetType();
	if (NowPropIndex == _NowPropSize - 1 && _Prop[NowPropIndex].count(NowPropType) > 1
		&& (NowPropType == GameProps::ItemType::Clock
		|| NowPropType == GameProps::ItemType::Shovel
		|| NowPropType == GameProps::ItemType::Steel_helmet) ) {
		_Prop[_Prop[NowPropIndex].find(NowPropType)].SetIfCountDown(false);
		_Prop[NowPropIndex].SetIfExist(false);
		return true;
	}
	return false;
}
void CGameStateRun::TrigAllProp() {
	for (int i = _NowPropSize - 1; i > -1; i--) {
		if ((CMovingBitmap::IsOverlap(_PlayerTank.GetTankBitmap(), _Prop[i].GetPropBitmap())
			|| _Prop[i].GetIfTouched()) && _Prop[i].GetIfExist()) {
			if (IfResetPropTime(i, _Prop[i])) {
				continue;
			}
			_IfEatItem[0] = 1;
			_IfEatItem[1] = _Prop[i].GetX();
			_IfEatItem[2] = _Prop[i].GetY();
			_IfEatItem[3] = clock();
			event.TrigGetProps(_Prop[i], Stage1, _PlayerTank, EnemyList);
		}
	}
}
void CGameStateRun::AllEnemyMove() {
	for (int i = 0; i < 4; i++) {
		auto& enemy = EnemyList[i];
		switch (enemy.GetTankState())
		{
		case Enemy::Spawn:
			break;
		case Enemy::Alive:
			EnemyTankMove(i);
			break;
		case Enemy::Death:
			enemy.TankExpolsion();
			break;
		}
	}
}
void CGameStateRun::PlayerTankMove(CPlayer *tank) {
	if ((_isHoldRightKey == true || 
		_isHoldLeftKey == true || 
		_isHoldDownKey == true || 
		_isHoldUpKey == true) && 
		tank->GetTankState() == tank->Alive)
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
void CGameStateRun::EnemyTankMove(int Order) {
	EnemyList[Order].EnemyRandomDirection();
	if (EnemyList[Order].GetIfFire(1) == false && clock() - EnemyFireLastTime[Order] >= 1000) {
		EnemyList[Order].FireBullet(1);
		EnemyFireLastTime[Order] = clock();
	}
	EnemyTankCollisionMap(&EnemyList[Order]);
}
bool CGameStateRun::BulletHitTank(CBullet CurrentBullet, CTank *BulletOwner, CTank *DetectTarget,BulletOrder Order) {
	if (CMovingBitmap::IsOverlap(CurrentBullet.GetBitmap(), DetectTarget->GetTankBitmap())
		&& DetectTarget->GetTankState() == DetectTarget->Alive) {
		BulletOwner->SetBulletStatus(Order, false);
		BulletOwner->SetIfFire(Order, false);
		return true;
	}
	return false;
}
bool CGameStateRun::BulletHitBullet(CBullet CurrentBullet, CTank *BulletOwner, CTank *DetectTarget, BulletOrder Order) {
	if (CMovingBitmap::IsOverlap(CurrentBullet.GetBitmap(), DetectTarget->GetBulletBitmap())
		&& DetectTarget->GetIfFire(1) && BulletOwner->GetIfFire(Order)) {
		_PlayerTank.SetBulletStatus(Order, false);
		_PlayerTank.SetIfFire(Order, false);
		DetectTarget->SetBulletStatus(1, false);
		DetectTarget->SetIfFire(1, false);
		return true;
	}
	return false;
}
void CGameStateRun::PlayerBulletCollision(BulletOrder Order) {
	auto& CurrentBullet = Order == FirstBullet ? _AllBullet[0] : _AllBullet[1];
	for (auto& enemy : EnemyList) {
		if (BulletHitTank(*CurrentBullet, &_PlayerTank, &enemy, Order)) {
			enemy.SetLife(0);
			if (enemy.GetEnemyHaveItem()) {
				event.TrigSetProps(_Prop, _NowPropSize);
				enemy.SetEnemyHaveItem(false);
				_NowPropSize += 1;
			}
			_NowTotalScore += enemy.GetEnemyScore();
			break;
		}
		if (BulletHitBullet(*CurrentBullet, &_PlayerTank, &enemy, Order)) {
			break;
		}
	}
	if (ShootCollision(*CurrentBullet, _PlayerTank.GetLevel()) && CurrentBullet->GetAlreadyFire()) {
		_PlayerTank.SetBulletStatus(Order, false);
		_PlayerTank.SetIfFire(Order, false);
	}
}
void CGameStateRun::EnemyAllBulletCollision() {
	for (const auto &i : { 2,3,4,5 }) { // Enemy's bullet
		if (!_AllBullet[i]->GetAlreadyFire()) continue;
		if (BulletHitTank(*_AllBullet[i], &EnemyList[i - 2], &_PlayerTank, FirstBullet)) {
			if (!_PlayerTank.GetIfInvicible()) {
				//_PlayerTank.SetLife(0);
			}
			continue;
		}
		if (ShootCollision(*_AllBullet[i], EnemyList[i - 2].GetLevel())) {
			EnemyList[i - 2].SetBulletStatus(1, false);
			EnemyList[i - 2].SetIfFire(1, false);
		}
	}
}
void CGameStateRun::AllBulletCollision() {
	for (const auto &i : {0,1} ) { // player's bullet
		if ( ! _AllBullet[i]->GetAlreadyFire()) continue; 
		switch (i)
		{
		case 0:
			PlayerBulletCollision(FirstBullet);
			break;
		case 1:
			PlayerBulletCollision(SecondBullet);
			break;
		}
	}
	EnemyAllBulletCollision();
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
bool CGameStateRun::EnemyTankCollision(CTank *tank) {
	bool _collision = false;
	for (int i = 0; i < 4; i++) {
		_collision |= TankCollision(tank, &EnemyList[i]);
	}
	return _collision || TankCollision(tank, &_PlayerTank);
}
bool CGameStateRun::TankCollision(CTank *tank, CTank *who) {
	if (who->GetTankState()== who->Alive){
		_tempcollision = Stage1.GetFrontGridsIndex(tank->GetTankFront());
		switch (tank->GetOriginAngle())
		{
		case Right:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1)) {
				return true;
			}
			break;
		case Down:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1)) {
				return true;
			}
			break;
		case Left:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 2 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) + 2 && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1())) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 2 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1)) {
				return true;
			}
			break;
		case Up:
			if ((_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1()) + 1) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 2) ||
				(_tempcollision[0][0] == Stage1.GetGridIndexX(who->GetX1()) + 1 && _tempcollision[0][1] == Stage1.GetGridIndexY(who->GetY1()) + 2) ||
				(_tempcollision[1][0] == Stage1.GetGridIndexX(who->GetX1()) && _tempcollision[1][1] == Stage1.GetGridIndexY(who->GetY1()) + 2)) {
				return true;
			}
			break;
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
	if (EnemyTypeList[0] == 0 && EnemyTypeList[1] == 0 && EnemyTypeList[2] == 0 && EnemyTypeList[3] == 0) {
		return;
	}
	tempIndex = rand() % 4;							
	while (EnemyTypeList[tempIndex] == 0) {
		tempIndex = rand() % 4;
	}
	EnemyTypeList[tempIndex] -= 1;
	EnemyList[num].SetEnemyType(tempIndex);
	EnemyList[num].SetEnemyReSpawn();
}
