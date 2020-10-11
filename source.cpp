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
unordered_map<string, vector<int>> GuimamaDemon; //鬼妈妈的生成恶魔
vector<string> GuimamaName; //鬼妈妈生成恶魔的名字
unordered_map<string, vector<int>> BoatPirate; //海盗船的生成海盗
vector<string> BoatName; //蒙古海军的名字
int enemyNeigui = 0; //为敌方生成内鬼的数量




//输出当前vector里存的随从
void showMinion(vector<minion*>& infos) {
	for (int i = 0; i < infos.size(); i++){
		infos[i]->showcard();
	}
	cout << "展示完毕" << endl;
	cout << "-------------------------" << endl;
}

//实现钴制卫士特效
void guzhibuff(vector<minion*>& infos, vector<minion*>& temp){
	int i;
	for(i = 0; i < infos.size(); i++){
		if(infos[i] == NULL){
			continue;
		}
		else if(infos[i]->read_name() == "偏折机器人"){
			infos[i]->gain_buff(1, 0, 1, 0, 0);
		}
	}
	for(i = 0; i < temp.size(); i++){
		if(temp[i] == NULL){
			continue;
		}
		else if(temp[i]->read_name() == "偏折机器人"){
			temp[i]->gain_buff(1, 0, 1, 0, 0);
		}
	}
}

//实现食腐土狼特效
void tulangbuff(vector<minion*>& infos, vector<minion*>& temp) {
	int i;
	for (i = 0; i < infos.size(); i++) {
		if(infos[i] == NULL) {
			continue;
		}
		else if (infos[i]->read_name() == "食腐土狼") {
			infos[i]->gain_buff(2, 1, 0, 0, 0);
		}
	}
	for (i = 0; i < temp.size(); i++) {
		if (temp[i] == NULL) {
			continue;
		}
		else if (temp[i]->read_name() == "食腐土狼") {
			temp[i]->gain_buff(2, 1, 0, 0, 0);
		}
	}
}

//实现破铜烂铁机器人特效
void lajibuff(vector<minion*>& infos, vector<minion*>& temp) {
	int i;
	for (i = 0; i < infos.size(); i++) {
		if(infos[i] == NULL) {
			continue;
		}
		else if (infos[i]->read_name() == "回收机器人") {
			infos[i]->gain_buff(2, 2, 0, 0, 0);
		}
	}
	for (i = 0; i < temp.size(); i++) {
		if (temp[i] == NULL) {
			continue;
		}
		else if (temp[i]->read_name() == "回收机器人") {
			temp[i]->gain_buff(2, 2, 0, 0, 0);
		}
	}
}

//实现老瞎眼特效
void laoxiayanbuff(vector<minion*>& myminion, vector<minion*>& yourminion, int gain){ //gain为1时，老瞎眼加攻，gain为-1时，老瞎眼减少攻击
	int i;
	int count = -1; //老瞎眼自己不统计在内
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
		if(myminion[i]->read_name() == "老瞎眼") {
			myminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
		}
	}
	for (i = 0; i < yourminion.size(); i++) {
		if (yourminion[i]->read_name() == "老瞎眼") {
			yourminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
		}
	}
}

//实现鱼人领军特效
void lingjunbuff(vector<minion*>& myminion, int gain){ //gain为1时，给全体鱼人加攻，gain为-1时，减少攻击
	int i;
	int count = 0;
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_name() == "鱼人领军") { // 统计场上领军数量，每有一个加2攻
			count += 2;
		}
	}
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_racial() == MURLOC || myminion[i]->read_racial() == ALL) {
			myminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
			if (myminion[i]->read_name() == "鱼人领军") {//领军自己不吃自己buff
				myminion[i] ->gain_buff((-2) * gain, 0, 0, 0, 0);
			}
		}
	}
}

//实现攻城恶魔特效
void gongchengbuff(vector<minion*>& myminion, int gain) {
	int i;
	int count = 0;
	for (i = 0; i < myminion.size(); i++) {
		if (myminion[i]->read_name() == "攻城恶魔") {
			count++;
		}
	}
	for (i = 0; i < myminion.size(); i++) {
		if(myminion[i]->read_racial() == DEMON || myminion[i]->read_racial() == ALL) {
			myminion[i]->gain_buff(count * gain, 0, 0, 0, 0);
			if (myminion[i]->read_name() == "攻城恶魔") {
				myminion[i]->gain_buff((-1) * gain, 0, 0, 0, 0);
			}
		}
	}
}

//读取场上光环
void readAura(vector<minion*>& infos, int& pirateAura, int& demonAura) { 
	int i;
	pirateAura = 0;
	demonAura = 0;
	for (i = 0; i < infos.size(); i++) { //读取场上已有光环数量
		if (infos[i]->read_name() == "玛尔加尼斯") {
			demonAura++;
		}
		else if (infos[i]->read_name() == "南海船长") {
			pirateAura++;
		}
	}
}

//初始化场上随从的光环计数
void setAura(vector<minion*>& infos, int& pirateAura, int& demonAura) { 
	int i;
	for (i = 0; i < infos.size(); i++) {
		if (infos[i]->read_racial() == PIRATE) { //初始化南海船长数量
			infos[i]->initial_pirate_aura(pirateAura);
		}
		else if (infos[i]->read_racial() == DEMON) { //初始化二王光环数量
			infos[i]->initial_demon_aura(demonAura);
		}
		else if (infos[i]->read_racial() == ALL) { //融合怪单独处理
			infos[i]->initial_demon_aura(demonAura);
			infos[i]->initial_pirate_aura(pirateAura);
		}
	}
}

//刷新场上随从光环效果
void refreshAura(vector<minion*>& infos, int& pirateAura, int& demonAura) {
	int racial = 0;
	for (int i = 0; i < infos.size(); i++) {
		racial = infos[i]->read_racial();
		if (racial <= DEMON || racial == ALL) {
			infos[i]->racial_buff(pirateAura, demonAura);
		}
	}
}

//受到来自敌对随从的非正常攻击(爆爆、喷子)
void untouchHit(vector<minion*>& enemys, int injury) {
	int i, x;
	int j;
	vector<minion*> availMinion(0);
	for (i = 0; i < enemys.size(); i++) { //将所有仍有血量的随从加入数组中
		if (enemys[i]->read_health() > 0) {
			availMinion.push_back(enemys[i]);
		}
	}
	x = availMinion.size();
	if (!x) { //所有随从都已死亡，无法再受到伤害
		return ;
	}
	else {
		i = rand() % x; //随机选取一名可受伤随从
		if (availMinion[i]->has_shield()) { //受到伤害随从具有圣盾，与圣盾相关随从获得buff
			for (j = 0; j < enemys.size(); j++) {
				if (enemys[j]->read_name() == "龙人执行者") {
					enemys[j]->gain_buff(2, 2, 0, 0, 0);
				}
				else if (enemys[j]->read_name() == "浴火者伯瓦尔") {
					enemys[j]->gain_buff(2, 0, 0, 0, 0);
				}
			}
		}
		availMinion[i]->get_hit(injury);

	}
}

// 随从刚刚受到攻击生成新随从
void just_hit_minion(vector<minion*>& infos) {
	int i, j;
	int pirateAura = 0;
	int demonAura = 0;
	int jh;
	string demonName;
	vector<minion*> temp;
	int d;
	for (i = 0; i < infos.size() && infos.size() < 7; i++) { //满场时不考虑受伤触发
		for (jh = 0; jh < infos[i]->just_got_hit(); jh++) {
			if (infos.size() >= 7) { //防止不受原本的上限影响
				break;
			}
			if (infos[i]->read_name() == "小鬼妈妈") {
				infos.resize(infos.size() + 1); // 增加一格
				for (j = infos.size() - 1; j > i + 1; j--){
					infos[j] = infos[j - 1];
				}
				d = rand() % GuimamaName.size(); //鬼妈随机召唤一个恶魔，其中不包括自己
				demonName = GuimamaName[d];
				infos[i + 1] = new minion(demonName, GuimamaDemon[demonName]);
				readAura(infos, pirateAura, demonAura); //如果出现二王，需要对血量进行重新判定
				refreshAura(infos, pirateAura, demonAura);
			}
			else if(infos[i]->read_name() == "小鬼首领") { //鬼父受伤召唤小鬼
				infos.resize(infos.size() + 1); // 增加一格
				for (j = infos.size() - 1; j > i + 1; j--){
					infos[j] = infos[j - 1];
				}
				infos[i + 1] = new minion("小鬼", 1, 1, DEMON);
				readAura(infos, pirateAura, demonAura); //如果出现二王，需要对血量进行重新判定
				refreshAura(infos, pirateAura, demonAura);
			}
			else if(infos[i]->read_name() == "安保巡游者") {
				infos.resize(infos.size() + 1); // 增加一格
				for (j = infos.size() - 1; j > i + 1; j--){
					infos[j] = infos[j - 1];
				}
				infos[i + 1] = new minion("安保机器人", 2, 3, MECH);
				infos[i + 1]->gain_buff(0, 0, 0, 1, 0); // 召唤物获得嘲讽
				guzhibuff(infos, temp);
			}
		}
	}

	for (i = 0; i < infos.size(); i++) { //重置受到攻击
		infos[i]->reset_just();
	}
}


//删除已经死去的随从
int clearMinion(vector<minion*>& infos, vector<minion*>& enemys) {
	int res = 0;
	int i;
	int j = 0;//用来算reborn
	int idle;//确认亡语、复生的随从数量
	int state;
	int nana = 0;
	int total = 0;//亡语等触发后随从的总数，最大为7
	int xiaohuangshield = 0;//小黄的圣盾
	queue<int> nextstate;
	vector<minion*> temp(7);
	vector<string> reborns;
	vector<minion*> neiguiSet(0);
	vector<int> toshield;//获得小黄的圣盾的随从编号
	vector<int> pucongatt(0);
	int deathtime = 1;//瑞文的效果
	int d;
	int jh = 0; //刚刚受到伤害次数
	int buffindex;//亡语buff的i变量
	string demonName; //鬼妈召唤恶魔的名字
	int pirateAura = 0; //用于鬼妈碰撞刚好死亡时被二王救活
	int demonAura = 0;
	int beastAttack = 0; //召唤野兽获得的攻击buff
	int beastHealth = 0; //召唤野兽获得的生命buff
	int newbeast = 0; //判断是否为新召唤野兽
	int penziCount = 0; //喷子的数量
	int die = 0; //记录是否有随从死亡，判断是否循环
	int neiguiCount = 0; //内鬼的数量

	neiguiCount = enemyNeigui; //先记录内鬼的数量
	enemyNeigui = 0;

	for (i = 0; i < infos.size(); i++) {
		if (infos[i]->read_health() > 0) {
			if (infos[i]->read_name() == "瑞文戴尔男爵") {
				deathtime = 2;
			}
			else if (infos[i]->read_name() == "灵魂杂耍者") {
				penziCount++;
			}
		}
	}


	just_hit_minion(infos);


	for (i = 0; i < infos.size(); i++) {
		if (infos[i]->read_health() > 0) {
			if (infos[i]->read_name() == "族群领袖") { //小马加2攻
				beastAttack += 2;
			}
			else if (infos[i]->read_name() == "熊妈妈") { //妈妈+4+4
				beastAttack += 4;
				beastHealth += 4;
			}
		}
	}

	for (i = 0; i < infos.size(); i++) {
		if (!(infos[i]->read_health() > 0)) {//发现列表中有死去的随从
			die = 1;
			if (infos[i]->has_death()) {//随从具有亡语
				if (infos[i]->read_name() == "瘟疫鼠群") {
					for (j = 0; j < infos[i]->read_attack(); j++) {
						for (d = 0; d < deathtime; d++)	nextstate.push(MOUSE);
						total += deathtime;
					}
				}
				else if (infos[i]->read_name() == "寄生饿狼") {
					for(d = 0; d < deathtime * 2; d++) nextstate.push(SPIDER);
					total += 2 * deathtime;
				}
				else if (infos[i]->read_name() == "长鬃草原狮") {
					for (d = 0; d < deathtime * 2; d++) nextstate.push(LION);
					total += 2 * deathtime;
				}
				else if (infos[i]->read_name() == "慈祥的外婆"){
					for (d = 0; d < deathtime; d++) nextstate.push(WAIPO);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "小鬼囚徒") {
					for (d = 0; d < deathtime; d++)nextstate.push(LDEMON);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "虚空领主") {
					for (d = 0; d < deathtime * 3; d++) nextstate.push(LANPANG);
					total += deathtime * 3;
				}
				else if (infos[i]->read_name() == "麦田傀儡") {
					for (d = 0; d < deathtime; d++) nextstate.push(MAITIAN);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "拜戈尔格国王") {
					for (d = 0; d < deathtime; d++) nextstate.push(MURKING);
				}
				else if (infos[i]->read_name() == "巨狼戈德林") {
					for (d = 0; d < deathtime; d++) nextstate.push(LANGDIE);
				}
				else if (infos[i]->has_death() > VEGE) { //植物亡语的宏设定值较大，因此先进行判断
					for (j = 0; j < infos[i]->has_death() - VEGE; j++) {
						for(d = 0; d < deathtime; d++) nextstate.push(VEGE);
						total += deathtime;
					}
				}
				else if (infos[i]->has_death() > MECHSS){//机械亡语，将宏设定为较大值，通过差值确认亡语数量
					for (j = 0; j < infos[i]->has_death() - MECHSS; j++) {
						for (d = 0; d < deathtime; d++) nextstate.push(LMECH);
						total += deathtime;
					}
				}
				else if (infos[i]->read_name() == "机械蛋") {
					for (d = 0; d < deathtime; d++) nextstate.push(EGG);
					total += deathtime;
				}
				else if (infos[i]->read_name() == "海浪剃刀号") {
					for (d = 0; d < deathtime * 3; d++) nextstate.push(MENGGU);
					total += deathtime * 3;
				}
				else if (infos[i]->read_name() == "比斯巨兽") {
					enemyNeigui += deathtime; //给敌方随从塞内鬼
				}
				else if (infos[i]->read_name() == "红衣纳迪娜") {
					nana = 1;
				}
				else if (infos[i]->read_name() == "爆爆机器人") {
					for (d = 0; d < deathtime; d++) nextstate.push(BOMB);
				}
				else if (infos[i]->read_name() == "无私的英雄") {
					xiaohuangshield += deathtime;
				}
				else if (infos[i]->read_name() == "恩佐斯的子嗣") {
					for (d = 0; d < deathtime; d++) nextstate.push(ERZI);
				}
				else if (infos[i]->read_name() == "邪魔仆从"){
					for (d = 0; d < deathtime; d++) {
						pucongatt.push_back(infos[i]->read_attack());
					}
				}
			}
			if (infos[i]->has_reborn()) {//发现死亡随从具有复生
				nextstate.push(REBORN);
				reborns.push_back(infos[i]->read_name());
				total++;
			}
			if (infos[i]->read_racial() == BEAST) { //死亡随从是野兽
				nextstate.push(DIEBST);
			}
			else if (infos[i]->read_racial() == MECH) { //死亡随从是机械
				nextstate.push(DIEMECH);
			}
			else if (infos[i]->read_racial() == DEMON) { //死亡随从是恶魔
				for (d = 0; d < penziCount; d++) nextstate.push(PENZI);
			}
			else if (infos[i]->read_racial() == ALL) { //死亡随从是融合怪类型
				nextstate.push(DIEBST);
				nextstate.push(DIEMECH);
				for (d = 0; d < penziCount; d++) nextstate.push(PENZI);
			}
			delete (infos[i]);
		}
		else {
			nextstate.push(i);//记录未死亡随从的原本位置，并保存副本
			temp[i] = infos[i];
			res++;
			total++;
		}
	}


	idle = 7 - res - neiguiCount;
	neiguiSet.resize(neiguiCount);
	for (i = 0; i < neiguiCount; i++) {
		neiguiSet[i] = new minion("内鬼", 3, 3);
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
		if (state < 7) {//原有随从并未死亡， 从temp处放回
			infos[i++] = temp[state];
			temp[state] = NULL;
		}//将亡语产生随从放入
		else if (state == MOUSE) {
			if (idle > 0) {
				infos[i++] = new minion("小老鼠", 1, 1, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == SPIDER) {
			if (idle > 0) {
				infos[i++] = new minion("小蜘蛛", 1, 1, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == LION) {
			if (idle > 0) {
				infos[i++] = new minion("小狮子", 2, 2, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == WAIPO) {
			if (idle > 0) {
				infos[i++] = new minion("狼外婆", 3, 2, BEAST);
				newbeast = 1;
				idle--;
			}
		}
		else if (state == LDEMON) {
			if (idle > 0) {
				infos[i++] = new minion("小鬼", 1, 1, DEMON);
				idle--;
			}
		}
		else if (state == LANPANG) {
			if (idle > 0) {
				infos[i++] = new minion("虚空行者", 1, 3, 0, 1, 0, 0, 0, 0, DEMON);
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
				infos[i++] = new minion("稻草人", 2, 1, MECH);
				idle--;
				guzhibuff(infos, temp);
			}
		}
		else if (state == LMECH) {
			if (idle > 0) {
				infos[i++] = new minion("小机器人", 1, 1, MECH);
				idle--;
				guzhibuff(infos, temp);
			}
		}
		else if (state == VEGE) {
			if(idle > 0) {
				infos[i++] = new minion("小植物", 1, 1);
				idle--;
			}
		}
		else if (state == EGG) {
			if (idle > 0) {
				infos[i++] = new minion("机械恐龙", 8, 8, MECH);
				idle--;
				guzhibuff(infos, temp);
			}
		}
		else if (state == REBORN) {//放入复生随从
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
				if (infos[i]->read_racial() == BEAST || infos[i]->read_racial() == ALL) { //复生了野兽
					infos[i]->gain_buff(beastAttack, beastHealth, 0, 0, 0);
				}
				i++;
				j++;
			}
		}
		else if (state == ERZI) { //儿子亡语buff全体
			for (buffindex = 0; buffindex < i; buffindex++) {//对已经进入最终随从区域的随从buff
				infos[buffindex]->gain_buff(1, 1, 0, 0, 0);
			}
			for (buffindex = 0; buffindex < 7; buffindex++) {//对处在temp数组的随从buff
				if (temp[buffindex] == NULL) {
					continue;
				}
				else
					temp[buffindex]->gain_buff(1, 1, 0, 0, 0);
			}
			for (buffindex = 0; buffindex < neiguiCount; buffindex++) { //对内鬼们进行buff
				neiguiSet[buffindex]->gain_buff(1, 1, 0, 0, 0);
			}
		}
		else if (state == MURKING) { //国王亡语buff鱼人
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
		else if (state == LANGDIE) { //狼爹亡语buff野兽， 同上
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
		else if (state == DIEBST) { //野兽死亡，对土狼buff
			tulangbuff(infos, temp);
		}
		else if (state == DIEMECH) { //机械死亡，对回收机器人buff
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

	if (infos.size() < 7 && neiguiCount) { //把内鬼加入随从里
		j = infos.size(); //读取先前随从数量
		if (j + neiguiCount > 7) { //加上内鬼后随从达到上限
			infos.resize(7);
		}
		else {
			infos.resize(j + neiguiCount); //加入内鬼未达到上限
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
	for (i = 0; i < xiaohuangshield; i++) { //小黄赋予圣盾过程
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

	if (infos.size()){ //当场上还剩余随从时才进行传功
		for (i = 0; i < pucongatt.size(); i++) { //邪魔仆从传功
			j = rand() % infos.size();
			infos[j]->gain_buff(pucongatt[i], 0, 0, 0, 0);
		}
	}


	return die || enemyNeigui;
}




//返回即将进行攻击的随从序号
int attackindex(vector<minion*>& knownminion) {
	int i;
	int n = knownminion.size();
	if (n == 0)
		return 0;
	if (knownminion[n - 1]->has_attacked()) {//最后一个随从也已经进行攻击，则重置所有随从的攻击
		for (i = 0; i < n; i++) {
			knownminion[i]->reset_attacked();
		}
		return 0;
	}
	else {
		for (i = n - 2; i > -1; i--) {//从末尾寻找已攻击随从，返回下一个随从的序号，找不到则从头开始攻击
			if (knownminion[i]->has_attacked())
				break;
		}

		i++;

		while (i < n) {//从第一个未攻击过的随从位置开始寻找
			if (knownminion[i]->read_attack() > 0) {//对于有攻击力的随从，返回序号
				return i;
			}
			else {//没有攻击力的随从，设定为已攻击，忽略
				knownminion[i]->set_attacked();
				i++;
			}
		}
		if (i == n)//当前已经无随从可攻击
			return -1;
	}
	return 0;
}

// 超杀龙超杀效果
void fire_overkill(minion* suffer, vector<minion*>& hittee, int& yourshield, int damage) {
	int i;
	minion* nowsuffer;
	just_hit_minion(hittee);
	if (suffer->read_health() < 0) { //达到超杀的条件
		for (i = 0; i < hittee.size(); i++) {
			if (hittee[i] == suffer) { //不再对当前受攻击随从造成影响
				continue;
			}
			else {
				if (hittee[i]->has_shield()) { //超杀效果到达圣盾随从
					yourshield++;
					hittee[i]->lose_shield();
					return ;
				}
				else {
					nowsuffer = hittee[i];
					if (nowsuffer->read_health() < 0) { //防止反复鞭尸
						continue;
					}
					nowsuffer->get_hit(damage); //记录当前受到超杀效果影响的随从
					just_hit_minion(hittee);
					if (nowsuffer->read_health() >= 0) { //超杀结束
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
	int attind;//进行攻击随从的序号
	int index;//受到攻击随从的序号
	int attackstate = 0;
	int noattack = 0;
	int myshield = 0;//我方失去圣盾数量
	int yourshield = 0;//敌方失去圣盾数量
	int wolfcount = 0;//撕心狼队长数量
	int captain = 0;//船长数量
	int pirateAura = 0;//恶魔光环数量
	int demonAura = 0;//海盗光环数量
	int out = 0; //是否结束收尸
	int windcount = 0; //风怒的次数判断
	vector<minion*> hitter;
	vector<minion*> hittee;
	vector<minion*> leastEnemy(0); //猎马人的攻击目标
	minion* suffer; //受到攻击的随从
	int minattack; //最小攻击力

	// laoxiayanbuff(here, there, -1); //开打前，先重置老瞎眼的攻击力为基础值，即不受其他鱼人影响
	// lingjunbuff(here, -1); //开打前，重置双方鱼人领军buff所给攻击力
	// lingjunbuff(there, -1); //同上
	// gongchengbuff(here, -1);//开打前，重置攻城恶魔给恶魔的攻击力buff
	// gongchengbuff(there, -1);//同上

	

	

	readAura(here, pirateAura, demonAura); //己方光环
	setAura(here, pirateAura, demonAura);

	readAura(there, pirateAura, demonAura); //敌方光环
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

		if (tauntminion.size() == 0)//敌方没有嘲讽
			index = rand() % (hittee.size());
		else {//找到嘲讽的随从
			i = rand() % (tauntminion.size());
			for (index = 0; index < hittee.size(); index++) {
				if (hittee[index] == tauntminion[i]) {
					break;
				}
			}
		}


		attind = attackindex(hitter);
		if (attind == -1) {//自己随从都不可以攻击了
			if (noattack) {//对手已经出现无攻击力情况
				return -2;//平局
			}
			else {
				noattack = 1;
				continue;//让对手进行攻击
			}
		}
		else {//找到能攻击的随从，重置不能攻击状态
			noattack = 0;
		}

		

		if(hitter[attind]->read_racial() == PIRATE) { //海盗攻击时触发的特效
			wolfcount = 0;//重置撕心狼队长统计
			captain = 0;//重置船长数量

			for (i = 0; i < hitter.size(); i++) { //统计撕心狼队长个数
				if (i == attind) { //跳过自己，避免撕心狼buff给自己
					continue;
				}
				if(hitter[i]->read_name() == "撕心狼队长") {
					wolfcount++;
				}
			}

			for (i = 0; i < hitter.size(); i++) { //统计舰长个数
				if(hitter[i]->read_name() == "亡灵舰长伊利扎") {
					captain++;
				}
			}


			hitter[attind]->gain_buff(wolfcount * 2, wolfcount * 2, 0, 0, 0);
			while(captain) { //触发亡灵舰长特效，全体buff，直到所有舰长都被触发
				for (i = 0; i < hitter.size(); i++) {
					hitter[i]->gain_buff(1, 1, 0, 0, 0);
				}
				captain--;
			}
		}

		if (hitter[attind]->read_name() == "雕文护卫者") {//翻倍龙特效
			hitter[attind]->gain_buff(hitter[attind]->read_attack(), 0, 0, 0, 0);
		}

		if (hitter[attind]->has_shield() && hittee[index]->read_attack() > 0) {//判断当前进行攻击随从是否会失去圣盾
			myshield = 1;
		}
		if (hittee[index]->has_shield()) {
			yourshield++;
		}

		

		if (hitter[attind]->read_name() == "猎马人") {
			leastEnemy.clear();
			// 找到敌方随从中最小的攻击力
			minattack = hittee[0]->read_attack();
			for (i = 1; i < hittee.size(); i++) {
				if (hittee[i]->read_attack() < minattack) {
					minattack = hittee[i]->read_attack();
				}
			}

			// 把攻击力最小的随从放入集合中
			for (i = 0; i < hittee.size(); i++) {
				if (hittee[i]->read_attack() == minattack) {
					leastEnemy.push_back(hittee[i]);
				}
			}
			i = rand() % leastEnemy.size();
			for (index = 0; index < hittee.size(); index++) { //找到攻击最小随从的编号
				if (hittee[index] == leastEnemy[i]) {
					break;
				}
			}
		}
		
		hitter[attind]->hit(hittee[index]); //攻击正戏
		suffer = hittee[index];

		if (hitter[attind]->read_name() == "火焰传令官") {
			fire_overkill(suffer, hittee, yourshield, 3);
		}
		else if (hitter[attind]->read_name() == "破浪巨人") {
			just_hit_minion(hittee);
			if (suffer->read_health() < 0) {
				for (i = 0; i < hitter.size(); i++) {
					if (i == attind) { //不能buff自己
						continue;
					}
					else if (hitter[i]->read_racial() == PIRATE || hitter[i]->read_racial() == ALL) { //对所有其他海盗buff
						hitter[i]->gain_buff(2, 2, 0, 0, 0);
					}
				}
			}
		}

		if (detail) {
			if (attackstate % 2) {
				cout << endl << "先手方进行攻击" << endl;
			}
			else {
				cout << endl << "后手方进行攻击" << endl;
			}
			cout << hitter[attind]->read_name() << "攻击了" << suffer->read_name() << endl;
		}


		//狂战斧特效判定
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

		for (i = 0; i < hitter.size(); i++) {//我方随从获得失去圣盾增益
			if (hitter[i]->read_name() == "浴火者伯瓦尔") {
				hitter[i]->gain_buff(myshield * 2, 0, 0, 0, 0);
			}
			else if (hitter[i]->read_name() == "龙人执行者") {
				hitter[i]->gain_buff(myshield * 2, myshield * 2, 0, 0, 0);
			}
		}

		for (i = 0; i < hittee.size(); i++) {//敌方随从获得失去圣盾增益
			if (hittee[i]->read_name() == "浴火者伯瓦尔") {
				hittee[i]->gain_buff(yourshield * 2, 0, 0, 0, 0);
			}
			else if (hittee[i]->read_name() == "龙人执行者") {
				hittee[i]->gain_buff(yourshield * 2, yourshield * 2, 0, 0, 0);
			}
		}

		myshield = 0;//重置圣盾增益统计
		yourshield = 0;

		if (!hitter[attind]->has_windfury()) {
			hitter[attind]->set_attacked();
		}
		else {
			if (windcount == 0) { //再给风怒方一次攻击机会
				attackstate--;
				windcount = 1;
			}
			else { // 风怒次数用尽
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
		
		laoxiayanbuff(here, there, -1); //在清除死亡随从前，先重置老瞎眼攻击力，防止出现结算错误
		lingjunbuff(here, -1);
		lingjunbuff(there, -1); //重置鱼人攻击力，理由同老瞎眼
		gongchengbuff(here, -1);//重置恶魔攻击力，理由同领军
		gongchengbuff(there, -1);

		
		while (1){
			out = 0;
			out += clearMinion(here, there);
			out += clearMinion(there, here);
			if (!out) { //当且仅当双方都无随从死亡时结束
				break;
			}
		}

		laoxiayanbuff(here, there, 1); //碰撞前，对老瞎眼进行buff
		lingjunbuff(here, 1);
		lingjunbuff(there, 1); //碰撞前，使鱼人领军buff生效
		gongchengbuff(here, 1);
		gongchengbuff(there, 1);
		

		// 清除随从后，重新检查种族buff情况
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
	if (here.size() > 0)//胜利
		return 1;
	else if (there.size() > 0)//失败
		return 0;
	else {
		return -2;//平局
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


unordered_map<string, vector<int>> card_info(int no) {//读入已存在的卡牌信息
	string fileName = "卡牌信息0";
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
		//圣盾2、 嘲讽3、 狂战4、 复生5、 亡语6、 剧毒7、 种族8、 特效9
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
				if (word == "圣盾") {
					info[2] = 1;
				}
				else if (word == "嘲讽")
					info[3] = 1;
				else if (word == "狂战")
					info[4] = 1;
				else if (word == "复生")
					info[5] = 1;
				else if (word == "亡语")
					info[6] = 1;
				else if (word == "剧毒")
					info[7] = 1;
				else if (word == "鱼人")
					info[8] = MURLOC;
				else if (word == "野兽")
					info[8] = BEAST;
				else if (word == "龙")
					info[8] = DRAGON;
				else if (word == "机械")
					info[8] = MECH;
				else if (word == "全部")
					info[8] = ALL;
				else if (word == "恶魔")
					info[8] = DEMON;
				else if (word == "海盗")
					info[8] = PIRATE;
				else if (word == "特效")
					info[9] = 1;
			}
			i++;
		}
		if (name == "量产型恐吓机")
			info[6] = MECHSS + 3;
		card[name] = info;

		if ((info[8] == DEMON && name != "小鬼妈妈") || info[8] == ALL) { //对于恶魔和融合怪，加入鬼妈妈的随从库中
			info[3] = 1;
			GuimamaDemon[name] = info;
			GuimamaName.push_back(name);
		}
		if (info[8] == PIRATE || info[8] == ALL) { //对于海盗，加入海盗船的蒙古海军中
			BoatPirate[name] = info;
			BoatName.push_back(name);
		}

	}
	return card;
}


//决定出场的随从
void choose_minion(vector<minion*>& lineup) {
	string name;
	int attack, health, shield, taunt, reborn;
	int i;
	while (cin >> name) {
		if (name == "结束")
			break;
		for (i = 0; i < 6; i++) {
			if (cards[i].count(name)) {
				lineup.push_back(new minion(name, cards[i][name]));
				cout << "这就是你要找的那一个，要来点buff吗" << endl;
				cout << "攻击、血量、圣盾、嘲讽、复生" << endl;
				cin >> attack >> health >> shield >> taunt >> reborn;
				lineup[lineup.size() - 1]->gain_buff(attack, health, shield, taunt, reborn);
				cout << "哦豁，你又前进了一大步" << endl;
				break;
			}
		}
		for (i = 0; i < lineup.size(); i++)
			lineup[i]->showcard();
		if (lineup.size() == 7) {
			cout << "我会暗中支持你的，可别告诉别人" << endl;
			break;
		}
		if (i == 6) {
			cout << "没关系人生难免不如意" << endl;
		}
	}
}

void read_minion(vector<minion*>& upminion, string info) {
	if (upminion.size() == 7) {
		cout << "当前场面已达到上限，请检查是否忘记换行" << endl;
		return;
	}
	else {
		istringstream words(info);
		string word;//单独信息
		string name;
		int i = 0;//前三为基础信息，而后为额外buff
		int j;//用来根据数据库生成随从
		int windfury = 0; //风怒
		vector<int> info(10, 0);
		//圣盾2、 嘲讽3、 狂战4、 复生5、 亡语6、 剧毒7、 种族8、 特效9
		while (words >> word) {
			if (i == 0) {
				name = word;
			}
			else if (i < 3) {
				info[i - 1] = stoi(word);
			}
			else if(word == "圣盾"){
				info[2] = 1;
			}
			else if (word == "嘲讽") {
				info[3] = 1;
			}
			else if (word == "复生") {
				info[5] = 1;
			}
			else if (word == "剧毒") {
				info[7] = 1;
			}
			else if (word == "风怒") {
				windfury = 1;
			}
			else if (word == "机械") {
				if(info[6] == 0) {
					info[6] = MECHSS + 3;
				}
				else {
					info[6] += 3;
				}
			}
			else if (word == "植物") {
				if (info[6] == 0) {
					info[6] = VEGE + 2;
				}
				else {
					info[6] += 2;
				}
			}
			else {
				cout << "检测到不明信息" << endl;
			}
			i++;
		}
		for (j = 0; j < 6; j++) {
			if (cards[j].count(name))
				break;
		}
		if (j == 6) {
			cout << "查无随从名:" << name << endl;
			return;
		}
		else {
			minion* miniona = new minion(name, cards[j][name]);
			miniona->reset_minion(info);//导入输入的身材和对应buff
			if (windfury) { miniona->set_windfury(); }
			if (name == "猎马人" || name == "破浪巨人") { miniona->set_windfury(); }
			upminion.push_back(miniona);//压入随从队列中
		}
	}

}

int main() {
	int win = 0; //1000场统计
	int deal = 0;
	int lose = 0;
	int i;
	int end;
	int n = 1000; //总场数
	int detail = 0; //是否查看细节，默认为不
	int state;
	string command;
	int readstate = 0;
	vector<minion*> myminion; //我方随从
	vector<minion*> yourminion; //敌方随从
	for (i = 1; i < 7; i++) {
		cards.push_back(card_info(i));
	}
	srand((int)time(NULL));
	cout << "嘿我最喜欢的指挥官回来了，准备来一局吗" << endl << endl << "输入1查看随从， 输入2选择随从， 输入3读取写好场面， 输入4查看单场对战细节" << endl;

	cin >> command;

	//命令为1查看酒馆，2选择随从，3从文件读取阵容，4查看单场对战细节
	if (command == "1") {
		cout << "输入酒馆的等级" << endl;
		while (cin >> i) {
			for (auto p : cards[i - 1]) {
				minion* single = new minion(p.first, p.second);
				single->showcard();
				delete single;
			}
			cout << endl << "你问我怎么招的这些随从，四个字，漏斗蛋糕" << endl;
		}
	}
	else if (command == "2") {

		cout << endl << "现在开始选择你的随从，输入他们的名字" << endl;
		choose_minion(myminion);

		cout << endl << "现在开始选择对手的随从，输入他们的名字" << endl;
		choose_minion(yourminion);
	}
	else if (command == "3") {
		string fileName = "场上随从.txt";
		ifstream readFile(fileName);
		string s;
		while (getline(readFile, s)) {
			if (s == "") {//读到空白行，状态转换
				readstate = 1;//开始读取敌方随从
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
		string fileName = "场上随从.txt";
		ifstream readFile(fileName);
		string s;
		while (getline(readFile, s)) {
			if (s == "") {//读到空白行，状态转换
				readstate = 1;//开始读取敌方随从
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
		cout << "不存在的命令" << endl;
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
		if (i == 0) {//第一次循环时，输出双方场面
			cout << "你的随从:" << endl;
			showMinion(here);
			cout << "敌方随从:" << endl;
			showMinion(there);
		}
		if (here.size() == there.size()) {//随从数量一样多，先后手都可能
			state = rand() % 2;//1为here先手，0为后手
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
	cout << "胜利" << win << "次,平局" << deal << "次,失利" << lose << "次" << endl << endl;
	if (win > lose) {
		cout << "你可真是个人才啊朋友" << endl;
	}
	else {
		cout << "我给你物色了一些亲人" << endl;
	}
	system("pause");
	return 0;
}