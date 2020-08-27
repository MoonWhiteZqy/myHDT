#pragma once
#include<iostream>
#include<vector>
#include<string>

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
	minion(std::string namein, int attackin, int healthin, int shieldin, int tauntin, int nearbyin, int rebornin, int deathrattlein, int racialin) {
		name = namein;
		attack = attackin;
		health = healthin;
		alive = health > 0;
		shield = shieldin;
		taunt = tauntin;
		nearby = nearbyin;
		reborn = rebornin;
		deathrattle = deathrattlein;
		racial = racialin;
	}

	//���ع�����
	int read_attack() {
		return this->attack;
	}

	//��������ֵ
	int read_health() {
		return this->health;
	}

	//�ƶ�
	void lose_shield() {
		this->shield = 0;
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
	std::string read_name() {
		return this->name;
	}

	//�жϴ��
	int is_alive() {
		return alive;
	}

	//˫����ײ
	int hit(minion* enemy) {
		int enemy_attack = enemy->read_attack();
		std::string enemy_name = enemy->read_name();
		//std::cout << name << "��" << enemy_name << "�����˹���" << std::endl;
		get_hit(enemy_attack, enemy_name);
		enemy->get_hit(attack, name);
		return this->is_alive() && enemy->is_alive();
	}

	//�ж�ʥ��
	int has_shield() {
		return shield;
	}

	//�жϳ���
	int has_taunt() {
		return taunt;
	}
	
	//�ж�����
	int has_death() {
		return deathrattle;
	}

	//�жϸ���
	int has_reborn() {
		return reborn;
	}

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
	int racial;//����
};