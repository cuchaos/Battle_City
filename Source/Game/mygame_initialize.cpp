#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	Sleep(1000);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
	_Lobby.LoadBitMap();
	_MouseX = 0;
	_MouseY = 0;
}

void CGameStateInit::OnBeginState()
{
}
void CGameStateInit::OnMove() {
	event.TrigLobbyMenu(_Lobby);
}
void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	_1POr2P = _Lobby.OnKeyDown(nChar, nRepCnt, nFlags);
	if ( _1POr2P != -1) {
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
}
void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point) {
	_MouseX = point.x;
	_MouseY = point.y;
}
void CGameStateInit::OnShow()
{
	_Lobby.OnShow();
	OnShowText();
}
void CGameStateInit::OnShowText() {
	CDC *pDC = CDDraw::GetBackCDC();
	//CFont *fp;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 180, 0));
	CTextDraw::Print(pDC, 0, 0, (to_string(_MouseX) + " " + to_string(_MouseY).c_str()));
	//_Lobby.OnShowText(pDC, fp);

	CDDraw::ReleaseBackCDC();
}