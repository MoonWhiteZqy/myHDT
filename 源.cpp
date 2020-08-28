#include "minions.h"
#include<iostream>
#include<random>
#include<ctime>
#include<queue>


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
		minion* my1 = new minion("������Ⱥ5", 5, 5, 0, 1, 0, 0, 0, 0, 0);
		//minion* my2 = new minion("��2", 2, 1, 0, 0, 0, 0, 0, 0);
		minion* my3 = new minion("������Ⱥ4", 2, 4, 0, 0, 0, 0, MOUSE, 1, 0);
		//minion* my4 = new minion("��4", 3, 3);
		//minion* my5 = new minion("��5", 1, 1, 0, 1, 0, 0, 0, 0);
		//minion* my6 = new minion("��6", 6, 6);
		//minion* my7 = new minion("��7", 10, 10);
		//minion* you1 = new minion("��1", 1, 1);
		//minion* you2 = new minion("��2", 2, 2);
		//minion* you3 = new minion("��3", 3, 3);
		//minion* you4 = new minion("��4", 4, 4);
		//minion* you5 = new minion("��5", 2, 2);
		minion* you6 = new minion("��6", 5, 6, 0, 0, 1, 0, 0, 0, 0);
		minion* you7 = new minion("��7", 10, 7);
		vector<minion*> here;
		vector<minion*> there;
		//here = { my7, my6, my5, my4, my3, my2, my1 };
		//there = { you7, you6, you5, you4, you3, you2, you1 };
		here = { my3, my1 };
		there = { you6, you7 };
		//showMinion(here);
		//showMinion(there);
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