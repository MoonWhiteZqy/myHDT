#include "minions.h"
#include<iostream>
#include<random>
#include<ctime>

#define MOUSE 1

using namespace std;

//ɾ���Ѿ���ȥ�����
int clearMinion(vector<minion*>& infos) {
	int res = 0;
	int i;
	int j;

	for (i = 0; i < infos.size(); i++) {
		if (!infos[i]->is_alive()) {//�����б�������ȥ�����
			res++;
			//�������������
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
		if (p->has_death())
			cout << " ����";
		cout << endl;
	}
	cout << "չʾ���" << endl;
	cout << "-------------------------" << endl;
}

int battle(vector<minion*>& here, vector<minion*>& there) {
	int pm = 0;
	int pn = 0;
	int i;
	vector<minion*> mytaunt;
	vector<minion*> youtaunt;
	int index;
	
	while (here.size() && there.size()) {

		youtaunt.clear();
		for (i = 0; i < there.size(); i++) {
			if (there[i]->has_taunt()) {
				youtaunt.push_back(there[i]);
			}
		}

		if (youtaunt.size() == 0)//�з�û�г���
			index = rand() % (there.size());
		else {//�ҵ���������
			i = rand() % (youtaunt.size());
			for (index = 0; index < there.size(); index++) {
				if (there[index] == youtaunt[i]) {
					break;
				}
			}
		}

		here[pm % (here.size())]->hit(there[index]);

		if (here[pm % (here.size())]->is_alive())//��ǰ���û����������һ��ӽ����´ν���
			pm++;
		clearMinion(here);
		clearMinion(there);

		

		if (!(here.size() && there.size()))//����һ���Ѿ��ճ�
			break;

		mytaunt.clear();
		for (i = 0; i < here.size(); i++) {
			if (here[i]->has_taunt()) {
				mytaunt.push_back(here[i]);
			}
		}

		if (mytaunt.size() == 0)//�ҷ�û�г���
			index = rand() % (here.size());
		else {
			i = rand() % (mytaunt.size());
			for (index = 0; index < here.size(); index++) {//�ҵ��ҷ��������λ��
				if (here[index] == mytaunt[i]) {
					break;
				}
			}
		}

		there[pn % (there.size())]->hit(here[index]);

		if (there[pn % (there.size())]->is_alive())
			pn++;
		clearMinion(here);
		clearMinion(there);

		
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

int main() {
	int win = 0;
	int deal = 0;
	int lose = 0;
	int i;
	int end;
	int n = 1000;
	int state;
	srand((int)time(NULL));
	cout << "������ϲ����ָ�ӹٻ����ˣ�׼����һ����" << endl << endl;
	for (i = 0; i < n; i++) {
		minion* my1 = new minion("������Ⱥ5", 5, 5, 0, 0, 0, 0, MOUSE, 0);
		//minion* my2 = new minion("��2", 1, 1, 1, 1);
		minion* my3 = new minion("������Ⱥ4", 4, 4, 0, 0, 0, 0, MOUSE, 0);
		//minion* my4 = new minion("��4", 4, 4);
		minion* my5 = new minion("��5", 2, 2);
		//minion* my6 = new minion("��6", 6, 6);
		//minion* my7 = new minion("��7", 10, 10);
		//minion* you1 = new minion("��1", 1, 1);
		//minion* you2 = new minion("��2", 2, 2);
		//minion* you3 = new minion("��3", 3, 3);
		//minion* you4 = new minion("��4", 4, 4);
		//minion* you5 = new minion("��5", 2, 2);
		minion* you6 = new minion("��6", 2, 1);
		minion* you7 = new minion("��7", 10, 16);
		vector<minion*> here;
		vector<minion*> there;
		//here = { my7, my6, my5, my4, my3, my2, my1 };
		//there = { you7, you6, you5, you4, you3, you2, you1 };
		here = { my3, my1, my5 };
		there = { you7 };
		//showMinion(here);
		//showMinion(there);
		if (i == 0) {
			cout << "������:" << endl;
			showMinion(here);
			cout << "�з����:" << endl;
			showMinion(there);
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