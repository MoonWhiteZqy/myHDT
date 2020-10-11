#pragma once
#include<iostream>
#include<vector>
#include<string>
#define MOUSE	8
#define SPIDER	9
#define LION	10
#define MECHSS	100
#define LMECH	11
#define ERZI	12
#define EGG		13
#define LDEMON	14
#define MAITIAN	15
#define WAIPO	16
#define LANPANG	17
#define VEGE	113
#define MENGGU	18
#define MURKING	19
#define LANGDIE	20
#define DIEBST	21
#define DIEMECH	22
#define BOMB	23
#define PENZI	24
#define REBORN	30
#define PIRATE	1
#define DEMON	2
#define MECH	3
#define	DRAGON	4
#define MURLOC	5
#define BEAST	6
#define ALL		7

class minion {
public:
	//�������캯��
	minion(std::string namein, int attackin, int healthin) {
		name = namein;
		attack = attackin;
		health = healthin;
		orighealth = healthin;
		shield = 0;
		taunt = 0;
		nearby = 0;
		reborn = 0;
		deathrattle = 0;
		racial = 0;
	}

	//��������(������)
	minion(std::string namein, int attackin, int healthin, int racialin) {
		name = namein;
		attack = attackin;
		health = healthin;
		orighealth = healthin;
		racial = racialin;
	}

	//����Ч���캯��
	minion(std::string namein, int attackin, int healthin, int shieldin, int tauntin, int nearbyin, int rebornin, int deathrattlein, int poisonousin, int racialin) {
		name = namein;
		attack = attackin;
		health = healthin;
		orighealth = healthin;
		shield = shieldin;
		taunt = tauntin;
		nearby = nearbyin;
		reborn = rebornin;
		deathrattle = deathrattlein;
		poisonous = poisonousin;
		racial = racialin;
	}

	//�����鹹�캯��
	minion(std::string namein, std::vector<int>& info) {
		name = namein;
		attack = info[0];
		health = info[1];
		orighealth = info[1];
		shield = info[2];
		taunt = info[3];
		nearby = info[4];
		reborn = info[5];
		deathrattle = info[6];
		poisonous = info[7];
		racial = info[8];
		special = info[9];
	}

	// �����Ѵ�����ӿ��ٹ���
	minion(minion* exist) {
		name = exist->read_name();
		attack = exist->read_attack();
		health = exist->read_health();
		orighealth = exist->read_health();
		shield = exist->has_shield();
		taunt = exist->has_taunt();
		nearby = exist->hit_nearby();
		reborn = exist->has_reborn();
		deathrattle = exist->has_death();
		poisonous = exist->is_poisonous();
		racial = exist->read_racial();
		special = exist->has_special();
		windfury = exist->has_windfury();
	}

	//���ع�����
	int read_attack() { return attack; }

	//��������ֵ
	int read_health() { return health; }

	//���س�ʼ����ֵ
	int read_orighealth() { return orighealth; }

	//��������
	int read_racial() { return racial; }

	//�ƶ�
	void lose_shield() {
		this->shield = 0;
	}

	//�ܵ���ֱ���˺�
	void get_hit(int injure) {
		if (has_shield()) {
			lose_shield();
			return;
		}
		just++;
		health -= injure;
	}

	//�볡ʱ���������ֵ/����
	void reset_minion(std::vector<int>& info){
		int *orig[10] = {&attack, &health, &shield, &taunt, &nearby, &reborn, &deathrattle, &poisonous, &racial, &special};
		for(int i = 0; i < 10; i++){
			if(info[i] > 0){
				*orig[i] = info[i];
			}
		}
	}

	//�ܵ����˹���������ֵ�ж�
	void get_hit(int injure, std::string enemy_name) {
		if (has_shield()) {
			lose_shield();
			return;
		}
		health -= injure;
		just++;
		//std::cout << name << "�ܵ�������" << enemy_name << "��" << injure << "���˺�,";
		if (this->health < 1) {
			//std::cout << name << "�Ѿ�����" << std::endl;
		}
		else {
			//std::cout << "��ǰ���:" << attack << '/' << health << std::endl;
		}
	}

	//��������
	std::string read_name() { return name; }


	//˫����ײ
	void hit(minion* enemy) {
		int enemy_attack = enemy->read_attack();
		std::string enemy_name = enemy->read_name();
		//std::cout << name << "��" << enemy_name << "�����˹���" << std::endl;
		if (enemy->is_poisonous())//�з��о綾���Լ�����
			get_poisonous();
		else
			get_hit(enemy_attack, enemy_name);
		if (is_poisonous()) {//�Լ��о綾���з�����
			enemy->get_poisonous();
		}
		else
			enemy->get_hit(attack, name);
	}

	//�ж�ʥ��
	int has_shield() { return shield; }

	//�жϳ���
	int has_taunt() { return taunt; }
	
	//�ж�����
	int has_death() { return deathrattle; }

	//�жϸ���
	int has_reborn() { return reborn; }

	//������ʧȥ����
	void lose_reborn() { reborn = 0; }

	//�жϿ�ս��
	int hit_nearby() { return nearby; }
	
	//�ж��Ƿ��Ѿ�����
	int has_attacked() { return attacked; }

	//����Ϊ�Ѿ�����
	void set_attacked() { attacked = 1; }

	//���ù���
	void reset_attacked() { attacked = 0; }

	//�жϾ綾
	int is_poisonous() { return poisonous; }

	//�綾��ɱ
	void get_poisonous() {
		if (shield) {
			lose_shield();
		}
		else {
			health = -2000;
		}
	}

	//����Ч
	int has_special() { return special; }

	//��ȡ�������buff
	void racial_buff(int pirateNow, int demonNow) { //����ǰ���ϵĹ⻷�����¼��ͬʱ�����ݹ⻷�����Ĳ���������
		int pirateGap;
		int demonGap;
		if (name == "�������˹") {
			demonNow--;
		}
		else if (name == "�Ϻ�����") {
			pirateNow--;
		}
		pirateGap = pirateNow - pirateAura; //��ǰ�⻷������ԭ���⻷�����Ĳ�࣬��ͬ��������Խ��е���
		demonGap = demonNow - demonAura;
		if (racial == PIRATE) {
			attack += pirateGap;
			if (pirateGap > 0) { //�⻷�������ӣ�Ѫ������
				health += pirateGap;
			}
			else if (orighealth + pirateNow < health) { //��ʧȥbuff��Ѫ����������ʱ������Ѫ�������򲻱�
				health = orighealth + pirateNow; //Ѫ��Ϊ����Ѫ������ʣ��buffѪ��
			}
		}
		else if (racial == DEMON) {
			attack += demonGap * 2;
			if (demonGap > 0) {
				health += demonGap * 2;
			}
			else if (orighealth + demonNow * 2 < health) { //ͬ��
				health = orighealth + demonNow * 2;
			}
		}
		else if (racial == ALL) {
			attack += pirateGap + demonGap * 2;
			if(pirateGap + demonGap * 2 > 0) {
				health += pirateGap + demonGap * 2;
			}
			else if (orighealth + pirateNow + demonNow * 2 < health) {
				health = orighealth + pirateNow + demonNow * 2;
			}
		}
		pirateAura = pirateNow; //�趨�µĹ⻷������ֵ
		demonAura = demonNow;
	}

	//��ȡbuff��������ġ�ʥ�ܡ���������
	void gain_buff(int gain_attack, int gain_health, int gain_shield, int gain_taunt, int gain_reborn) {
		attack += gain_attack;
		health += gain_health;
		orighealth += gain_health;
		if (!shield)
			shield = gain_shield;
		if (!taunt)
			taunt = gain_taunt;
		if (!reborn)
			reborn = gain_reborn;
	}

	//չʾ��Ƭ
	void showcard() {
		std::cout << name << ' ';
		std::cout << attack << '/' << health << ' ';
		if (racial == MURLOC)
			std::cout << "���� ";
		else if (racial == BEAST)
			std::cout << "Ұ�� ";
		else if (racial == DRAGON)
			std::cout << "�� ";
		else if (racial == DEMON)
			std::cout << "��ħ ";
		else if (racial == MECH)
			std::cout << "��е ";
		else if (racial == PIRATE)
			std::cout << "���� ";
		else if (racial == ALL)
			std::cout << "ȫ�� ";
		if (shield)
			std::cout << "ʥ�� ";
		if (taunt)
			std::cout << "���� ";
		if (nearby)
			std::cout << "��ս ";
		if (reborn)
			std::cout << "���� ";
		if (deathrattle)
			std::cout << "���� ";
		if (poisonous)
			std::cout << "�綾 ";
		if (windfury)
			std::cout << "��ŭ ";
		if (special)
			std::cout << "��Ч ";
		std::cout << std::endl;
	}

	//�ж��Ƿ�ո��ܵ�����
	int just_got_hit(){
		return just;
	}

	//����ո��ܵ�����
	void reset_just(){
		just = 0;
	}

	//��ʼ����������⻷����
	void initial_pirate_aura(int x){
		if (name == "�Ϻ�����") {
			x--;
		}
		pirateAura = x;
		if (racial == PIRATE) { //�ںϹֻ�����֮���趨�����⻷buff����˴˴�ֻ�Ժ�����Ч
			set_orighealth();
		}
	}

	//��ʼ�������⻷����
	void initial_demon_aura(int x) {
		if (name == "�������˹") {
			x--;
		}
		demonAura = x;
		set_orighealth();
	}

	//�趨��ʵ����
	void set_orighealth() {
		if (racial == PIRATE) {
			orighealth = health - pirateAura;
		}
		else if (racial == DEMON) {
			orighealth = health - demonAura * 2;
		}
		else if (racial == ALL) {
			orighealth = health - demonAura * 2 - pirateAura;
		}
	}

	// �����ŭ
	void set_windfury() { windfury = 1;	}

	// �ж��Ƿ���з�ŭ
	int has_windfury() { return windfury; }

	//��������
	virtual ~minion() {
		//std::cout << name << "���Ƴ�" << std::endl;
		
	}

private:
	std::string name;//����
	int health;//����ֵ
	int attack;//������
	int shield = 0;//ʥ��
	int taunt = 0;//����
	int nearby = 0;//��ս��
	int reborn = 0;//����
	int deathrattle = 0;//����
	int poisonous = 0;//�綾
	int racial;//����
	int attacked = 0;
	int special = 0;
	int just = 0; // �ո��ܵ�����(���衢��������)
	int orighealth;
	int pirateAura = 0; //�Ϻ�����buff
	int demonAura = 0; //����buff
	int windfury = 0; //��ŭ
};