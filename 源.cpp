#include "minions.h"
#include<iostream>
#include<random>
#include<ctime>
#include<queue>
#include<unordered_map>
#include<fstream>
#include<sstream>


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


unordered_map<string, vector<int>> card_info(int no) {
	string fileName = "卡牌信息0";
	fileName += '0' + no;
	fileName += ".txt";
	ifstream readFile(fileName);
	unordered_map<string, vector<int>> card;
	string s;
	string name;
	int i;
	string word;
	while (getline(readFile, s)) {
		vector<int> info(10, 0);
		//圣盾2、 嘲讽3、 狂战4、 复生5、 亡语6、 剧毒7、 种族8、 特效9
		istringstream words(s);
		i = 0;
		while (words >> word) {
			if (i == 0) {
				name = word;
			}
			else if (i < 3) {
				info[i - 1] = stoi(word);
			}
			else {
				if (word == "圣盾") {
					info[2] = 1;
				}
				else if (word == "嘲讽")
					info[3] = 1;
				else if (word == "狂战")
					info[4] = 1;
				else if (word == "复生")
					info[5] = 1;
				else if (word == "亡语")
					info[6] = 1;
				else if (word == "剧毒")
					info[7] = 1;
				else if (word == "鱼人")
					info[8] = MURLOC;
				else if (word == "野兽")
					info[8] = BEAST;
				else if (word == "龙")
					info[8] = DRAGON;
				else if (word == "机械")
					info[8] = MECH;
				else if (word == "全部")
					info[8] = ALL;
				else if (word == "恶魔")
					info[8] = DEMON;
				else if (word == "海盗")
					info[8] = PIRATE;
				else if (word == "特效")
					info[9] = 1;
			}
			i++;
		}
		card[name] = info;

	}
	return card;
}


int main() {
	int win = 0;
	int deal = 0;
	int lose = 0;
	int i;
	int end;
	int n = 1000;
	int state;
	string command;
	string name;
	vector<unordered_map<string, vector<int>>> cards;
	vector<minion*> myminion;
	vector<minion*> yourminion;
	for (i = 1; i < 7; i++) {
		cards.push_back(card_info(i));
	}
	srand((int)time(NULL));
	cout << "嘿我最喜欢的指挥官回来了，准备来一局吗" << endl << endl << "输入1查看随从， 输入2选择随从" << endl;

	cin >> command;

	if (command == "1") {
		cout << "输入酒馆的等级" << endl;
		while (cin >> i) {
			for (auto p : cards[i - 1]) {
				minion* single = new minion(p.first, p.second);
				single->showcard();
				delete single;
			}
			cout << endl << "你问我怎么招的这些随从，四个字，漏斗蛋糕" << endl;
		}
	}
	else {
		cout << "输入你的随从的名字" << endl;
		while (cin >> name) {
			if (name == "结束")
				break;
			for (i = 0; i < 6; i++) {
				if (cards[i].count(name)) {
					myminion.push_back(new minion(name, cards[i][name]));
					cout << "这就是你要找的那一个" << endl;
					break;
				}
			}
			if (i == 6) {
				cout << "没关系人生难免不如意" << endl;
			}
		}
		cout << endl << "输入对手的随从的名字" << endl;
		while (cin >> name) {
			if (name == "结束")
				break;
			for (i = 0; i < 6; i++) {
				if (cards[i].count(name)) {
					yourminion.push_back(new minion(name, cards[i][name]));
					cout << "这就是你要找的那一个" << endl;
					break;
				}
			}
			if (i == 6) {
				cout << "没关系人生难免不如意" << endl;
			}
		}
		cout << endl;
	}


	for (i = 0; i < n; i++) {
		vector<minion*> here;
		vector<minion*> there;

		here.resize(myminion.size());
		for (int m = 0; m < here.size(); m++) {
			here[m] = new minion(myminion[m]);
		}
		there.resize(yourminion.size());
		for (int n = 0; n < there.size(); n++) {
			there[n] = new minion(yourminion[n]);
		}

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