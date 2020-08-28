#pragma once
#include<iostream>
#include<vector>
#include<string>
#define MOUSE	8
#define BEAST	1
#define DEMON	2
#define MECH	3
#define	DRAGON	4
#define MURLOC	5
#define PIRATE	6
#define ALL		7

class minion {
public:
	//�������캯��
	minion(std::string namein, int attackin, int healthin) {
		name = namein;
		attack = attackin;
		health = healthin;
		alive = health > 0;
		shield = 0;
		taunt = 0;
		nearby = 0;
		reborn = 0;
		deathrattle = 0;
		racial = 0;
	}

	//����Ч���캯��
	minion(std::string namein, int attackin, int healthin, int shieldin, int tauntin, int nearbyin, int rebornin, int deathrattlein, int poisonousin, int racialin) {
		name = namein;
		attack = attackin;
		health = healthin;
		alive = health > 0;
		shield = shieldin;
		taunt = tauntin;
		nearby = nearbyin;
		reborn = rebornin;
		deathrattle = deathrattlein;
		poisonous = poisonousin;
		racial = racialin;
	}

	minion(std::string namein, std::vector<int> info) {
		name = namein;
		attack = info[0];
		health = info[1];
		shield = info[2];
		taunt = info[3];
		nearby = info[4];
		reborn = info[5];
		deathrattle = info[6];
		poisonous = info[7];
		racial = info[8];
		special = info[9];
	}

	minion(minion* exist) {
		name = exist->read_name();
		attack = exist->read_attack();
		health = exist->read_health();
		shield = exist->has_shield();
		taunt = exist->has_taunt();
		nearby = exist->hit_nearby();
		reborn = exist->has_reborn();
		deathrattle = exist->has_death();
		poisonous = exist->is_poisonous();
		racial = exist->read_racial();
		special = exist->has_special();
	}

	//���ع�����
	int read_attack() { return attack; }

	//��������ֵ
	int read_health() { return health; }

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
		health -= injure;
		if (health < 1) {
			this->alive = 0;
		}
		else {

		}
	}

	//�ܵ����˹���������ֵ�ж�
	void get_hit(int injure, std::string enemy_name) {
		if (this->has_shield()) {
			this->lose_shield();
			return;
		}
		this->health -= injure;
		//std::cout << name << "�ܵ�������" << enemy_name << "��" << injure << "���˺�,";
		if (this->health < 1) {
			//std::cout << name << "�Ѿ�����" << std::endl;
			this->alive = 0;
		}
		else {
			//std::cout << "��ǰ���:" << attack << '/' << health << std::endl;
		}
	}

	//��������
	std::string read_name() { return name; }

	//�жϴ��
	int is_alive() { return alive; }

	//˫����ײ
	int hit(minion* enemy) {
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
		return this->is_alive() && enemy->is_alive();
	}

	//�ж�ʥ��
	int has_shield() { return shield; }

	//�жϳ���
	int has_taunt() { return taunt; }
	
	//�ж�����
	int has_death() { return deathrattle; }

	//�жϸ���
	int has_reborn() { return reborn; }

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
	void get_poisonous() { alive = 0; health = 0; }

	//����Ч
	int has_special() { return special; }

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
			std::cout << "ȫ��";
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
		if (special)
			std::cout << "��Ч ";
		std::cout << std::endl;
	}

	//��������
	virtual ~minion() {
		//std::cout << name << "���Ƴ�" << std::endl;
		
	}

private:
	std::string name;//����
	int health;//����ֵ
	int attack;//������
	int alive = 1;//���Ϊ1������Ϊ0
	int shield = 0;//ʥ��
	int taunt = 0;//����
	int nearby = 0;//��ս��
	int reborn = 0;//����
	int deathrattle = 0;//����
	int poisonous = 0;//�綾
	int racial;//����
	int attacked = 0;
	int special = 0;
};