#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<algorithm>
#include<iomanip>
#include<vector>
#include<list>
#include<deque>
#include<string>
using namespace std;
#define RED 0
#define BLUE 1
int numCity, iniElement, kLion, R, timeEnd, hourEnd, minuteEnd;
int iniStrength[5];//dragon 0, ninja 1, iceman 2, lion 3,wolf 4
int iniForce[5];//dragon 0, ninja 1, iceman 2, lion 3,wolf 4
int lastbornRed = 0, lastbornBlue = 0;
string nameW[5] = { "dragon","ninja","iceman","lion","wolf" };
string nameWe[5] = { "sword","bomb","arrow" };
int taken[2] = { 0 };
void printtime(int tt);
class Headquarter;
class CWarrior;
class CWeapon;
class CCity;
int recFlag[2][22] = { 0 };//0 red 1 blue 用于两次胜利插旗
CWarrior * WaitingRedHeadquarter=NULL;
CWarrior * WaitingBlueHeadquarter=NULL;

class CCity
{
public:
	CWarrior * pRed;
	CWarrior * pBlue;
	int cityflag;
	int cityElement;
	CCity() :cityflag(-1), cityElement(0)
	{
		delete pRed;
		delete pBlue;
		pRed = NULL;
		pBlue = NULL;
	}
	void clearr()
	{
		cityflag = -1;
		cityElement = 0;
		if (pRed != NULL) {
			delete pRed;
			pRed = NULL;
		}
		if (pBlue != NULL) {
			delete pBlue;
			pBlue = NULL;
		}
	}
};
CCity cities[22];
class CWeapon
{
public:
	int numWeapon, WeaponForce, used;
	bool own;
	CWeapon() {};
	CWeapon(int n, int f, int u, int o) :numWeapon(n), WeaponForce(f), used(u), own(o) {};
	virtual CWeapon & operator=(const CWeapon & w)
	{
		numWeapon = w.numWeapon;
		WeaponForce = w.WeaponForce;
		used = w.used;
		own = w.own;
		return *this;
	}
};
class CSword :public CWeapon
{
public:
	CSword(int userF) :CWeapon(0, userF * 2 / 10, 0, 0) {};
	CSword() {};
	CSword(CWeapon & w)
	{
		numWeapon = w.numWeapon;
		WeaponForce = w.WeaponForce;
		used = w.used;
	}
};
class CBomb :public CWeapon
{
public:
	CBomb() :CWeapon(1, 0, 0, 0) {};
	CBomb(CWeapon & w)
	{
		numWeapon = w.numWeapon;
		WeaponForce = w.WeaponForce;
		used = w.used;
	}
};
class CArrow :public CWeapon
{
public:
	CArrow() :CWeapon(2, R, 0, 0) {};
	CArrow(CWeapon & w)
	{
		numWeapon = w.numWeapon;
		WeaponForce = w.WeaponForce;
		used = w.used;
	}
};
class CWarrior
{
public:
	int Nodot, strength, force, loyalty, step;
	double morale;
	CSword hisSword;
	CBomb hisBomb;
	CArrow hisArrow;
	int type;//dragon 0, ninja 1, iceman 2, lion 3,wolf 4
	CWarrior(int No, int st, int fo, int ty, int loy, double mor) :step(0), Nodot(No), strength(st), force(fo), type(ty), loyalty(loy), morale(mor), hisSword(fo), hisArrow(), hisBomb() {};
	CWarrior & operator=(CWarrior & w)
	{
		Nodot = w.Nodot; strength = w.strength; force = w.force; loyalty = w.loyalty; step = w.step; morale = w.morale;
		hisArrow = w.hisArrow; hisBomb = w.hisBomb; hisSword = w.hisSword;
		type = w.type;
		return *this;
	}
	CWarrior() {};
	friend void Output(Headquarter & red, Headquarter & blue);
};
class CDragon :public CWarrior
{
public:
	CDragon(int No, int ele) :CWarrior(No, iniStrength[0], iniForce[0], 0, 0, ((double)ele) / ((double)iniStrength[0]))
	{
		switch (Nodot % 3)
		{
		case 0:if(iniForce[0]>0){hisSword.own = 1;} break;
		case 1:hisBomb.own = 1; break;
		case 2:hisArrow.own = 1; break;
		}
	}
	friend void Output(Headquarter & red, Headquarter & blue);
};
class CNinja :public CWarrior
{
public:
	CNinja(int No) :CWarrior(No, iniStrength[1], iniForce[1], 1, 0, 0)
	{
		switch (Nodot % 3)
		{
		case 0: if(iniForce[1]>0){hisSword.own = 1;} hisBomb.own = 1; break;
		case 1: hisBomb.own = 1; hisArrow.own = 1; break;
		case 2: if(iniForce[1]>0){hisSword.own = 1;} hisArrow.own = 1; break;
		}
	}
};
class CIceman :public CWarrior
{
public:
	CIceman(int No) :CWarrior(No, iniStrength[2], iniForce[2], 2, 0, 0)
	{
		switch (Nodot % 3)
		{
		case 0:if(iniForce[2]>0){hisSword.own = 1; }break;
		case 1:hisBomb.own = 1; break;
		case 2:hisArrow.own = 1; break;
		}
	}
};
class CLion :public CWarrior
{
public:
	CLion(int No, int loy) :CWarrior(No, iniStrength[3], iniForce[3], 3, loy, 0) {};
};
class CWolf :public CWarrior
{
public:
	CWolf(int No) :CWarrior(No, iniStrength[4], iniForce[4], 4, 0, 0) {};
};
class Headquarter
{
	int element; //司令部所剩生命元（总）
	bool color;
	int seqCreateW[5];//制造武士的顺序 red:23410  numW[seqCreateW[j%5]]
	int numW[5];//武士数量 dragon 0, ninja 1, iceman 2, lion 3,wolf 4
public:
	vector<CWarrior> cwarrior;

	//public:
	Headquarter(int element_, int color_) :cwarrior(), element(element_), color(color_)
	{
		memset(numW, 0, sizeof(numW));
		if (color == RED) {
			seqCreateW[0] = 2; seqCreateW[1] = 3; seqCreateW[2] = 4; seqCreateW[3] = 1; seqCreateW[4] = 0;
		}
		else if (color == BLUE) {
			seqCreateW[0] = 3; seqCreateW[1] = 0; seqCreateW[2] = 1; seqCreateW[3] = 2; seqCreateW[4] = 4;
		}
	}
	friend void Output(Headquarter & red, Headquarter & blue);
};
void printtime(int tt)
{
	if (tt<10)
		cout << "00" << tt;
	else if (tt<100)
		cout << "0" << tt;
	else
		cout << tt;
}
bool fight(CWarrior * &attacker, CWarrior * &victim)//有反击返回1
{
	victim->strength -= attacker->force;
	if (attacker->hisSword.own == 1) {
		victim->strength -= attacker->hisSword.WeaponForce;
		attacker->hisSword.WeaponForce = attacker->hisSword.WeaponForce * 8 / 10;
		if (attacker->hisSword.WeaponForce <= 0)
			attacker->hisSword.own = 0;
	}
	if (victim->strength>0 && victim->type != 1) {
		attacker->strength -= victim->force / 2;
		if (victim->hisSword.own == 1) {
			attacker->strength -= victim->hisSword.WeaponForce;
			victim->hisSword.WeaponForce = victim->hisSword.WeaponForce * 8 / 10;
			if (victim->hisSword.WeaponForce <= 0)
				victim->hisSword.own = 0;
		}
		return true;
	}
	return false;
}

void Output(Headquarter & red, Headquarter & blue)
{
	for (int hour = 0; hour <= hourEnd; hour++) {
		if (red.element >= iniStrength[red.seqCreateW[lastbornRed % 5]]) {
			red.element -= iniStrength[red.seqCreateW[lastbornRed % 5]];
			red.numW[red.seqCreateW[lastbornRed % 5]]++;
			switch (red.seqCreateW[lastbornRed % 5])
			{
			case 0:red.cwarrior.push_back(CDragon(lastbornRed + 1, red.element)); break;
			case 1:red.cwarrior.push_back(CNinja(lastbornRed + 1)); break;
			case 2:red.cwarrior.push_back(CIceman(lastbornRed + 1)); break;
			case 3:red.cwarrior.push_back(CLion(lastbornRed + 1, red.element)); break;
			case 4:red.cwarrior.push_back(CWolf(lastbornRed + 1)); break;
			}
			cities[0].pRed = new CWarrior;
			*(cities[0].pRed) = red.cwarrior[red.cwarrior.size() - 1];
			printtime(hour);
			cout << ":00 red " << nameW[red.seqCreateW[lastbornRed % 5]] << " " << lastbornRed + 1 << " born" << endl;
			if (red.seqCreateW[lastbornRed % 5] == 3) {
				cout << "Its loyalty is " << red.cwarrior[red.cwarrior.size() - 1].loyalty << endl;
			}
			if (red.seqCreateW[lastbornRed % 5] == 0) {
				cout << "Its morale is " << setprecision(2) << fixed << red.cwarrior[red.cwarrior.size() - 1].morale << endl;
			}
			lastbornRed++;
		}

		if (blue.element >= iniStrength[blue.seqCreateW[lastbornBlue % 5]]) {
			blue.element -= iniStrength[blue.seqCreateW[lastbornBlue % 5]];
			blue.numW[blue.seqCreateW[lastbornBlue % 5]]++;
			switch (blue.seqCreateW[lastbornBlue % 5])
			{
			case 0:blue.cwarrior.push_back(CDragon(lastbornBlue + 1, blue.element)); break;
			case 1:blue.cwarrior.push_back(CNinja(lastbornBlue + 1)); break;
			case 2:blue.cwarrior.push_back(CIceman(lastbornBlue + 1)); break;
			case 3:blue.cwarrior.push_back(CLion(lastbornBlue + 1, blue.element)); break;
			case 4:blue.cwarrior.push_back(CWolf(lastbornBlue + 1)); break;
			}
			cities[numCity + 1].pBlue = new CWarrior;
			*(cities[numCity + 1].pBlue) = blue.cwarrior[blue.cwarrior.size() - 1];
			printtime(hour);
			cout << ":00 blue " << nameW[blue.seqCreateW[lastbornBlue % 5]] << " " << lastbornBlue + 1 << " born" << endl;
			if (blue.seqCreateW[lastbornBlue % 5] == 3) {
				cout << "Its loyalty is " << blue.cwarrior[blue.cwarrior.size() - 1].loyalty << endl;
			}
			if (blue.seqCreateW[lastbornBlue % 5] == 0) {
				cout << "Its morale is " << setprecision(2) << fixed << blue.cwarrior[blue.cwarrior.size() - 1].morale << endl;
			}
			lastbornBlue++;
		}

		//lion逃跑
		if (hour == hourEnd&&minuteEnd<5)return;
		for (int i = 0; i <= numCity + 1; i++) {
			if (i != numCity + 1 && cities[i].pRed != NULL&&cities[i].pRed->type == 3 && cities[i].pRed->loyalty <= 0) {
				printtime(hour);
				cout << ":05 red lion " << cities[i].pRed->Nodot << " ran away" << endl;
				delete cities[i].pRed;
				cities[i].pRed = NULL;
			}

			if (i != 0 && cities[i].pBlue != NULL&&cities[i].pBlue->type == 3 && cities[i].pBlue->loyalty <= 0) {
				printtime(hour);
				cout << ":05 blue lion " << cities[i].pBlue->Nodot << " ran away" << endl;
				delete cities[i].pBlue;
				cities[i].pBlue = NULL;
			}

		}
		//武士前进到某一城市
		if (hour == hourEnd&&minuteEnd<10)return;
		int flag = 0;
		for (int i = 0; i <= numCity + 1; i++) { //iceman融化
			if (cities[i].pRed != NULL&&cities[i].pRed->type == 2) {
				cities[i].pRed->step++;
				if (cities[i].pRed->step == 2) {
					cities[i].pRed->strength -= 9;
					if (cities[i].pRed->strength <= 0)
						cities[i].pRed->strength = 1;
					cities[i].pRed->force += 20;
					cities[i].pRed->step = 0;
				}
			}
			if (cities[i].pBlue != NULL&&cities[i].pBlue->type == 2) {
				cities[i].pBlue->step++;
				if (cities[i].pBlue->step == 2) {
					cities[i].pBlue->strength -= 9;
					if (cities[i].pBlue->strength <= 0)
						cities[i].pBlue->strength = 1;
					cities[i].pBlue->force += 20;
					cities[i].pBlue->step = 0;
				}
			}
		}
		if (cities[numCity + 1].pRed != NULL) {
            WaitingBlueHeadquarter=new CWarrior;
			*WaitingBlueHeadquarter = *(cities[numCity + 1].pRed);
		}
		for (int i = (hour + 1>numCity ? numCity + 1 : hour + 1); i>0; --i) {
			if (cities[i - 1].pRed != NULL) {
				if (cities[i].pRed != NULL) {
					delete cities[i].pRed;
				}
				cities[i].pRed = new CWarrior;
				*(cities[i].pRed) = *(cities[i - 1].pRed);
				delete cities[i - 1].pRed;
				cities[i - 1].pRed = NULL;
			}
			else {
				if (cities[i].pRed != NULL) {
					delete cities[i].pRed;
					cities[i].pRed = NULL;
				}
			}
		}
		if (cities[0].pRed != NULL) {
			delete cities[0].pRed;
			cities[0].pRed = NULL;
		}
		if (cities[0].pBlue != NULL) {
		    WaitingRedHeadquarter=new CWarrior;
			*WaitingRedHeadquarter = *(cities[0].pBlue);
		}
		for (int i = (hour + 1>numCity ? 0 : (numCity - hour)); i<numCity + 1; ++i) {
			if (cities[i + 1].pBlue != NULL) {
				if (cities[i].pBlue != NULL) {
					delete cities[i].pBlue;
				}
				cities[i].pBlue = new CWarrior;
				*(cities[i].pBlue) = *(cities[i + 1].pBlue);
				delete cities[i + 1].pBlue;
				cities[i + 1].pBlue = NULL;
			}
			else {
				if (cities[i].pBlue != NULL) {
					delete cities[i].pBlue;
					cities[i].pBlue = NULL;
				}
			}
		}
		if (cities[numCity + 1].pBlue != NULL) {
			delete cities[numCity + 1].pBlue;
			cities[numCity + 1].pBlue = NULL;
		}

		if (cities[0].pBlue != NULL) {
			printtime(hour);
			cout << ":10 blue " << nameW[cities[0].pBlue->type] << " " << cities[0].pBlue->Nodot << " reached red headquarter with " << cities[0].pBlue->strength << " elements and force " << cities[0].pBlue->force << endl;
			taken[RED]++;
			if (taken[RED] == 2) {
				printtime(hour);
				cout << ":10 red headquarter was taken" << endl;
				flag = 1;
			}
		}
		for (int i = 1; i <= numCity; i++) {
			if (cities[i].pRed != NULL) {
				printtime(hour);
				cout << ":10 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " marched to city " << i << " with " << cities[i].pRed->strength << " elements and force " << cities[i].pRed->force << endl;
			}
			if (cities[i].pBlue != NULL) {
				printtime(hour);
				cout << ":10 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " marched to city " << i << " with " << cities[i].pBlue->strength << " elements and force " << cities[i].pBlue->force << endl;
			}
		}
		if (cities[numCity + 1].pRed != NULL) {
			printtime(hour);
			cout << ":10 red " << nameW[cities[numCity + 1].pRed->type] << " " << cities[numCity + 1].pRed->Nodot << " reached blue headquarter with " << cities[numCity + 1].pRed->strength << " elements and force " << cities[numCity + 1].pRed->force << endl;
			taken[BLUE]++;
			if (taken[BLUE] == 2) {
				printtime(hour);
				cout << ":10 blue headquarter was taken" << endl;
				flag = 1;
			}
		}
		if (flag == 1)return;

		//20分 城市产出生命元
		if (hour == hourEnd&&minuteEnd<20)return;
		for (int i = 1; i <= numCity; i++) {
			cities[i].cityElement += 10;
		}
		//30分 司令部（通过武士）生命元++
		if (hour == hourEnd&&minuteEnd<30)return;
		for (int i = 1; i <= numCity; i++) {
			if (cities[i].pRed != NULL&&cities[i].pBlue == NULL) {
				red.element += cities[i].cityElement;
				printtime(hour); cout << ":30 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " earned " << cities[i].cityElement << " elements for his headquarter" << endl;
				cities[i].cityElement = 0;
			}
			else if (cities[i].pBlue != NULL&&cities[i].pRed == NULL) {
				blue.element += cities[i].cityElement;
				printtime(hour); cout << ":30 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " earned " << cities[i].cityElement << " elements for his headquarter" << endl;
				cities[i].cityElement = 0;
			}
		}

		//武士放箭
		if (hour == hourEnd&&minuteEnd<35)return;
		int recArrow[2][22] = { 0 };//0 red 1 blue

		for (int i = 0; i <= numCity + 1; i++) {
			if (i != numCity + 1 && cities[i].pRed != NULL&& cities[i].pRed->hisArrow.own == 1 && cities[i + 1].pBlue != NULL) {
				cities[i + 1].pBlue->strength -= R;
				cities[i].pRed->hisArrow.used++;
				if (cities[i].pRed->hisArrow.used == 3) {
					cities[i].pRed->hisArrow.own = 0;
				}
				printtime(hour);
				cout << ":35 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " shot";
				if (cities[i + 1].pBlue->strength <= 0) {
					recArrow[1][i + 1] = 1;
					cout << " and killed blue " << nameW[cities[i + 1].pBlue->type] << " " << cities[i + 1].pBlue->Nodot;
				}
				cout << endl;
			}
			if (i != 0 && cities[i].pBlue != NULL&&cities[i].pBlue->hisArrow.own == 1 && cities[i - 1].pRed != NULL) {
				cities[i - 1].pRed->strength -= R;
				cities[i].pBlue->hisArrow.used++;
				if (cities[i].pBlue->hisArrow.used == 3) {
					cities[i].pBlue->hisArrow.own = 0;
				}
				printtime(hour);
				cout << ":35 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " shot";
				if (cities[i - 1].pRed->strength <= 0) {
					recArrow[0][i - 1] = 1;
					cout << " and killed red " << nameW[cities[i - 1].pRed->type] << " " << cities[i - 1].pRed->Nodot;
				}
				cout << endl;
			}
		}
		for (int i = 0; i <= numCity + 1; i++) {//有对手的被射死的没有被delete
		    if(recArrow[RED][i]==1&&recArrow[BLUE][i]==1){
                delete cities[i].pRed;delete cities[i].pBlue;
                cities[i].pRed=NULL;cities[i].pBlue=NULL;
                recArrow[RED][i]=0;recArrow[BLUE][i]=0;
		    }
			if (recArrow[RED][i] == 1 && cities[i].pBlue == NULL) {
				delete cities[i].pRed;
				cities[i].pRed = NULL;
				recArrow[RED][i] = 0;
			}
			if (recArrow[BLUE][i] == 1 && cities[i].pRed == NULL) {
				delete cities[i].pBlue;
				cities[i].pBlue = NULL;
				recArrow[BLUE][i] = 0;
			}
		}

		//武士使用bomb
		if (hour == hourEnd&&minuteEnd<38)return;
		for (int i = 1; i<numCity + 1; i++) {

			if (cities[i].pBlue != NULL&&cities[i].pRed != NULL&&cities[i].pBlue->strength>0 && cities[i].pRed->strength>0 && (cities[i].pRed->hisBomb.own == 1 || cities[i].pBlue->hisBomb.own == 1)) {
				CWarrior * tmpRed = NULL, *tmpBlue = NULL;
				tmpBlue = new CWarrior; tmpRed = new CWarrior;
				*(tmpRed) = *(cities[i].pRed); *(tmpBlue) = *(cities[i].pBlue);
				bool redDead = false, blueDead = false;
				if ((cities[i].cityflag == RED) || (cities[i].cityflag != BLUE&&i % 2 == 1)) {
					fight(tmpRed, tmpBlue);
					if (tmpBlue->strength <= 0) {
						blueDead = true;
					}
					if (tmpRed->strength <= 0) {
						redDead = true;
					}
				}
				else if ((cities[i].cityflag == BLUE) || (cities[i].cityflag != RED&&i % 2 == 0)) {
					fight(tmpBlue, tmpRed);
					if (tmpBlue->strength <= 0) {
						blueDead = true;
					}
					if (tmpRed->strength <= 0) {
						redDead = true;
					}
				}
				if (cities[i].pRed->hisBomb.own == 1 && redDead == true) {
					printtime(hour);
					cout << ":38 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " used a bomb and killed blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << endl;
					delete cities[i].pRed; cities[i].pRed = NULL;
					delete cities[i].pBlue; cities[i].pBlue = NULL;
				}
				else if (cities[i].pBlue->hisBomb.own == 1 && blueDead == true) {
					printtime(hour);
					cout << ":38 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " used a bomb and killed red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << endl;
					delete cities[i].pRed; cities[i].pRed = NULL;
					delete cities[i].pBlue; cities[i].pBlue = NULL;
				}
			}
		}
		//报告战斗情况
		if (hour == hourEnd&&minuteEnd<40)return;
		int recAward[2] = { 0 };
		int recVictory[2][22] = { 0 };
		for (int i = 1; i <= numCity; i++) {

			if (cities[i].pBlue != NULL&&cities[i].pRed != NULL) {//有一蓝一红
				if ((cities[i].cityflag == RED) || (cities[i].cityflag != BLUE&&i % 2 == 1)) {
					if (cities[i].pBlue->strength > 0 && cities[i].pRed->strength > 0) {
						int beforeRed = cities[i].pRed->strength, beforeBlue = cities[i].pBlue->strength;
						printtime(hour);
						cout << ":40 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " attacked blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " in city " << i << " with " << cities[i].pRed->strength << " elements and force " << cities[i].pRed->force << endl;
						if (fight(cities[i].pRed, cities[i].pBlue)) {
							printtime(hour);
							cout << ":40 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " fought back against red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " in city " << i << endl;
						}

						//lion loyalty
						if (cities[i].pBlue->type == 3) {
							if (cities[i].pRed->strength > 0) {
								cities[i].pBlue->loyalty -= kLion;
							}
						}
						if (cities[i].pRed->type == 3) {
							if (cities[i].pBlue->strength > 0) {
								cities[i].pRed->loyalty -= kLion;
							}
						}
						//
						if (cities[i].pBlue->strength <= 0) {
							if (cities[i].pBlue->type == 3) {
								cities[i].pRed->strength += beforeBlue;
							}
							printtime(hour);
							cout << ":40 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " was killed in city " << i << endl;
						}
						if (cities[i].pRed->strength <= 0) {
							if (cities[i].pRed->type == 3) {
								cities[i].pBlue->strength += beforeRed;
							}
							printtime(hour);
							cout << ":40 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " was killed in city " << i << endl;
						}

					}
					//dragon morale
					if (cities[i].pBlue->type == 0) {
						if (cities[i].pRed->strength <= 0) {
							cities[i].pBlue->morale += 0.2;
						}
						else
							cities[i].pBlue->morale -= 0.2;
					}
					if (cities[i].pRed->type == 0) {
						if (cities[i].pBlue->strength <= 0) {
							cities[i].pRed->morale += 0.2;
						}
						else
							cities[i].pRed->morale -= 0.2;
					}
					if (cities[i].pRed->type == 0 && cities[i].pRed->strength > 0 && cities[i].pRed->morale > 0.8) {
						printtime(hour); cout << ":40 red dragon " << cities[i].pRed->Nodot << " yelled in city " << i << endl;
					}
					//wolf缴获武器
					if (cities[i].pBlue->type == 4) {
						if (cities[i].pRed->strength < 0) {
							if (cities[i].pBlue->hisSword.own == 0 && cities[i].pRed->hisSword.own == 1) {
								cities[i].pBlue->hisSword.own = 1;
								cities[i].pBlue->hisSword.WeaponForce = cities[i].pRed->hisSword.WeaponForce;
							}
							if (cities[i].pBlue->hisBomb.own == 0 && cities[i].pRed->hisBomb.own == 1) {
								cities[i].pBlue->hisBomb.own = 1;
							}
							if (cities[i].pBlue->hisArrow.own == 0 && cities[i].pRed->hisArrow.own == 1) {
								cities[i].pBlue->hisArrow.own = 1;
								cities[i].pBlue->hisArrow.used = cities[i].pRed->hisArrow.used;
							}
						}
					}
					if (cities[i].pRed->type == 4) {
						if (cities[i].pBlue->strength < 0) {
							if (cities[i].pRed->hisSword.own == 0 && cities[i].pBlue->hisSword.own == 1) {
								cities[i].pRed->hisSword.own = 1;
								cities[i].pRed->hisSword.WeaponForce = cities[i].pBlue->hisSword.WeaponForce;
							}
							if (cities[i].pRed->hisBomb.own == 0 && cities[i].pBlue->hisBomb.own == 1) {
								cities[i].pRed->hisBomb.own = 1;
							}
							if (cities[i].pRed->hisArrow.own == 0 && cities[i].pBlue->hisArrow.own == 1) {
								cities[i].pRed->hisArrow.own = 1;
								cities[i].pRed->hisArrow.used = cities[i].pBlue->hisArrow.used;
							}
						}
					}
					if (cities[i].pBlue->strength>0 || cities[i].pRed->strength>0) {
						if (cities[i].pRed->strength <= 0) {
							printtime(hour);
							cout << ":40 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " earned " << cities[i].cityElement << " elements for his headquarter" << endl;
							recAward[BLUE] += cities[i].cityElement;
							cities[i].cityElement = 0;
							recFlag[BLUE][i]++;
							recVictory[BLUE][i] = 1;
							if (recFlag[BLUE][i] == 2&&cities[i].cityflag!=BLUE) {
								cities[i].cityflag = BLUE;
								recFlag[BLUE][i] = 0;
								printtime(hour); cout << ":40 blue flag raised in city " << i << endl;
							}
							delete cities[i].pRed;
							cities[i].pRed = NULL;
						}
						else
							recFlag[BLUE][i] = 0;
						if (cities[i].pBlue->strength <= 0) {
							printtime(hour);
							cout << ":40 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " earned " << cities[i].cityElement << " elements for his headquarter" << endl;
							recAward[RED] += cities[i].cityElement;
							cities[i].cityElement = 0;
							recFlag[RED][i]++;
							recVictory[RED][i] = 1;
							if (recFlag[RED][i] == 2&&cities[i].cityflag!=RED) {
								cities[i].cityflag = RED;
								recFlag[RED][i] = 0;
								printtime(hour); cout << ":40 red flag raised in city " << i << endl;
							}
							delete cities[i].pBlue;
							cities[i].pBlue = NULL;
						}
						else
							recFlag[RED][i] = 0;
					}

				}
				else if ((cities[i].cityflag == BLUE) || (cities[i].cityflag != RED&&i % 2 == 0)) {
					if (cities[i].pBlue->strength > 0 && cities[i].pRed->strength > 0) {
						int beforeRed = cities[i].pRed->strength, beforeBlue = cities[i].pBlue->strength;
						printtime(hour);
						cout << ":40 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " attacked red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " in city " << i << " with " << cities[i].pBlue->strength << " elements and force " << cities[i].pBlue->force << endl;
						if (fight(cities[i].pBlue, cities[i].pRed)) {
							printtime(hour);
							cout << ":40 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " fought back against blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " in city " << i << endl;
						}

						//lion loyalty
						if (cities[i].pBlue->type == 3) {
							if (cities[i].pRed->strength > 0) {
								cities[i].pBlue->loyalty -= kLion;
							}
						}
						if (cities[i].pRed->type == 3) {
							if (cities[i].pBlue->strength > 0) {
								cities[i].pRed->loyalty -= kLion;
							}
						}
						//
						if (cities[i].pBlue->strength <= 0) {
							if (cities[i].pBlue->type == 3) {
								cities[i].pRed->strength += beforeBlue;
							}
							printtime(hour);
							cout << ":40 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " was killed in city " << i << endl;
						}
						if (cities[i].pRed->strength <= 0) {
							if (cities[i].pRed->type == 3) {
								cities[i].pBlue->strength += beforeRed;
							}
							printtime(hour);
							cout << ":40 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " was killed in city " << i << endl; NULL;
						}

					}
					//dragon morale
					if (cities[i].pBlue->type == 0) {
						if (cities[i].pRed->strength <= 0) {
							cities[i].pBlue->morale += 0.2;
						}
						else
							cities[i].pBlue->morale -= 0.2;
					}
					if (cities[i].pRed->type == 0) {
						if (cities[i].pBlue->strength <= 0) {
							cities[i].pRed->morale += 0.2;
						}
						else
							cities[i].pRed->morale -= 0.2;
					}
					if (cities[i].pBlue->type == 0 && cities[i].pBlue->strength > 0 && cities[i].pBlue->morale > 0.8) {
						printtime(hour); cout << ":40 blue dragon " << cities[i].pBlue->Nodot << " yelled in city " << i << endl;
					}
					//wolf缴获武器
					if (cities[i].pBlue->type == 4) {
						if (cities[i].pRed->strength < 0) {
							if (cities[i].pBlue->hisSword.own == 0 && cities[i].pRed->hisSword.own == 1) {
								cities[i].pBlue->hisSword.own = 1;
								cities[i].pBlue->hisSword.WeaponForce = cities[i].pRed->hisSword.WeaponForce;
							}
							if (cities[i].pBlue->hisBomb.own == 0 && cities[i].pRed->hisBomb.own == 1) {
								cities[i].pBlue->hisBomb.own = 1;
							}
							if (cities[i].pBlue->hisArrow.own == 0 && cities[i].pRed->hisArrow.own == 1) {
								cities[i].pBlue->hisArrow.own = 1;
								cities[i].pBlue->hisArrow.used = cities[i].pRed->hisArrow.used;
							}
						}
					}
					if (cities[i].pRed->type == 4) {
						if (cities[i].pBlue->strength < 0) {
							if (cities[i].pRed->hisSword.own == 0 && cities[i].pBlue->hisSword.own == 1) {
								cities[i].pRed->hisSword.own = 1;
								cities[i].pRed->hisSword.WeaponForce = cities[i].pBlue->hisSword.WeaponForce;
							}
							if (cities[i].pRed->hisBomb.own == 0 && cities[i].pBlue->hisBomb.own == 1) {
								cities[i].pRed->hisBomb.own = 1;
							}
							if (cities[i].pRed->hisArrow.own == 0 && cities[i].pBlue->hisArrow.own == 1) {
								cities[i].pRed->hisArrow.own = 1;
								cities[i].pRed->hisArrow.used = cities[i].pBlue->hisArrow.used;
							}
						}
					}
					if (cities[i].pBlue->strength>0 || cities[i].pRed->strength>0) {
						if (cities[i].pRed->strength <= 0) {
							delete cities[i].pRed;
							cities[i].pRed = NULL;
							printtime(hour);
							cout << ":40 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " earned " << cities[i].cityElement << " elements for his headquarter" << endl;
							recAward[BLUE] += cities[i].cityElement;
							cities[i].cityElement = 0;
							recFlag[BLUE][i]++;
							recVictory[BLUE][i] = 1;
							if (recFlag[BLUE][i] == 2&&cities[i].cityflag!=BLUE) {
								cities[i].cityflag = BLUE;
								recFlag[BLUE][i] = 0;
								printtime(hour); cout << ":40 blue flag raised in city " << i << endl;
							}
						}
						else
							recFlag[BLUE][i] = 0;
						if (cities[i].pBlue->strength <= 0) {
							delete cities[i].pBlue;
							cities[i].pBlue = NULL;
							printtime(hour);
							cout << ":40 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " earned " << cities[i].cityElement << " elements for his headquarter" << endl;
							recAward[RED] += cities[i].cityElement;
							cities[i].cityElement = 0;
							recFlag[RED][i]++;
							recVictory[RED][i] = 1;
							if (recFlag[RED][i] == 2&&cities[i].cityflag!=RED) {
								cities[i].cityflag = RED;
								recFlag[RED][i] = 0;
								printtime(hour); cout << ":40 red flag raised in city " << i << endl;
							}
						}
						else
							recFlag[RED][i] = 0;
					}


				}
			}
		}
		for (int i = numCity; i >= 1; --i) {
			if (recVictory[RED][i] == 1 && red.element >= 8) {
				cities[i].pRed->strength += 8;
				red.element -= 8;
			}
			else if (red.element<8)
				break;
		}
		for (int i = 1; i <= numCity; ++i) {
			if (recVictory[BLUE][i] == 1 && blue.element >= 8) {
				cities[i].pBlue->strength += 8;
				blue.element -= 8;
			}
			else if (blue.element<8)
				break;
		}
		red.element += recAward[RED];
		blue.element += recAward[BLUE];

		//司令部报告生命元数量
		if (hour == hourEnd&&minuteEnd<50)return;
		printtime(hour);
		cout << ":50 " << red.element << " elements in red headquarter" << endl;
		printtime(hour);
		cout << ":50 " << blue.element << " elements in blue headquarter" << endl;
		//武士报告情况
		if (hour == hourEnd&&minuteEnd<55)return;
		for (int i = 0; i <= numCity + 1; i++) {
			if (cities[i].pRed != NULL) {
				int flag = 0;
				printtime(hour);
				cout << ":55 red " << nameW[cities[i].pRed->type] << " " << cities[i].pRed->Nodot << " has ";
				if (cities[i].pRed->hisArrow.own == 1) {
					cout << "arrow(" << 3 - cities[i].pRed->hisArrow.used << ")";
					flag = 1;
				}
				if (cities[i].pRed->hisBomb.own == 1) {
					if (flag == 1) {
						cout << ",";
					}
					cout << "bomb";
					flag = 1;
				}
				if (cities[i].pRed->hisSword.own == 1&&cities[i].pRed->hisSword.WeaponForce>0) {
					if (flag == 1) {
						cout << ",";
					}
					cout << "sword(" << cities[i].pRed->hisSword.WeaponForce << ")";
					flag = 1;
				}
				if (flag == 0) {
					cout << "no weapon" << endl;
				}
				else
					cout << endl;
			}
		}
		if (WaitingBlueHeadquarter != NULL) {
			int flag = 0;
			printtime(hour);
			cout << ":55 red " << nameW[WaitingBlueHeadquarter->type] << " " << WaitingBlueHeadquarter->Nodot << " has ";
			if (WaitingBlueHeadquarter->hisArrow.own == 1) {
				cout << "arrow(" << 3 - WaitingBlueHeadquarter->hisArrow.used << ")";
				flag = 1;
			}
			if (WaitingBlueHeadquarter->hisBomb.own == 1) {
				if (flag == 1) {
					cout << ",";
				}
				cout << "bomb";
				flag = 1;
			}
			if (WaitingBlueHeadquarter->hisSword.own == 1&&WaitingBlueHeadquarter->hisSword.WeaponForce>0) {
				if (flag == 1) {
					cout << ",";
				}
				cout << "sword(" << WaitingBlueHeadquarter->hisSword.WeaponForce << ")";
				flag = 1;
			}
			if (flag == 0) {
				cout << "no weapon" << endl;
			}
			else
				cout << endl;
		}
		if (WaitingRedHeadquarter != NULL) {
			int flag = 0;
			printtime(hour);
			cout << ":55 blue " << nameW[WaitingRedHeadquarter->type] << " " << WaitingRedHeadquarter->Nodot << " has ";
			if (WaitingRedHeadquarter->hisArrow.own == 1) {
				cout << "arrow(" << 3 - WaitingRedHeadquarter->hisArrow.used << ")";
				flag = 1;
			}
			if (WaitingRedHeadquarter->hisBomb.own == 1) {
				if (flag == 1) {
					cout << ",";
				}
				cout << "bomb";
				flag = 1;
			}
			if (WaitingRedHeadquarter->hisSword.own == 1&&WaitingRedHeadquarter->hisSword.WeaponForce>0) {
				if (flag == 1) {
					cout << ",";
				}
				cout << "sword(" << WaitingRedHeadquarter->hisSword.WeaponForce << ")";
				flag = 1;
			}
			if (flag == 0) {
				cout << "no weapon" << endl;
			}
			else
				cout << endl;
		}
		for (int i = 0; i <= numCity + 1; i++) {
			if (cities[i].pBlue != NULL) {
				int flag = 0;
				printtime(hour);
				cout << ":55 blue " << nameW[cities[i].pBlue->type] << " " << cities[i].pBlue->Nodot << " has ";
				if (cities[i].pBlue->hisArrow.own == 1) {
					cout << "arrow(" << 3 - cities[i].pBlue->hisArrow.used << ")";
					flag = 1;
				}
				if (cities[i].pBlue->hisBomb.own == 1) {
					if (flag == 1) {
						cout << ",";
					}
					cout << "bomb";
					flag = 1;
				}
				if (cities[i].pBlue->hisSword.own == 1&&cities[i].pBlue->hisSword.WeaponForce>0) {
					if (flag == 1) {
						cout << ",";
					}
					cout << "sword(" << cities[i].pBlue->hisSword.WeaponForce << ")";
					flag = 1;
				}
				if (flag == 0) {
					cout << "no weapon" << endl;
				}
				else
					cout << endl;
			}
		}



	}
}
int main()
{
//	freopen("war.txt", "w", stdout);
	int cases;
	cin >> cases;
	for (int g = 0; g<cases; g++) {
		lastbornBlue = 0; lastbornRed = 0;
		memset(recFlag, 0, sizeof(recFlag));
		memset(taken, 0, sizeof(taken));
		delete WaitingBlueHeadquarter; WaitingBlueHeadquarter = NULL;
		delete WaitingRedHeadquarter; WaitingRedHeadquarter = NULL;
		cin >> iniElement >> numCity >> R >> kLion >> timeEnd;
		hourEnd = timeEnd / 60;
		minuteEnd = timeEnd % 60;
		cin >> iniStrength[0] >> iniStrength[1] >> iniStrength[2] >> iniStrength[3] >> iniStrength[4];
		cin >> iniForce[0] >> iniForce[1] >> iniForce[2] >> iniForce[3] >> iniForce[4];
		for (int h = 0; h <= 21; h++) {
			cities[h].clearr();
		}
		Headquarter red(iniElement, RED), blue(iniElement, BLUE);
		cout << "Case " << g + 1 << ":" << endl;
		Output(red, blue);
	}

	return 0;
}
