#include "minions.h"
#include<iostream>
#include<random>
#include<ctime>
#include<queue>


using namespace std;

//删除已经死去的随从
int clearMinion(vector<minion*>& infos) {
	int res = 0;
	int i;
	int j;
	int idle;//确认亡语、复生的随从数量
	int state;
	int total = 0;//亡语等触发后随从的总数，最大为7
	queue<int> nextstate;
	vector<minion*> temp(7);

	for (i = 0; i < infos.size(); i++) {
		if (!infos[i]->is_alive()) {//发现列表中有死去的随从
			if (infos[i]->has_death() == MOUSE) {
				for (j = 0; j < infos[i]->read_attack(); j++) {
					nextstate.push(MOUSE);
					total++;
				}
			}
			//发现老鼠的亡语
			/*
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
			}*/
			delete (infos[i]);
			/*
			for (j = i; j < infos.size() - 1; j++) {
				infos[j] = infos[j + 1];
			}
			infos.resize(infos.size() - 1);
			*/
		}
		else {
			nextstate.push(i);//记录未死亡随从的原本位置，并保存副本
			temp[i] = infos[i];
			res++;
			total++;
		}
	}
	idle = 7 - res;
	i = 0;
	if (total > 7)
		infos.resize(7);
	else
		infos.resize(total);
	while (!nextstate.empty()) {
		state = nextstate.front();
		if (state < 7) {
			infos[i++] = temp[state];
		}
		else if (state == MOUSE) {
			if (idle > 0) {
				infos[i++] = new minion("小老鼠", 1, 1);
				idle--;
			}
			else {

			}
		}
		nextstate.pop();
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
		if (p->hit_nearby())
			cout << " 狂战";
		if (p->has_death())
			cout << " 亡语";
		if (p->is_poisonous())
			cout << " 剧毒";
		cout << endl;
	}
	cout << "展示完毕" << endl;
	cout << "-------------------------" << endl;
}


//返回即将进行攻击的随从序号
int attackindex(vector<minion*>& knownminion) {
	int i;
	int n = knownminion.size();
	if (knownminion[n - 1]->has_attacked()) {//最后一个随从也已经进行攻击，则重置所有随从的攻击
		for (i = 0; i < n; i++) {
			knownminion[i]->reset_attacked();
		}
		return 0;
	}
	else {
		for (i = n - 2; i > -1; i--) {//从末尾寻找已攻击随从，返回下一个随从的序号，找不到则从头开始攻击
			if (knownminion[i]->has_attacked())
				return i + 1;
		}
	}
	return 0;
}

int battle(vector<minion*>& here, vector<minion*>& there) {
	int i;
	vector<minion*> tauntminion;
	int attind;//进行攻击随从的序号
	int index;//受到攻击随从的序号
	int attackstate = 0;
	vector<minion*> hitter;
	vector<minion*> hittee;
	
	while (here.size() && there.size()) {
		if (++attackstate % 2) {
			hitter = here;
			hittee = there;
		}
		else {
			hitter = there;
			hittee = here;
		}
		tauntminion.clear();
		for (i = 0; i < hittee.size(); i++) {
			if (hittee[i]->has_taunt()) {
				tauntminion.push_back(hittee[i]);
			}
		}

		if (tauntminion.size() == 0)//敌方没有嘲讽
			index = rand() % (hittee.size());
		else {//找到嘲讽的随从
			i = rand() % (tauntminion.size());
			for (index = 0; index < hittee.size(); index++) {
				if (hittee[index] == tauntminion[i]) {
					break;
				}
			}
		}


		attind = attackindex(hitter);
		hitter[attind]->hit(hittee[index]);
		//狂战斧特效判定
		if (hitter[attind]->hit_nearby()) {
			if (index > 0)
				hittee[index - 1]->get_hit(hitter[attind]->read_attack());
			if (index < hittee.size() - 1)
				hittee[index + 1]->get_hit(hitter[attind]->read_attack());
		}

		hitter[attind]->set_attacked();
		clearMinion(here);
		clearMinion(there);
		//showMinion(here);
		//showMinion(there);

		
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
		minion* my1 = new minion("瘟疫鼠群5", 5, 5, 0, 1, 0, 0, 0, 0, 0);
		//minion* my2 = new minion("我2", 2, 1, 0, 0, 0, 0, 0, 0);
		minion* my3 = new minion("瘟疫鼠群4", 2, 4, 0, 0, 0, 0, MOUSE, 1, 0);
		//minion* my4 = new minion("我4", 3, 3);
		//minion* my5 = new minion("我5", 1, 1, 0, 1, 0, 0, 0, 0);
		//minion* my6 = new minion("我6", 6, 6);
		//minion* my7 = new minion("我7", 10, 10);
		//minion* you1 = new minion("你1", 1, 1);
		//minion* you2 = new minion("你2", 2, 2);
		//minion* you3 = new minion("你3", 3, 3);
		//minion* you4 = new minion("你4", 4, 4);
		//minion* you5 = new minion("你5", 2, 2);
		minion* you6 = new minion("你6", 5, 6, 0, 0, 1, 0, 0, 0, 0);
		minion* you7 = new minion("你7", 10, 7);
		vector<minion*> here;
		vector<minion*> there;
		//here = { my7, my6, my5, my4, my3, my2, my1 };
		//there = { you7, you6, you5, you4, you3, you2, you1 };
		here = { my3, my1 };
		there = { you6, you7 };
		//showMinion(here);
		//showMinion(there);
		if (i == 0) {
			cout << "你的随从:" << endl;
			showMinion(here);
			cout << "敌方随从:" << endl;
			showMinion(there);
		}
		else {
			//break;
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