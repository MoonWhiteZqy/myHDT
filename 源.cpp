#include "minions.h"
#include<iostream>
#include<random>
#include<ctime>
#include<queue>
#include<unordered_map>
#include<fstream>
#include<sstream>


using namespace std;



vector<unordered_map<string, vector<int>>> cards;

//删除已经死去的随从
int clearMinion(vector<minion*>& infos) {
	int res = 0;
	int i;
	int j = 0;//用来算reborn
	int idle;//确认亡语、复生的随从数量
	int state;
	int nana = 0;
	int total = 0;//亡语等触发后随从的总数，最大为7
	int guzhicount = 0;//钴制卫士的机械计数
	int xiaohuangshield = 0;//小黄的圣盾
	queue<int> nextstate;
	vector<minion*> temp(7);
	vector<string> reborns;
	vector<int> toshield;//获得小黄的圣盾的随从编号
	int deathtime = 1;//瑞文的效果
	int d;
	int erziindex;//子嗣亡语的指针

	for (i = 0; i < infos.size(); i++) {
		if (infos[i]->read_name() == "瑞文戴尔男爵" && infos[i]->is_alive())
			deathtime = 2;
	}

	for (i = 0; i < infos.size(); i++) {
		if (!infos[i]->is_alive()) {//发现列表中有死去的随从
			if (infos[i]->has_death()) {//随从具有亡语
				if (infos[i]->read_name() == "瘟疫鼠群") {
					for (j = 0; j < infos[i]->read_attack(); j++) {
						for (d = 0; d < deathtime; d++)	nextstate.push(MOUSE);
						total++;
					}
				}
				else if (infos[i]->read_name() == "寄生饿狼") {
					for(d = 0; d < deathtime * 2; d++) nextstate.push(SPIDER);
					total += 2;
				}
				else if (infos[i]->read_name() == "长鬃草原狮") {
					for (d = 0; d < deathtime * 2; d++) nextstate.push(LION);
					total += 2;
				}
				else if (infos[i]->has_death() > MECHSS) {//机械亡语，将宏设定为较大值，通过差值确认亡语数量
					for (j = 0; j < infos[i]->has_death() - MECHSS; j++) {
						for (d = 0; d < deathtime * 2; d++) nextstate.push(LMECH);
						total++;
					}
				}
				else if (infos[i]->read_name() == "红衣纳迪娜") {
					nana = 1;
				}
				else if (infos[i]->read_name() == "无私的英雄") {
					xiaohuangshield += deathtime;
				}
				else if (infos[i]->read_name() == "恩佐斯的子嗣") {
					for (d = 0; d < deathtime; d++) nextstate.push(ERZI);
				}
			}
			if (infos[i]->has_reborn()) {//发现死亡随从具有复生
				nextstate.push(REBORN);
				reborns.push_back(infos[i]->read_name());
				total++;
			}
			delete (infos[i]);
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
	j = 0;
	if (total > 7)
		infos.resize(7);
	else
		infos.resize(total);
	while (!nextstate.empty()) {
		state = nextstate.front();
		if (state < 7) {
			infos[i++] = temp[state];
			temp[state] = NULL;
		}
		else if (state == MOUSE) {
			if (idle > 0) {
				infos[i++] = new minion("小老鼠", 1, 1, BEAST);
				idle--;
			}
		}
		else if (state == SPIDER) {
			if (idle > 0) {
				infos[i++] = new minion("小蜘蛛", 1, 1, BEAST);
				idle--;
			}
		}
		else if (state == LION) {
			if (idle > 0) {
				infos[i++] = new minion("小狮子", 2, 2, BEAST);
				idle--;
			}
		}
		else if (state == LMECH) {
			if (idle > 0) {
				infos[i++] = new minion("小机器人", 1, 1, MECH);
				idle--;
				guzhicount++;
			}
		}
		else if (state == REBORN) {
			if (idle > 0) {
				int k;
				for (k = 0; k < 6; k++) {
					if (cards[k].count(reborns[j])) {
						break;
					}
				}
				infos[i] = new minion(reborns[j], cards[k][reborns[j]]);
				infos[i]->lose_reborn();
				infos[i]->gain_buff(0, 1 - infos[i]->read_attack(), 0, 0, 0);
				i++;
				j++;
			}
		}
		else if (state == ERZI) {
			for (erziindex = 0; erziindex < i; erziindex++) {//对已经进入最终随从区域的随从buff
				infos[erziindex]->gain_buff(1, 1, 0, 0, 0);
			}
			for (erziindex = 0; erziindex < 7; erziindex++) {//对处在temp数组的随从buff
				if (temp[erziindex] == NULL) {
					continue;
				}
				else
					temp[erziindex]->gain_buff(1, 1, 0, 0, 0);
			}
		}
		nextstate.pop();
	}
	if (guzhicount) {
		for (i = 0; i < infos.size(); i++) {
			if (infos[i]->read_name() == "偏折机器人") {
				infos[i]->gain_buff(guzhicount, 0, 1, 0, 0);
			}
		}
	}

	if (nana) {
		for (i = 0; i < infos.size(); i++) {
			if (infos[i]->read_racial() == DRAGON) {
				infos[i]->gain_buff(0, 0, 1, 0, 0);
			}
		}
	}
	for (i = 0; i < xiaohuangshield; i++) {
		toshield.resize(0);
		for (j = 0; j < infos.size(); j++) {
			if (!infos[j]->has_shield()) {
				toshield.push_back(j);
			}
		}
		if (toshield.size() == 0)
			break;
		int huang = rand() % toshield.size();
		infos[toshield[huang]]->gain_buff(0, 0, 1, 0, 0);
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
		if (p->has_reborn())
			cout << " 复生";
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
		if (name == "量产型恐吓机")
			info[6] = MECHSS + 3;
		card[name] = info;

	}
	return card;
}


//决定出场的随从
void choose_minion(vector<minion*>& lineup) {
	string name;
	int attack, health, shield, taunt, reborn;
	int i;
	while (cin >> name) {
		if (name == "结束")
			break;
		for (i = 0; i < 6; i++) {
			if (cards[i].count(name)) {
				lineup.push_back(new minion(name, cards[i][name]));
				cout << "这就是你要找的那一个，要来点buff吗" << endl;
				cout << "攻击、血量、圣盾、嘲讽、复生" << endl;
				cin >> attack >> health >> shield >> taunt >> reborn;
				lineup[lineup.size() - 1]->gain_buff(attack, health, shield, taunt, reborn);
				cout << "哦豁，你又前进了一大步" << endl;
				break;
			}
		}
		for (i = 0; i < lineup.size(); i++)
			lineup[i]->showcard();
		if (lineup.size() == 7) {
			cout << "我会暗中支持你的，可别告诉别人" << endl;
			break;
		}
		if (i == 6) {
			cout << "没关系人生难免不如意" << endl;
		}
	}
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

		cout << endl << "现在开始选择你的随从，输入他们的名字" << endl;
		choose_minion(myminion);

		cout << endl << "现在开始选择对手的随从，输入他们的名字" << endl;
		choose_minion(yourminion);
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