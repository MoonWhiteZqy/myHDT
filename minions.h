#pragma once
#include<iostream>
#include<vector>
#include<string>
#define MOUSE 8

class minion {
public:
	//基础构造函数
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

	//带特效构造函数
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

	//返回攻击力
	int read_attack() { return attack; }

	//返回生命值
	int read_health() { return health; }

	//破盾
	void lose_shield() {
		this->shield = 0;
	}

	//受到非直接伤害
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

	//受到敌人攻击后生命值判断
	void get_hit(int injure, std::string enemy_name) {
		if (this->has_shield()) {
			this->lose_shield();
			return;
		}
		this->health -= injure;
		//std::cout << name << "受到了来自" << enemy_name << "的" << injure << "点伤害,";
		if (this->health < 1) {
			//std::cout << name << "已经死亡" << std::endl;
			this->alive = 0;
		}
		else {
			//std::cout << "当前身材:" << attack << '/' << health << std::endl;
		}
	}

	//返回名字
	std::string read_name() { return name; }

	//判断存活
	int is_alive() { return alive; }

	//双方对撞
	int hit(minion* enemy) {
		int enemy_attack = enemy->read_attack();
		std::string enemy_name = enemy->read_name();
		//std::cout << name << "对" << enemy_name << "发起了攻击" << std::endl;
		if (enemy->is_poisonous())//敌方有剧毒，自己被秒
			get_poisonous();
		else
			get_hit(enemy_attack, enemy_name);
		if (is_poisonous()) {//自己有剧毒，敌方被秒
			enemy->get_poisonous();
		}
		else
			enemy->get_hit(attack, name);
		return this->is_alive() && enemy->is_alive();
	}

	//判断圣盾
	int has_shield() { return shield; }

	//判断嘲讽
	int has_taunt() { return taunt; }
	
	//判断亡语
	int has_death() { return deathrattle; }

	//判断复生
	int has_reborn() { return reborn; }

	//判断狂战斧
	int hit_nearby() { return nearby; }
	
	//判断是否已经攻击
	int has_attacked() { return attacked; }

	//设置为已经攻击
	void set_attacked() { attacked = 1; }

	//重置攻击
	void reset_attacked() { attacked = 0; }

	//判断剧毒
	int is_poisonous() { return poisonous; }

	//剧毒秒杀
	void get_poisonous() { alive = 0; health = 0; }

	//析构函数
	virtual ~minion() {
		//std::cout << name << "被移除" << std::endl;
		
	}

private:
	std::string name;//名字
	int health;//生命值
	int attack;//攻击力
	int alive;//存活为1，死亡为0
	int shield;//圣盾
	int taunt;//嘲讽
	int nearby;//狂战斧
	int reborn;//复生
	int deathrattle;//亡语
	int poisonous = 0;//剧毒
	int racial;//种族
	int attacked = 0;
};