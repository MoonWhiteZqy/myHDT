#include "minions.h"
#include<iostream>
#include<random>
#include<ctime>

#define MOUSE 1

using namespace std;

//删除已经死去的随从
int clearMinion(vector<minion*>& infos) {
	int res = 0;
	int i;
	int j;

	for (i = 0; i < infos.size(); i++) {
		if (!infos[i]->is_alive()) {//发现列表中有死去的随从
			res++;
			//发现老鼠的亡语
			if (infos[i]->has_death() == MOUSE) {
				minion* temp = infos[i];
				int rest = 8 - infos.size();
				int exist = infos.size();
				if (temp->read_attack() < rest) {//触发亡语后有多余空位
					infos.resize(infos.size() + temp->read_attack() - 1);
					for (j = 1; j <= temp->read_attack() && i + j < exist; j++) {
						infos[i + j + temp->read_attack() - 1] = infos[i + j];
					}
					for (j = 0; j < temp->read_attack(); j++) {
						infos[i + j] = new minion("小老鼠", 1, 1);
					}
					i += temp->read_attack();
					delete temp;
					continue;
				}
				else {//格子是满的
					infos.resize(7);
					for (j = i + 1; j < exist; j++) {
						infos[7 - exist + j] = infos[j];
					}
					for (j = i; j < 8 - exist + i; j++) {
						infos[j] = new minion("小老鼠", 1, 1);
					}
					delete temp;
					continue;
				}
			}
			delete (infos[i]);
			for (j = i; j < infos.size() - 1; j++) {
				infos[j] = infos[j + 1];
			}
			infos.resize(infos.size() - 1);
		}
	}

	return res;
}

//输出当前vector里存的随从
void showMinion(vector<minion*>& infos) {
	int health;
	int attack;
	string name;
	//cout << "-------------------------" << endl;
	//cout << "当前随从为:" << endl;
	for (auto p : infos) {
		health = p->read_health();
		attack = p->read_attack();
		name = p->read_name();
		cout << name << ' ' << attack << '/' << health;
		if (p->has_shield())
			cout << " 圣盾";
		if (p->has_taunt())
			cout << " 嘲讽";
		if (p->has_death())
			cout << " 亡语";
		cout << endl;
	}
	cout << "展示完毕" << endl;
	cout << "-------------------------" << endl;
}

int battle(vector<minion*>& here, vector<minion*>& there) {
	int pm = 0;
	int pn = 0;
	int i;
	vector<minion*> mytaunt;
	vector<minion*> youtaunt;
	
	while (here.size() && there.size()) {

		youtaunt.clear();
		for (i = 0; i < there.size(); i++) {
			if (there[i]->has_taunt()) {
				youtaunt.push_back(there[i]);
			}
		}

		if (youtaunt.size() == 0)//敌方没有嘲讽
			here[pm % (here.size())]->hit(there[rand() % (there.size())]);
		else//攻击嘲讽的随从
			here[pm % (here.size())]->hit(youtaunt[rand() % (youtaunt.size())]);

		if (here[pm % (here.size())]->is_alive())//当前随从没有死亡，下一随从进行下次进攻
			pm++;
		clearMinion(here);
		clearMinion(there);

		

		if (!(here.size() && there.size()))//出现一方已经空场
			break;

		mytaunt.clear();
		for (i = 0; i < here.size(); i++) {
			if (here[i]->has_taunt()) {
				mytaunt.push_back(here[i]);
			}
		}

		if (mytaunt.size() == 0)//我方没有嘲讽
			there[pn % (there.size())]->hit(here[rand() % (here.size())]);
		else
			there[pn % (there.size())]->hit(mytaunt[rand() % (mytaunt.size())]);
		if (there[pn % (there.size())]->is_alive())
			pn++;
		clearMinion(here);
		clearMinion(there);

		
	}
	//showMinion(here);
	//showMinion(there);
	if (here.size() > 0)//胜利
		return 1;
	else if (there.size() > 0)//失败
		return 0;
	else {
		return -2;//平局
	}
}

void judgeWin(int& win, int& deal, int& lose, int end) {
	if (end == -2) {
		deal++;
	}
	else if (end == 1)
		win++;
	else
		lose++;

}

int main() {
	int win = 0;
	int deal = 0;
	int lose = 0;
	int i;
	int end;
	int n = 1000;
	int state;
	srand((int)time(NULL));
	cout << "嘿我最喜欢的指挥官回来了，准备来一局吗" << endl << endl;
	for (i = 0; i < n; i++) {
		minion* my1 = new minion("瘟疫鼠群5", 5, 5, 0, 0, 0, 0, MOUSE, 0);
		//minion* my2 = new minion("我2", 1, 1, 1, 1);
		minion* my3 = new minion("瘟疫鼠群4", 4, 4, 0, 0, 0, 0, MOUSE, 0);
		//minion* my4 = new minion("我4", 4, 4);
		minion* my5 = new minion("我5", 2, 2);
		//minion* my6 = new minion("我6", 6, 6);
		//minion* my7 = new minion("我7", 10, 10);
		//minion* you1 = new minion("你1", 1, 1);
		//minion* you2 = new minion("你2", 2, 2);
		//minion* you3 = new minion("你3", 3, 3);
		//minion* you4 = new minion("你4", 4, 4);
		//minion* you5 = new minion("你5", 2, 2);
		minion* you6 = new minion("你6", 2, 1);
		minion* you7 = new minion("你7", 10, 16);
		vector<minion*> here;
		vector<minion*> there;
		//here = { my7, my6, my5, my4, my3, my2, my1 };
		//there = { you7, you6, you5, you4, you3, you2, you1 };
		here = { my3, my1, my5 };
		there = { you7 };
		//showMinion(here);
		//showMinion(there);
		if (i == 0) {
			cout << "你的随从:" << endl;
			showMinion(here);
			cout << "敌方随从:" << endl;
			showMinion(there);
		}
		if (here.size() == there.size()) {//随从数量一样多，先后手都可能
			state = rand() % 2;//1为here先手，0为后手
			if (state)
				end = battle(here, there);
			else
				end = battle(there, here);
			if (end == -2) {
				deal++;
			}
			else {
				if (state ^ end)
					lose++;
				else
					win++;
			}
		}
		else {
			if (here.size() > there.size()) {
				end = battle(here, there);
				judgeWin(win, deal, lose, end);
			}
			else {
				end = battle(there, here);
				judgeWin(lose, deal, win, end);
			}
		}
	}
	cout << "胜利" << win << "次,平局" << deal << "次,失利" << lose << "次" << endl << endl;
	if (win > lose) {
		cout << "你可真是个人才啊朋友" << endl;
	}
	else {
		cout << "我给你物色了一些亲人" << endl;
	}
	system("pause");
	return 0;
}