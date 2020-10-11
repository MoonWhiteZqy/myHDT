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
unordered_map<string, vector<int>> GuimamaDemon; //����������ɶ�ħ
vector<string> GuimamaName; //���������ɶ�ħ������
unordered_map<string, vector<int>> BoatPirate; //�����������ɺ���
vector<string> BoatName; //�ɹź���������
int enemyNeigui = 0; //Ϊ�з������ڹ������




//�����ǰvector�������
void showMinion(vector<minion*>& infos) {
	for (int i = 0; i < infos.size(); i++){
		infos[i]->showcard();
	}
	cout << "չʾ���" << endl;
	cout << "-------------------------" << endl;
}

//ʵ��������ʿ��Ч
void guzhibuff(vector<minion*>& infos, vector<minion*>& temp){
	int i;
	for(i = 0; i < infos.size(); i++){
		if(infos[i] == NULL){
			continue;
		}
		else if(infos[i]->read_name() == "ƫ�ۻ�����"){
			infos[i]->gain_buff(1, 0, 1, 0, 0);
		}
	}
	for(i = 0; i < temp.size(); i++){
		if(temp[i] == NULL){
			continue;
		}
		else if(temp[i]->read_name() == "ƫ�ۻ�����"){
			temp[i]->gain_buff(1, 0, 1, 0, 0);
		}
	}
}

//ʵ��ʳ��������Ч
void tulangbuff(vector<minion*>& infos, vector<minion*>& temp) {
	int i;
	for (i = 0; i < infos.size(); i++) {
		if(infos[i] == NULL) {
			continue;
		}
		else if (infos[i]->read_name() == "ʳ������") {
			infos[i]->gain_buff(2, 1, 0, 0, 0);
		}
	}
	for (i = 0; i < temp.size(); i++) {
		if (temp[i] == NULL) {
			continue;
		}
		else if (temp[i]->read_name() == "ʳ������") {
			temp[i]->gain_buff(2, 1, 0, 0, 0);
		}
	}
}

//ʵ����ͭ������������Ч
void lajibuff(vector<minion*>& infos, vector<minion*>& temp) {
	int i;
	for (i = 0; i < infos.size(); i++) {
		if(infos[i] == NULL) {
			continue;
		}
		else if (infos[i]->read_name() == "���ջ�����") {
			infos[i]->gain_buff(2, 2, 0, 0, 0);
		}
	}
	for (i = 0; i < temp.size(); i++) {
		if (temp[i] == NULL) {
			continue;
		}
		else if (temp[i]->read_name() == "���ջ�����") {
			temp[i]->gain_buff(2, 2, 0, 0, 0);
		}
	}
}

//ʵ����Ϲ����Ч
void laoxiayanbuff(vector<minion*>& myminion, vector<minion*>& yourminion, int gain){ //gainΪ1ʱ����Ϲ�ۼӹ���gainΪ-1ʱ����Ϲ�ۼ��ٹ���
	int i;
	int count = -1; //��Ϲ���Լ���ͳ������
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_racial() == MURLOC || myminion[i]->read_racial() == ALL) {
			count++;
		}
	}
	for (i = 0; i < yourminion.size(); i++) {
		if (yourminion[i]->read_racial() == MURLOC || yourminion[i]->read_racial() == ALL) {
			count++;
		}
	}
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_name() == "��Ϲ��") {
			myminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
		}
	}
	for (i = 0; i < yourminion.size(); i++) {
		if (yourminion[i]->read_name() == "��Ϲ��") {
			yourminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
		}
	}
}

//ʵ�����������Ч
void lingjunbuff(vector<minion*>& myminion, int gain){ //gainΪ1ʱ����ȫ�����˼ӹ���gainΪ-1ʱ�����ٹ���
	int i;
	int count = 0;
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_name() == "�������") { // ͳ�Ƴ������������ÿ��һ����2��
			count += 2;
		}
	}
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_racial() == MURLOC || myminion[i]->read_racial() == ALL) {
			myminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
			if (myminion[i]->read_name() == "�������") {//����Լ������Լ�buff
				myminion[i] ->gain_buff((-2) * gain, 0, 0, 0, 0);
			}
		}
	}
}

//ʵ�ֹ��Ƕ�ħ��Ч
void gongchengbuff(vector<minion*>& myminion, int gain) {
	int i;
	int count = 0;
	for (i = 0; i < myminion.size(); i++) {
		if (myminion[i]->read_name() == "���Ƕ�ħ") {
			count++;
		}
	}
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_racial() == DEMON || myminion[i]->read_racial() == ALL) {
			myminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
			if (myminion[i]->read_name() == "���Ƕ�ħ") {
				myminion[i]->gain_buff((-1) * gain, 0, 0, 0, 0);
			}
		}
	}
}

//��ȡ���Ϲ⻷
void readAura(vector<minion*>& infos, int& pirateAura, int& demonAura) { 
	int i;
	pirateAura = 0;
	demonAura = 0;
	for (i = 0; i < infos.size(); i++) { //��ȡ�������й⻷����
		if (infos[i]->read_name() == "�������˹") {
			demonAura++;
		}
		else if (infos[i]->read_name() == "�Ϻ�����") {
			pirateAura++;
		}
	}
}

//��ʼ��������ӵĹ⻷����
void setAura(vector<minion*>& infos, int& pirateAura, int& demonAura) { 
	int i;
	for (i = 0; i < infos.size(); i++) {
		if (infos[i]->read_racial() == PIRATE) { //��ʼ���Ϻ���������
			infos[i]->initial_pirate_aura(pirateAura);
		}
		else if (infos[i]->read_racial() == DEMON) { //��ʼ�������⻷����
			infos[i]->initial_demon_aura(demonAura);
		}
		else if (infos[i]->read_racial() == ALL) { //�ںϹֵ�������
			infos[i]->initial_demon_aura(demonAura);
			infos[i]->initial_pirate_aura(pirateAura);
		}
	}
}

//ˢ�³�����ӹ⻷Ч��
void refreshAura(vector<minion*>& infos, int& pirateAura, int& demonAura) {
	int racial = 0;
	for (int i = 0; i < infos.size(); i++) {
		racial = infos[i]->read_racial();
		if (racial <= DEMON || racial == ALL) {
			infos[i]->racial_buff(pirateAura, demonAura);
		}
	}
}

//�ܵ����Եж���ӵķ���������(����������)
void untouchHit(vector<minion*>& enemys, int injury) {
	int i, x;
	int j;
	vector<minion*> availMinion(0);
	for (i = 0; i < enemys.size(); i++) { //����������Ѫ������Ӽ���������
		if (enemys[i]->read_health() > 0) {
			availMinion.push_back(enemys[i]);
		}
	}
	x = availMinion.size();
	if (!x) { //������Ӷ����������޷����ܵ��˺�
		return ;
	}
	else {
		i = rand() % x; //���ѡȡһ�����������
		if (availMinion[i]->has_shield()) { //�ܵ��˺���Ӿ���ʥ�ܣ���ʥ�������ӻ��buff
			for (j = 0; j < enemys.size(); j++) {
				if (enemys[j]->read_name() == "����ִ����") {
					enemys[j]->gain_buff(2, 2, 0, 0, 0);
				}
				else if (enemys[j]->read_name() == "ԡ���߲��߶�") {
					enemys[j]->gain_buff(2, 0, 0, 0, 0);
				}
			}
		}
		availMinion[i]->get_hit(injury);

	}
}

// ��Ӹո��ܵ��������������
void just_hit_minion(vector<minion*>& infos) {
	int i, j;
	int pirateAura = 0;
	int demonAura = 0;
	int jh;
	string demonName;
	vector<minion*> temp;
	int d;
	for (i = 0; i < infos.size() && infos.size() < 7; i++) { //����ʱ���������˴���
		for (jh = 0; jh < infos[i]->just_got_hit(); jh++) {
			if (infos.size() >= 7) { //��ֹ����ԭ��������Ӱ��
				break;
			}
			if (infos[i]->read_name() == "С������") {
				infos.resize(infos.size() + 1); // ����һ��
				for (j = infos.size() - 1; j > i + 1; j--){
					infos[j] = infos[j - 1];
				}
				d = rand() % GuimamaName.size(); //��������ٻ�һ����ħ�����в������Լ�
				demonName = GuimamaName[d];
				infos[i + 1] = new minion(demonName, GuimamaDemon[demonName]);
				readAura(infos, pirateAura, demonAura); //������ֶ�������Ҫ��Ѫ�����������ж�
				refreshAura(infos, pirateAura, demonAura);
			}
			else if(infos[i]->read_name() == "С������") { //�������ٻ�С��
				infos.resize(infos.size() + 1); // ����һ��
				for (j = infos.size() - 1; j > i + 1; j--){
					infos[j] = infos[j - 1];
				}
				infos[i + 1] = new minion("С��", 1, 1, DEMON);
				readAura(infos, pirateAura, demonAura); //������ֶ�������Ҫ��Ѫ�����������ж�
				refreshAura(infos, pirateAura, demonAura);
			}
			else if(infos[i]->read_name() == "����Ѳ����") {
				infos.resize(infos.size() + 1); // ����һ��
				for (j = infos.size() - 1; j > i + 1; j--){
					infos[j] = infos[j - 1];
				}
				infos[i + 1] = new minion("����������", 2, 3, MECH);
				infos[i + 1]->gain_buff(0, 0, 0, 1, 0); // �ٻ����ó���
				guzhibuff(infos, temp);
			}
		}
	}

	for (i = 0; i < infos.size(); i++) { //�����ܵ�����
		infos[i]->reset_just();
	}
}


//ɾ���Ѿ���ȥ�����
int clearMinion(vector<minion*>& infos, vector<minion*>& enemys) {
	int res = 0;
	int i;
	int j = 0;//������reborn
	int idle;//ȷ������������������
	int state;
	int nana = 0;
	int total = 0;//����ȴ�������ӵ����������Ϊ7
	int xiaohuangshield = 0;//С�Ƶ�ʥ��
	queue<int> nextstate;
	vector<minion*> temp(7);
	vector<string> reborns;
	vector<minion*> neiguiSet(0);
	vector<int> toshield;//���С�Ƶ�ʥ�ܵ���ӱ��
	vector<int> pucongatt(0);
	int deathtime = 1;//���ĵ�Ч��
	int d;
	int jh = 0; //�ո��ܵ��˺�����
	int buffindex;//����buff��i����
	string demonName; //�����ٻ���ħ������
	int pirateAura = 0; //���ڹ�����ײ�պ�����ʱ�������Ȼ�
	int demonAura = 0;
	int beastAttack = 0; //�ٻ�Ұ�޻�õĹ���buff
	int beastHealth = 0; //�ٻ�Ұ�޻�õ�����buff
	int newbeast = 0; //�ж��Ƿ�Ϊ���ٻ�Ұ��
	int penziCount = 0; //���ӵ�����
	int die = 0; //��¼�Ƿ�������������ж��Ƿ�ѭ��
	int neiguiCount = 0; //�ڹ������

	neiguiCount = enemyNeigui; //�ȼ�¼�ڹ������
	enemyNeigui = 0;

	for (i = 0; i < infos.size(); i++) {
		if (infos[i]->read_health() > 0) {
			if (infos[i]->read_name() == "���Ĵ����о�") {
				deathtime = 2;
			}
			else if (infos[i]->read_name() == "�����ˣ��") {
				penziCount++;
			}
		}
	}


	just_hit_minion(infos);


	for (i = 0; i < infos.size(); i++) {
		if (infos[i]->read_health() > 0) {
			if (infos[i]->read_name() == "��Ⱥ����") { //С���2��
				beastAttack += 2;
			}
			else if (infos[i]->read_name() == "������") { //����+4+4
				beastAttack += 4;
				beastHealth += 4;
			}
		}
	}

	for (i = 0; i < infos.size(); i++) {
		if (!(infos[i]->read_health() > 0)) {//�����б�������ȥ�����
			die = 1;
			if (infos[i]->has_death()) {//��Ӿ�������
				if (infos[i]->read_name() == "������Ⱥ") {
					for (j = 0; j < infos[i]->read_attack(); j++) {
						for (d = 0; d < deathtime; d++)	nextstate.push(MOUSE);
						total += deathtime;
					}
				}
				else if (infos[i]->read_name() == "��������") {
					for(d = 0; d < deathtime * 2; d++) nextstate.push(SPIDER);
					total += 2 * deathtime;
				}
				else if (infos[i]->read_name() == "���ײ�ԭʨ") {
					for (d = 0; d < deathtime * 2; d++) nextstate.push(LION);
					total += 2 * deathtime;
				}
				else if (infos[i]->read_name() == "���������"){
					for (d = 0; d < deathtime; d++) nextstate.push(WAIPO);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "С����ͽ") {
					for (d = 0; d < deathtime; d++)nextstate.push(LDEMON);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "�������") {
					for (d = 0; d < deathtime * 3; d++) nextstate.push(LANPANG);
					total += deathtime * 3;
				}
				else if (infos[i]->read_name() == "�������") {
					for (d = 0; d < deathtime; d++) nextstate.push(MAITIAN);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "�ݸ�������") {
					for (d = 0; d < deathtime; d++) nextstate.push(MURKING);
				}
				else if (infos[i]->read_name() == "���Ǹ����") {
					for (d = 0; d < deathtime; d++) nextstate.push(LANGDIE);
				}
				else if (infos[i]->has_death() > VEGE) { //ֲ������ĺ��趨ֵ�ϴ�����Ƚ����ж�
					for (j = 0; j < infos[i]->has_death() - VEGE; j++) {
						for(d = 0; d < deathtime; d++) nextstate.push(VEGE);
						total += deathtime;
					}
				}
				else if (infos[i]->has_death() > MECHSS){//��е��������趨Ϊ�ϴ�ֵ��ͨ����ֵȷ����������
					for (j = 0; j < infos[i]->has_death() - MECHSS; j++) {
						for (d = 0; d < deathtime; d++) nextstate.push(LMECH);
						total += deathtime;
					}
				}
				else if (infos[i]->read_name() == "��е��") {
					for (d = 0; d < deathtime; d++) nextstate.push(EGG);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "�����굶��") {
					for (d = 0; d < deathtime * 3; d++) nextstate.push(MENGGU);
					total += deathtime * 3;
				}
				else if (infos[i]->read_name() == "��˹����") {
					enemyNeigui += deathtime; //���з�������ڹ�
				}
				else if (infos[i]->read_name() == "�����ɵ���") {
					nana = 1;
				}
				else if (infos[i]->read_name() == "����������") {
					for (d = 0; d < deathtime; d++) nextstate.push(BOMB);
				}
				else if (infos[i]->read_name() == "��˽��Ӣ��") {
					xiaohuangshield += deathtime;
				}
				else if (infos[i]->read_name() == "����˹������") {
					for (d = 0; d < deathtime; d++) nextstate.push(ERZI);
				}
				else if (infos[i]->read_name() == "аħ�ʹ�"){
					for (d = 0; d < deathtime; d++) {
						pucongatt.push_back(infos[i]->read_attack());
					}
				}
			}
			if (infos[i]->has_reborn()) {//����������Ӿ��и���
				nextstate.push(REBORN);
				reborns.push_back(infos[i]->read_name());
				total++;
			}
			if (infos[i]->read_racial() == BEAST) { //���������Ұ��
				nextstate.push(DIEBST);
			}
			else if (infos[i]->read_racial() == MECH) { //��������ǻ�е
				nextstate.push(DIEMECH);
			}
			else if (infos[i]->read_racial() == DEMON) { //��������Ƕ�ħ
				for (d = 0; d < penziCount; d++) nextstate.push(PENZI);
			}
			else if (infos[i]->read_racial() == ALL) { //����������ںϹ�����
				nextstate.push(DIEBST);
				nextstate.push(DIEMECH);
				for (d = 0; d < penziCount; d++) nextstate.push(PENZI);
			}
			delete (infos[i]);
		}
		else {
			nextstate.push(i);//��¼δ������ӵ�ԭ��λ�ã������渱��
			temp[i] = infos[i];
			res++;
			total++;
		}
	}


	idle = 7 - res - neiguiCount;
	neiguiSet.resize(neiguiCount);
	for (i = 0; i < neiguiCount; i++) {
		neiguiSet[i] = new minion("�ڹ�", 3, 3);
	}
	i = 0;
	j = 0;
	infos.clear();
	if (total > 7)
		infos.resize(7);
	else
		infos.resize(total);
	while (!nextstate.empty()) {
		state = nextstate.front();
		newbeast = 0;
		if (state < 7) {//ԭ����Ӳ�δ������ ��temp���Ż�
			infos[i++] = temp[state];
			temp[state] = NULL;
		}//�����������ӷ���
		else if (state == MOUSE) {
			if (idle > 0) {
				infos[i++] = new minion("С����", 1, 1, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == SPIDER) {
			if (idle > 0) {
				infos[i++] = new minion("С֩��", 1, 1, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == LION) {
			if (idle > 0) {
				infos[i++] = new minion("Сʨ��", 2, 2, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == WAIPO) {
			if (idle > 0) {
				infos[i++] = new minion("������", 3, 2, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == LDEMON) {
			if (idle > 0) {
				infos[i++] = new minion("С��", 1, 1, DEMON);
				idle--;
			}
		}
		else if (state == LANPANG) {
			if (idle > 0) {
				infos[i++] = new minion("�������", 1, 3, 0, 1, 0, 0, 0, 0, DEMON);
				idle--;
			}
		}
		else if (state == MENGGU) {
			if(idle > 0) {
				int haijunnum = rand() % BoatName.size();
				string pirateName = BoatName[haijunnum];
				infos[i++] = new minion(pirateName, BoatPirate[pirateName]);
				idle--;
			}
		}
		else if (state == MAITIAN) {
			if (idle > 0) {
				infos[i++] = new minion("������", 2, 1, MECH);
				idle--;
				guzhibuff(infos, temp);
			}
		}
		else if (state == LMECH) {
			if (idle > 0) {
				infos[i++] = new minion("С������", 1, 1, MECH);
				idle--;
				guzhibuff(infos, temp);
			}
		}
		else if (state == VEGE) {
			if(idle > 0) {
				infos[i++] = new minion("Сֲ��", 1, 1);
				idle--;
			}
		}
		else if (state == EGG) {
			if (idle > 0) {
				infos[i++] = new minion("��е����", 8, 8, MECH);
				idle--;
				guzhibuff(infos, temp);
			}
		}
		else if (state == REBORN) {//���븴�����
			if (idle > 0) {
				int k;
				for (k = 0; k < 6; k++) {
					if (cards[k].count(reborns[j])) {
						break;
					}
				}
				if(cards[k][reborns[j]][8] == MECH || cards[k][reborns[j]][8] == ALL){
					guzhibuff(infos, temp);
				}
				infos[i] = new minion(reborns[j], cards[k][reborns[j]]);
				infos[i]->lose_reborn();
				infos[i]->gain_buff(0, 1 - infos[i]->read_health(), 0, 0, 0);
				if (infos[i]->read_racial() == BEAST || infos[i]->read_racial() == ALL) { //������Ұ��
					infos[i]->gain_buff(beastAttack, beastHealth, 0, 0, 0);
				}
				i++;
				j++;
			}
		}
		else if (state == ERZI) { //��������buffȫ��
			for (buffindex = 0; buffindex < i; buffindex++) {//���Ѿ��������������������buff
				infos[buffindex]->gain_buff(1, 1, 0, 0, 0);
			}
			for (buffindex = 0; buffindex < 7; buffindex++) {//�Դ���temp��������buff
				if (temp[buffindex] == NULL) {
					continue;
				}
				else
					temp[buffindex]->gain_buff(1, 1, 0, 0, 0);
			}
			for (buffindex = 0; buffindex < neiguiCount; buffindex++) { //���ڹ��ǽ���buff
				neiguiSet[buffindex]->gain_buff(1, 1, 0, 0, 0);
			}
		}
		else if (state == MURKING) { //��������buff����
			for (buffindex = 0; buffindex < i; buffindex++) {
				if (infos[buffindex]->read_racial() == MURLOC || infos[buffindex]->read_racial() == ALL) {
					infos[buffindex]->gain_buff(2, 2, 0, 0, 0);
				}
			}
			for (buffindex = 0; buffindex < 7; buffindex++) {
				if(temp[buffindex] == NULL) {
					continue;
				}
				else {
					if(temp[buffindex]->read_racial() == MURLOC || temp[buffindex]->read_racial() == ALL) {
						temp[buffindex]->gain_buff(2, 2, 0, 0, 0);
					}
				}
			}
		}
		else if (state == LANGDIE) { //�ǵ�����buffҰ�ޣ� ͬ��
			for (buffindex = 0; buffindex < i; buffindex++) {
				if (infos[buffindex]->read_racial() == BEAST || infos[buffindex]->read_racial() == ALL) {
					infos[buffindex]->gain_buff(5, 5, 0, 0, 0);
				}
			}
			for (buffindex = 0; buffindex < 7; buffindex++) {
				if(temp[buffindex] == NULL) {
					continue;
				}
				else {
					if(temp[buffindex]->read_racial() == BEAST || temp[buffindex]->read_racial() == ALL) {
						temp[buffindex]->gain_buff(5, 5, 0, 0, 0);
					}
				}
			}
		}
		else if (state == DIEBST) { //Ұ��������������buff
			tulangbuff(infos, temp);
		}
		else if (state == DIEMECH) { //��е�������Ի��ջ�����buff
			lajibuff(infos, temp);
		}
		else if (state == BOMB) {
			untouchHit(enemys, 4);
		}
		else if (state == PENZI) {
			untouchHit(enemys, 3);
		}
		
		if (newbeast) {
			infos[i - 1]->gain_buff(beastAttack, beastHealth, 0, 0, 0);
		}

		nextstate.pop();
	}

	if (infos.size() < 7 && neiguiCount) { //���ڹ���������
		j = infos.size(); //��ȡ��ǰ�������
		if (j + neiguiCount > 7) { //�����ڹ����Ӵﵽ����
			infos.resize(7);
		}
		else {
			infos.resize(j + neiguiCount); //�����ڹ�δ�ﵽ����
		}
		for (i = 0; i < neiguiCount; i++) {
			infos[j + i] = neiguiSet[i];
		}
	}
	

	if (nana) {
		for (i = 0; i < infos.size(); i++) {
			if (infos[i]->read_racial() == DRAGON || infos[i]->read_racial() == ALL) {
				infos[i]->gain_buff(0, 0, 1, 0, 0);
			}
		}
	}
	for (i = 0; i < xiaohuangshield; i++) { //С�Ƹ���ʥ�ܹ���
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

	if (infos.size()){ //�����ϻ�ʣ�����ʱ�Ž��д���
		for (i = 0; i < pucongatt.size(); i++) { //аħ�ʹӴ���
			j = rand() % infos.size();
			infos[j]->gain_buff(pucongatt[i], 0, 0, 0, 0);
		}
	}


	return die || enemyNeigui;
}




//���ؼ������й�����������
int attackindex(vector<minion*>& knownminion) {
	int i;
	int n = knownminion.size();
	if (n == 0)
		return 0;
	if (knownminion[n - 1]->has_attacked()) {//���һ�����Ҳ�Ѿ����й�����������������ӵĹ���
		for (i = 0; i < n; i++) {
			knownminion[i]->reset_attacked();
		}
		return 0;
	}
	else {
		for (i = n - 2; i > -1; i--) {//��ĩβѰ���ѹ�����ӣ�������һ����ӵ���ţ��Ҳ������ͷ��ʼ����
			if (knownminion[i]->has_attacked())
				break;
		}

		i++;

		while (i < n) {//�ӵ�һ��δ�����������λ�ÿ�ʼѰ��
			if (knownminion[i]->read_attack() > 0) {//�����й���������ӣ��������
				return i;
			}
			else {//û�й���������ӣ��趨Ϊ�ѹ���������
				knownminion[i]->set_attacked();
				i++;
			}
		}
		if (i == n)//��ǰ�Ѿ�����ӿɹ���
			return -1;
	}
	return 0;
}

// ��ɱ����ɱЧ��
void fire_overkill(minion* suffer, vector<minion*>& hittee, int& yourshield, int damage) {
	int i;
	minion* nowsuffer;
	just_hit_minion(hittee);
	if (suffer->read_health() < 0) { //�ﵽ��ɱ������
		for (i = 0; i < hittee.size(); i++) {
			if (hittee[i] == suffer) { //���ٶԵ�ǰ�ܹ���������Ӱ��
				continue;
			}
			else {
				if (hittee[i]->has_shield()) { //��ɱЧ������ʥ�����
					yourshield++;
					hittee[i]->lose_shield();
					return ;
				}
				else {
					nowsuffer = hittee[i];
					if (nowsuffer->read_health() < 0) { //��ֹ������ʬ
						continue;
					}
					nowsuffer->get_hit(damage); //��¼��ǰ�ܵ���ɱЧ��Ӱ������
					just_hit_minion(hittee);
					if (nowsuffer->read_health() >= 0) { //��ɱ����
						return ;
					}
				}
			}
		}
	}
}


int battle(vector<minion*>& here, vector<minion*>& there, int detail) {
	int i;
	vector<minion*> tauntminion;
	int attind;//���й�����ӵ����
	int index;//�ܵ�������ӵ����
	int attackstate = 0;
	int noattack = 0;
	int myshield = 0;//�ҷ�ʧȥʥ������
	int yourshield = 0;//�з�ʧȥʥ������
	int wolfcount = 0;//˺���Ƕӳ�����
	int captain = 0;//��������
	int pirateAura = 0;//��ħ�⻷����
	int demonAura = 0;//�����⻷����
	int out = 0; //�Ƿ������ʬ
	int windcount = 0; //��ŭ�Ĵ����ж�
	vector<minion*> hitter;
	vector<minion*> hittee;
	vector<minion*> leastEnemy(0); //�����˵Ĺ���Ŀ��
	minion* suffer; //�ܵ����������
	int minattack; //��С������

	// laoxiayanbuff(here, there, -1); //����ǰ����������Ϲ�۵Ĺ�����Ϊ����ֵ����������������Ӱ��
	// lingjunbuff(here, -1); //����ǰ������˫���������buff����������
	// lingjunbuff(there, -1); //ͬ��
	// gongchengbuff(here, -1);//����ǰ�����ù��Ƕ�ħ����ħ�Ĺ�����buff
	// gongchengbuff(there, -1);//ͬ��

	

	

	readAura(here, pirateAura, demonAura); //�����⻷
	setAura(here, pirateAura, demonAura);

	readAura(there, pirateAura, demonAura); //�з��⻷
	setAura(there, pirateAura, demonAura);
	
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
		if (attind == -1) {//�Լ���Ӷ������Թ�����
			if (noattack) {//�����Ѿ������޹��������
				return -2;//ƽ��
			}
			else {
				noattack = 1;
				continue;//�ö��ֽ��й���
			}
		}
		else {//�ҵ��ܹ�������ӣ����ò��ܹ���״̬
			noattack = 0;
		}

		

		if(hitter[attind]->read_racial() == PIRATE) { //��������ʱ��������Ч
			wolfcount = 0;//����˺���Ƕӳ�ͳ��
			captain = 0;//���ô�������

			for (i = 0; i < hitter.size(); i++) { //ͳ��˺���Ƕӳ�����
				if (i == attind) { //�����Լ�������˺����buff���Լ�
					continue;
				}
				if(hitter[i]->read_name() == "˺���Ƕӳ�") {
					wolfcount++;
				}
			}

			for (i = 0; i < hitter.size(); i++) { //ͳ�ƽ�������
				if(hitter[i]->read_name() == "���齢��������") {
					captain++;
				}
			}


			hitter[attind]->gain_buff(wolfcount * 2, wolfcount * 2, 0, 0, 0);
			while(captain) { //�������齢����Ч��ȫ��buff��ֱ�����н�����������
				for (i = 0; i < hitter.size(); i++) {
					hitter[i]->gain_buff(1, 1, 0, 0, 0);
				}
				captain--;
			}
		}

		if (hitter[attind]->read_name() == "���Ļ�����") {//��������Ч
			hitter[attind]->gain_buff(hitter[attind]->read_attack(), 0, 0, 0, 0);
		}

		if (hitter[attind]->has_shield() && hittee[index]->read_attack() > 0) {//�жϵ�ǰ���й�������Ƿ��ʧȥʥ��
			myshield = 1;
		}
		if (hittee[index]->has_shield()) {
			yourshield++;
		}

		

		if (hitter[attind]->read_name() == "������") {
			leastEnemy.clear();
			// �ҵ��з��������С�Ĺ�����
			minattack = hittee[0]->read_attack();
			for (i = 1; i < hittee.size(); i++) {
				if (hittee[i]->read_attack() < minattack) {
					minattack = hittee[i]->read_attack();
				}
			}

			// �ѹ�������С����ӷ��뼯����
			for (i = 0; i < hittee.size(); i++) {
				if (hittee[i]->read_attack() == minattack) {
					leastEnemy.push_back(hittee[i]);
				}
			}
			i = rand() % leastEnemy.size();
			for (index = 0; index < hittee.size(); index++) { //�ҵ�������С��ӵı��
				if (hittee[index] == leastEnemy[i]) {
					break;
				}
			}
		}
		
		hitter[attind]->hit(hittee[index]); //������Ϸ
		suffer = hittee[index];

		if (hitter[attind]->read_name() == "���洫���") {
			fire_overkill(suffer, hittee, yourshield, 3);
		}
		else if (hitter[attind]->read_name() == "���˾���") {
			just_hit_minion(hittee);
			if (suffer->read_health() < 0) {
				for (i = 0; i < hitter.size(); i++) {
					if (i == attind) { //����buff�Լ�
						continue;
					}
					else if (hitter[i]->read_racial() == PIRATE || hitter[i]->read_racial() == ALL) { //��������������buff
						hitter[i]->gain_buff(2, 2, 0, 0, 0);
					}
				}
			}
		}

		if (detail) {
			if (attackstate % 2) {
				cout << endl << "���ַ����й���" << endl;
			}
			else {
				cout << endl << "���ַ����й���" << endl;
			}
			cout << hitter[attind]->read_name() << "������" << suffer->read_name() << endl;
		}


		//��ս����Ч�ж�
		if (hitter[attind]->hit_nearby()) {
			if (index > 0) {
				if (hittee[index - 1]->has_shield()) {
					yourshield++;
				}
				hittee[index - 1]->get_hit(hitter[attind]->read_attack());
			}
			if (index < hittee.size() - 1) {
				if (hittee[index + 1]->has_shield()) {
					yourshield++;
				}
				hittee[index + 1]->get_hit(hitter[attind]->read_attack());
			}
		}

		for (i = 0; i < hitter.size(); i++) {//�ҷ���ӻ��ʧȥʥ������
			if (hitter[i]->read_name() == "ԡ���߲��߶�") {
				hitter[i]->gain_buff(myshield * 2, 0, 0, 0, 0);
			}
			else if (hitter[i]->read_name() == "����ִ����") {
				hitter[i]->gain_buff(myshield * 2, myshield * 2, 0, 0, 0);
			}
		}

		for (i = 0; i < hittee.size(); i++) {//�з���ӻ��ʧȥʥ������
			if (hittee[i]->read_name() == "ԡ���߲��߶�") {
				hittee[i]->gain_buff(yourshield * 2, 0, 0, 0, 0);
			}
			else if (hittee[i]->read_name() == "����ִ����") {
				hittee[i]->gain_buff(yourshield * 2, yourshield * 2, 0, 0, 0);
			}
		}

		myshield = 0;//����ʥ������ͳ��
		yourshield = 0;

		if (!hitter[attind]->has_windfury()) {
			hitter[attind]->set_attacked();
		}
		else {
			if (windcount == 0) { //�ٸ���ŭ��һ�ι�������
				attackstate--;
				windcount = 1;
			}
			else { // ��ŭ�����þ�
				windcount = 0;
				hitter[attind]->set_attacked();
			}
		}


		if (attackstate % 2) {
			there = hittee;
			here = hitter;
		}
		else {
			here = hittee;
			there = hitter;
		}
		
		laoxiayanbuff(here, there, -1); //������������ǰ����������Ϲ�۹���������ֹ���ֽ������
		lingjunbuff(here, -1);
		lingjunbuff(there, -1); //�������˹�����������ͬ��Ϲ��
		gongchengbuff(here, -1);//���ö�ħ������������ͬ���
		gongchengbuff(there, -1);

		
		while (1){
			out = 0;
			out += clearMinion(here, there);
			out += clearMinion(there, here);
			if (!out) { //���ҽ���˫�������������ʱ����
				break;
			}
		}

		laoxiayanbuff(here, there, 1); //��ײǰ������Ϲ�۽���buff
		lingjunbuff(here, 1);
		lingjunbuff(there, 1); //��ײǰ��ʹ�������buff��Ч
		gongchengbuff(here, 1);
		gongchengbuff(there, 1);
		

		// �����Ӻ����¼������buff���
		readAura(here, pirateAura, demonAura);
		refreshAura(here, pirateAura, demonAura);
		readAura(there, pirateAura, demonAura);
		refreshAura(there, pirateAura, demonAura);

		if (detail) {
			showMinion(here);
			showMinion(there);
			cout << endl ;
			system("pause");
		}
		
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


unordered_map<string, vector<int>> card_info(int no) {//�����Ѵ��ڵĿ�����Ϣ
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
		if (name == "�����Ϳ��Ż�")
			info[6] = MECHSS + 3;
		card[name] = info;

		if ((info[8] == DEMON && name != "С������") || info[8] == ALL) { //���ڶ�ħ���ںϹ֣�������������ӿ���
			info[3] = 1;
			GuimamaDemon[name] = info;
			GuimamaName.push_back(name);
		}
		if (info[8] == PIRATE || info[8] == ALL) { //���ں��������뺣�������ɹź�����
			BoatPirate[name] = info;
			BoatName.push_back(name);
		}

	}
	return card;
}


//�������������
void choose_minion(vector<minion*>& lineup) {
	string name;
	int attack, health, shield, taunt, reborn;
	int i;
	while (cin >> name) {
		if (name == "����")
			break;
		for (i = 0; i < 6; i++) {
			if (cards[i].count(name)) {
				lineup.push_back(new minion(name, cards[i][name]));
				cout << "�������Ҫ�ҵ���һ����Ҫ����buff��" << endl;
				cout << "������Ѫ����ʥ�ܡ���������" << endl;
				cin >> attack >> health >> shield >> taunt >> reborn;
				lineup[lineup.size() - 1]->gain_buff(attack, health, shield, taunt, reborn);
				cout << "Ŷ������ǰ����һ��" << endl;
				break;
			}
		}
		for (i = 0; i < lineup.size(); i++)
			lineup[i]->showcard();
		if (lineup.size() == 7) {
			cout << "�һᰵ��֧����ģ��ɱ���߱���" << endl;
			break;
		}
		if (i == 6) {
			cout << "û��ϵ�������ⲻ����" << endl;
		}
	}
}

void read_minion(vector<minion*>& upminion, string info) {
	if (upminion.size() == 7) {
		cout << "��ǰ�����Ѵﵽ���ޣ������Ƿ����ǻ���" << endl;
		return;
	}
	else {
		istringstream words(info);
		string word;//������Ϣ
		string name;
		int i = 0;//ǰ��Ϊ������Ϣ������Ϊ����buff
		int j;//�����������ݿ��������
		int windfury = 0; //��ŭ
		vector<int> info(10, 0);
		//ʥ��2�� ����3�� ��ս4�� ����5�� ����6�� �綾7�� ����8�� ��Ч9
		while (words >> word) {
			if (i == 0) {
				name = word;
			}
			else if (i < 3) {
				info[i - 1] = stoi(word);
			}
			else if(word == "ʥ��"){
				info[2] = 1;
			}
			else if (word == "����") {
				info[3] = 1;
			}
			else if (word == "����") {
				info[5] = 1;
			}
			else if (word == "�綾") {
				info[7] = 1;
			}
			else if (word == "��ŭ") {
				windfury = 1;
			}
			else if (word == "��е") {
				if(info[6] == 0) {
					info[6] = MECHSS + 3;
				}
				else {
					info[6] += 3;
				}
			}
			else if (word == "ֲ��") {
				if (info[6] == 0) {
					info[6] = VEGE + 2;
				}
				else {
					info[6] += 2;
				}
			}
			else {
				cout << "��⵽������Ϣ" << endl;
			}
			i++;
		}
		for (j = 0; j < 6; j++) {
			if (cards[j].count(name))
				break;
		}
		if (j == 6) {
			cout << "���������:" << name << endl;
			return;
		}
		else {
			minion* miniona = new minion(name, cards[j][name]);
			miniona->reset_minion(info);//�����������ĺͶ�Ӧbuff
			if (windfury) { miniona->set_windfury(); }
			if (name == "������" || name == "���˾���") { miniona->set_windfury(); }
			upminion.push_back(miniona);//ѹ����Ӷ�����
		}
	}

}

int main() {
	int win = 0; //1000��ͳ��
	int deal = 0;
	int lose = 0;
	int i;
	int end;
	int n = 1000; //�ܳ���
	int detail = 0; //�Ƿ�鿴ϸ�ڣ�Ĭ��Ϊ��
	int state;
	string command;
	int readstate = 0;
	vector<minion*> myminion; //�ҷ����
	vector<minion*> yourminion; //�з����
	for (i = 1; i < 7; i++) {
		cards.push_back(card_info(i));
	}
	srand((int)time(NULL));
	cout << "������ϲ����ָ�ӹٻ����ˣ�׼����һ����" << endl << endl << "����1�鿴��ӣ� ����2ѡ����ӣ� ����3��ȡд�ó��棬 ����4�鿴������սϸ��" << endl;

	cin >> command;

	//����Ϊ1�鿴�ƹݣ�2ѡ����ӣ�3���ļ���ȡ���ݣ�4�鿴������սϸ��
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
	else if (command == "2") {

		cout << endl << "���ڿ�ʼѡ�������ӣ��������ǵ�����" << endl;
		choose_minion(myminion);

		cout << endl << "���ڿ�ʼѡ����ֵ���ӣ��������ǵ�����" << endl;
		choose_minion(yourminion);
	}
	else if (command == "3") {
		string fileName = "�������.txt";
		ifstream readFile(fileName);
		string s;
		while (getline(readFile, s)) {
			if (s == "") {//�����հ��У�״̬ת��
				readstate = 1;//��ʼ��ȡ�з����
				continue;
			}
			if (!readstate) {
				read_minion(myminion, s);
			}
			else {
				read_minion(yourminion, s);
			}
		}
	}
	else if (command == "4") {
		string fileName = "�������.txt";
		ifstream readFile(fileName);
		string s;
		while (getline(readFile, s)) {
			if (s == "") {//�����հ��У�״̬ת��
				readstate = 1;//��ʼ��ȡ�з����
				continue;
			}
			if (!readstate) {
				read_minion(myminion, s);
			}
			else {
				read_minion(yourminion, s);
			}
		}
		n = 1;
		detail = 1;
	}
	else {
		cout << "�����ڵ�����" << endl;
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
		if (i == 0) {//��һ��ѭ��ʱ�����˫������
			cout << "������:" << endl;
			showMinion(here);
			cout << "�з����:" << endl;
			showMinion(there);
		}
		if (here.size() == there.size()) {//�������һ���࣬�Ⱥ��ֶ�����
			state = rand() % 2;//1Ϊhere���֣�0Ϊ����
			if (state)
				end = battle(here, there, detail);
			else
				end = battle(there, here, detail);
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
				end = battle(here, there, detail);
				judgeWin(win, deal, lose, end);
			}
			else {
				end = battle(there, here, detail);
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