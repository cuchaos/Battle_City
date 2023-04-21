#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include <fstream>
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
	ifstream myfile;
	myfile.open("MapRawData.txt");
	string content;
	for (int i = 0; i < 35; i++) { // give every stage row and col clean
		vector<vector<int>> temp_2D;
		for (int j = 0; j < 26; j++) {
			vector<int> temp_1D;
			for (int k = 0; k < 26; k++) {
				temp_1D.push_back(-1);
			}
			temp_2D.push_back(temp_1D);
		}
		_AllStage.push_back(temp_2D);
	}
	for (int i = 0; i < 35; i++) {
		getline(myfile, content); // we have a space in front of every stage
		for (int j = 0; j < 26; j++) {  //have 26 row
			getline(myfile, content);
			//cout<<content<<endl;
			int col = 0;
			for (int k = 0; k < (int)content.size(); k++) {
				if (content[k] != ',' && content[k] != '{' && content[k] != '}') {
					_AllStage[i][j][col] = content[k] - '0';
					col++;
				}
			}
		}
		getline(myfile, content);
	}
	myfile.close();
	myfile.open("EnemyTankData.txt");
	for (int i = 0; i < 35; i++) {
		vector<int> temp = { 0,0,0,0 };
		_AllStageEnemy.push_back(temp);
		getline(myfile, content);
		int lastindex = content.size() - 1;
		int vectorindex = 3;
		for (int j = content.size() - 1; j > -1; j--) {
			if (content[j] == ' ' || j == 0) {
				int temp_count = 0;
				int temp_first = 0;
				if (j == 0) {
					temp_first = j;
				}
				else {
					temp_first = j + 1;
				}
				for (int k = temp_first; k <= lastindex; k++) {
					if (k != lastindex) {
						temp_count += 10 * (content[k] - '0');
						continue;
					}
					temp_count += content[k] - '0';
				}
				_AllStageEnemy[i][vectorindex] = temp_count;
				lastindex = j - 1;
				vectorindex -= 1;
			}
		}
		getline(myfile, content);
	}
	myfile.close();
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
	/*CTextDraw::Print(pDC, 0, 50, (to_string(_AllStageEnemy[9][0])));
	CTextDraw::Print(pDC, 0, 75, (to_string(_AllStageEnemy[9][1])));
	CTextDraw::Print(pDC, 0, 100, (to_string(_AllStageEnemy[9][2])));
	CTextDraw::Print(pDC, 0, 125, (to_string(_AllStageEnemy[9][3])));*/
	//_Lobby.OnShowText(pDC, fp);
	CDDraw::ReleaseBackCDC();
}