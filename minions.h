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
	//基础构造函数
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

	//简单衍生物(带种族)
	minion(std::string namein, int attackin, int healthin, int racialin) {
		name = namein;
		attack = attackin;
		health = healthin;
		orighealth = healthin;
		racial = racialin;
	}

	//带特效构造函数
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

	//带数组构造函数
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

	// 复制已存在随从快速构造
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
	}

	//返回攻击力
	int read_attack() { return attack; }

	//返回生命值
	int read_health() { return health; }

	//返回初始生命值
	int read_orighealth() { return orighealth; }

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
		just++;
		health -= injure;
	}

	//入场时重设随从数值/亡语
	void reset_minion(std::vector<int>& info){
		int *orig[10] = {&attack, &health, &shield, &taunt, &nearby, &reborn, &deathrattle, &poisonous, &racial, &special};
		for(int i = 0; i < 10; i++){
			if(info[i] > 0){
				*orig[i] = info[i];
			}
		}
	}

	//受到敌人攻击后生命值判断
	void get_hit(int injure, std::string enemy_name) {
		if (has_shield()) {
			lose_shield();
			return;
		}
		health -= injure;
		just = 1;
		//std::cout << name << "受到了来自" << enemy_name << "的" << injure << "点伤害,";
		if (this->health < 1) {
			//std::cout << name << "已经死亡" << std::endl;
		}
		else {
			//std::cout << "当前身材:" << attack << '/' << health << std::endl;
		}
	}

	//返回名字
	std::string read_name() { return name; }


	//双方对撞
	void hit(minion* enemy) {
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
	void get_poisonous() { health = -2000; }

	//有特效
	int has_special() { return special; }

	//获取种族相关buff
	void racial_buff(int pirateNow, int demonNow) { //当当前场上的光环数与记录不同时，根据光环数量的差距进行运算
		int pirateGap;
		int demonGap;
		if (name == "玛尔加尼斯") {
			demonNow--;
		}
		else if (name == "南海船长") {
			pirateNow--;
		}
		pirateGap = pirateNow - pirateAura; //当前光环数量与原本光环数量的差距，不同，则对属性进行调整
		demonGap = demonNow - demonAura;
		if (racial == PIRATE) {
			attack += pirateGap;
			if (pirateGap > 0) { //光环数量增加，血量增加
				health += pirateGap;
			}
			else if (orighealth + pirateNow < health) { //当失去buff后，血量超过本来时，减少血量，否则不变
				health = orighealth + pirateNow; //血量为基础血量加上剩余buff血量
			}
		}
		else if (racial == DEMON) {
			attack += demonGap * 2;
			if (demonGap > 0) {
				health += demonGap * 2;
			}
			else if (orighealth + demonNow * 2 < health) { //同上
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
		pirateAura = pirateNow; //设定新的光环基础数值
		demonAura = demonNow;
	}

	//获取buff，包括身材、圣盾、嘲讽、复生
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
			std::cout << "全部 ";
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

	//判断是否刚刚受到攻击
	int just_got_hit(){
		return just;
	}

	//重设刚刚受到攻击
	void reset_just(){
		just = 0;
	}

	//初始化海盗种族光环数量
	void initial_pirate_aura(int x){
		if (name == "南海船长") {
			x--;
		}
		pirateAura = x;
		if (racial == PIRATE) { //融合怪会在这之后设定二王光环buff，因此此处只对海盗生效
			set_orighealth();
		}
	}

	//初始化二王光环数量
	void initial_demon_aura(int x) {
		if (name == "玛尔加尼斯") {
			x--;
		}
		demonAura = x;
		set_orighealth();
	}

	//设定真实生命
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

	//析构函数
	virtual ~minion() {
		//std::cout << name << "被移除" << std::endl;
		
	}

private:
	std::string name;//名字
	int health;//生命值
	int attack;//攻击力
	int shield = 0;//圣盾
	int taunt = 0;//嘲讽
	int nearby = 0;//狂战斧
	int reborn = 0;//复生
	int deathrattle = 0;//亡语
	int poisonous = 0;//剧毒
	int racial;//种族
	int attacked = 0;
	int special = 0;
	int just = 0; // 刚刚受到攻击(鬼妈、安保、鬼父)
	int orighealth;
	int pirateAura = 0; //南海船长buff
	int demonAura = 0; //二王buff
};