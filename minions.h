#pragma once
#include<iostream>
#include<vector>
#include<string>
#define MOUSE 8

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

	//���ع�����
	int read_attack() { return attack; }

	//��������ֵ
	int read_health() { return health; }

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

	//��������
	virtual ~minion() {
		//std::cout << name << "���Ƴ�" << std::endl;
		
	}

private:
	std::string name;//����
	int health;//����ֵ
	int attack;//������
	int alive;//���Ϊ1������Ϊ0
	int shield;//ʥ��
	int taunt;//����
	int nearby;//��ս��
	int reborn;//����
	int deathrattle;//����
	int poisonous = 0;//�綾
	int racial;//����
	int attacked = 0;
};