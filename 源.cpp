#include "minions.h"
#include<iostream>
#include<random>
#include<ctime>
#include<queue>
#include<unordered_map>
#include<fstream>
#include<sstream>


using namespace std;

//ɾ���Ѿ���ȥ�����
int clearMinion(vector<minion*>& infos) {
	int res = 0;
	int i;
	int j;
	int idle;//ȷ������������������
	int state;
	int total = 0;//����ȴ�������ӵ����������Ϊ7
	queue<int> nextstate;
	vector<minion*> temp(7);

	for (i = 0; i < infos.size(); i++) {
		if (!infos[i]->is_alive()) {//�����б�������ȥ�����
			if (infos[i]->has_death() == MOUSE) {
				for (j = 0; j < infos[i]->read_attack(); j++) {
					nextstate.push(MOUSE);
					total++;
				}
			}
			//�������������
			/*
			if (infos[i]->has_death() == MOUSE) {
				minion* temp = infos[i];
				int rest = 8 - infos.size();
				int exist = infos.size();
				if (temp->read_attack() < rest) {//����������ж����λ
					infos.resize(infos.size() + temp->read_attack() - 1);
					for (j = 1; j <= temp->read_attack() && i + j < exist; j++) {
						infos[i + j + temp->read_attack() - 1] = infos[i + j];
					}
					for (j = 0; j < temp->read_attack(); j++) {
						infos[i + j] = new minion("С����", 1, 1);
					}
					i += temp->read_attack();
					delete temp;
					continue;
				}
				else {//����������
					infos.resize(7);
					for (j = i + 1; j < exist; j++) {
						infos[7 - exist + j] = infos[j];
					}
					for (j = i; j < 8 - exist + i; j++) {
						infos[j] = new minion("С����", 1, 1);
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
			nextstate.push(i);//��¼δ������ӵ�ԭ��λ�ã������渱��
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
				infos[i++] = new minion("С����", 1, 1);
				idle--;
			}
			else {

			}
		}
		nextstate.pop();
	}

	return res;
}

//�����ǰvector�������
void showMinion(vector<minion*>& infos) {
	int health;
	int attack;
	string name;
	//cout << "-------------------------" << endl;
	//cout << "��ǰ���Ϊ:" << endl;
	for (auto p : infos) {
		health = p->read_health();
		attack = p->read_attack();
		name = p->read_name();
		cout << name << ' ' << attack << '/' << health;
		if (p->has_shield())
			cout << " ʥ��";
		if (p->has_taunt())
			cout << " ����";
		if (p->hit_nearby())
			cout << " ��ս";
		if (p->has_death())
			cout << " ����";
		if (p->is_poisonous())
			cout << " �綾";
		cout << endl;
	}
	cout << "չʾ���" << endl;
	cout << "-------------------------" << endl;
}


//���ؼ������й�����������
int attackindex(vector<minion*>& knownminion) {
	int i;
	int n = knownminion.size();
	if (knownminion[n - 1]->has_attacked()) {//���һ�����Ҳ�Ѿ����й�����������������ӵĹ���
		for (i = 0; i < n; i++) {
			knownminion[i]->reset_attacked();
		}
		return 0;
	}
	else {
		for (i = n - 2; i > -1; i--) {//��ĩβѰ���ѹ�����ӣ�������һ����ӵ���ţ��Ҳ������ͷ��ʼ����
			if (knownminion[i]->has_attacked())
				return i + 1;
		}
	}
	return 0;
}

int battle(vector<minion*>& here, vector<minion*>& there) {
	int i;
	vector<minion*> tauntminion;
	int attind;//���й�����ӵ����
	int index;//�ܵ�������ӵ����
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

		if (tauntminion.size() == 0)//�з�û�г���
			index = rand() % (hittee.size());
		else {//�ҵ���������
			i = rand() % (tauntminion.size());
			for (index = 0; index < hittee.size(); index++) {
				if (hittee[index] == tauntminion[i]) {
					break;
				}
			}
		}


		attind = attackindex(hitter);
		hitter[attind]->hit(hittee[index]);
		//��ս����Ч�ж�
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
	if (here.size() > 0)//ʤ��
		return 1;
	else if (there.size() > 0)//ʧ��
		return 0;
	else {
		return -2;//ƽ��
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
	string fileName = "������Ϣ0";
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
		//ʥ��2�� ����3�� ��ս4�� ����5�� ����6�� �綾7�� ����8�� ��Ч9
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
				if (word == "ʥ��") {
					info[2] = 1;
				}
				else if (word == "����")
					info[3] = 1;
				else if (word == "��ս")
					info[4] = 1;
				else if (word == "����")
					info[5] = 1;
				else if (word == "����")
					info[6] = 1;
				else if (word == "�綾")
					info[7] = 1;
				else if (word == "����")
					info[8] = MURLOC;
				else if (word == "Ұ��")
					info[8] = BEAST;
				else if (word == "��")
					info[8] = DRAGON;
				else if (word == "��е")
					info[8] = MECH;
				else if (word == "ȫ��")
					info[8] = ALL;
				else if (word == "��ħ")
					info[8] = DEMON;
				else if (word == "����")
					info[8] = PIRATE;
				else if (word == "��Ч")
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
	cout << "������ϲ����ָ�ӹٻ����ˣ�׼����һ����" << endl << endl << "����1�鿴��ӣ� ����2ѡ�����" << endl;

	cin >> command;

	if (command == "1") {
		cout << "����ƹݵĵȼ�" << endl;
		while (cin >> i) {
			for (auto p : cards[i - 1]) {
				minion* single = new minion(p.first, p.second);
				single->showcard();
				delete single;
			}
			cout << endl << "��������ô�е���Щ��ӣ��ĸ��֣�©������" << endl;
		}
	}
	else {
		cout << "���������ӵ�����" << endl;
		while (cin >> name) {
			if (name == "����")
				break;
			for (i = 0; i < 6; i++) {
				if (cards[i].count(name)) {
					myminion.push_back(new minion(name, cards[i][name]));
					cout << "�������Ҫ�ҵ���һ��" << endl;
					break;
				}
			}
			if (i == 6) {
				cout << "û��ϵ�������ⲻ����" << endl;
			}
		}
		cout << endl << "������ֵ���ӵ�����" << endl;
		while (cin >> name) {
			if (name == "����")
				break;
			for (i = 0; i < 6; i++) {
				if (cards[i].count(name)) {
					yourminion.push_back(new minion(name, cards[i][name]));
					cout << "�������Ҫ�ҵ���һ��" << endl;
					break;
				}
			}
			if (i == 6) {
				cout << "û��ϵ�������ⲻ����" << endl;
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
			cout << "������:" << endl;
			showMinion(here);
			cout << "�з����:" << endl;
			showMinion(there);
		}
		else {
			//break;
		}
		if (here.size() == there.size()) {//�������һ���࣬�Ⱥ��ֶ�����
			state = rand() % 2;//1Ϊhere���֣�0Ϊ����
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
	cout << "ʤ��" << win << "��,ƽ��" << deal << "��,ʧ��" << lose << "��" << endl << endl;
	if (win > lose) {
		cout << "������Ǹ��˲Ű�����" << endl;
	}
	else {
		cout << "�Ҹ�����ɫ��һЩ����" << endl;
	}
	system("pause");
	return 0;
}