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
#define REBORN	20
#define BEAST	1
#define DEMON	2
#define MECH	3
#define	DRAGON	4
#define MURLOC	5
#define PIRATE	6
#define ALL		7

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

	//简单衍生物(带种族)
	minion(std::string namein, int attackin, int healthin, int racialin) {
		name = namein;
		attack = attackin;
		health = healthin;
		racial = racialin;
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

	//带数组构造函数
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

	//返回攻击力
	int read_attack() { return attack; }

	//返回生命值
	int read_health() { return health; }

	//返回种族
	int read_racial() { return racial; }

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
		if (has_shield()) {
			lose_shield();
			return;
		}
		health -= injure;
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

	//复生后失去复生
	void lose_reborn() { reborn = 0; }

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

	//有特效
	int has_special() { return special; }

	//获取buff，包括身材、圣盾、嘲讽、复生
	void gain_buff(int gain_attack, int gain_health, int gain_shield, int gain_taunt, int gain_reborn) {
		attack += gain_attack;
		health += gain_health;
		if (!shield)
			shield = gain_shield;
		if (!taunt)
			taunt = gain_taunt;
		if (!reborn)
			reborn = gain_reborn;
	}

	//展示卡片
	void showcard() {
		std::cout << name << ' ';
		std::cout << attack << '/' << health << ' ';
		if (racial == MURLOC)
			std::cout << "鱼人 ";
		else if (racial == BEAST)
			std::cout << "野兽 ";
		else if (racial == DRAGON)
			std::cout << "龙 ";
		else if (racial == DEMON)
			std::cout << "恶魔 ";
		else if (racial == MECH)
			std::cout << "机械 ";
		else if (racial == PIRATE)
			std::cout << "海盗 ";
		else if (racial == ALL)
			std::cout << "全部";
		if (shield)
			std::cout << "圣盾 ";
		if (taunt)
			std::cout << "嘲讽 ";
		if (nearby)
			std::cout << "狂战 ";
		if (reborn)
			std::cout << "复生 ";
		if (deathrattle)
			std::cout << "亡语 ";
		if (special)
			std::cout << "特效 ";
		std::cout << std::endl;
	}

	//析构函数
	virtual ~minion() {
		//std::cout << name << "被移除" << std::endl;
		
	}

private:
	std::string name;//名字
	int health;//生命值
	int attack;//攻击力
	int alive = 1;//存活为1，死亡为0
	int shield = 0;//圣盾
	int taunt = 0;//嘲讽
	int nearby = 0;//狂战斧
	int reborn = 0;//复生
	int deathrattle = 0;//亡语
	int poisonous = 0;//剧毒
	int racial;//种族
	int attacked = 0;
	int special = 0;
};