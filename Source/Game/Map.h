#pragma once

namespace game_framework {
	class Map { //�a�Ϫ�����(��l)
	public:
		Map(vector<vector<int>> stage); //�n�ǬY�@��stage���C�Ӯ�l type
		vector<vector<MapItem>> _Stage; //�C�@��stage
		int _BoardX; //�ѽL��x
		int _BoardY; //�ѽL��y
		void OnShow(); 
	};
}