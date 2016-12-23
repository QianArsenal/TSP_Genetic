// TSP_Genetic.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
using namespace std;

#define CITY_NUM	10
#define GENETIC_NUM		100
#define RANDOM_NUM	123456789
#define MAX_ROUND	100
#define INHERIT_NUM		30

int dis[CITY_NUM][CITY_NUM];
int x[CITY_NUM];
int y[CITY_NUM];
struct city{
	int city_id;
	city* next_city;
};

struct city_group{
	struct city* c_header;
	int sum_dis;
	bool operator < (const city_group& c_g) const
	{
		return sum_dis < c_g.sum_dis;
	}
	void destroy()
	{
		struct city *h = c_header;
		struct city *n;
		for (int i = 0; i < CITY_NUM; i++){
			n = h->next_city;
			free(h);
			h = n;
		}
	}
};

int generate_random_num(int* r_num, int c_n)
{
	int n = (int)pow(10 ,rand() % c_n);
	int res = (*r_num / n) % 10;
	*r_num = *r_num % n + (*r_num / (n * 10)) * n;
	return res;
}

vector<city_group> city_genetics;

city* generate_city_group()
{
	city *header, *last;
	int ran_num = RANDOM_NUM;
	header = (city*)malloc(sizeof(city));
	last = header;
	header->city_id = generate_random_num(&ran_num, CITY_NUM);
	for (int i = 1; i < CITY_NUM; i++){
		last->next_city = (city*)malloc(sizeof(city));
		last = last->next_city;
		last->city_id = generate_random_num(&ran_num, CITY_NUM - i);
	}
	last->next_city = header;
	return header;
}

int cal_sum_dis(city *header)
{
	int sum = 0;
	city *c = header;
	while (c->next_city != header){
		sum += dis[c->city_id][c->next_city->city_id];
		c = c->next_city;
	}
	return sum;
}

void generate_genetics()
{
	city_genetics.clear();
	for (int i = 0; i < GENETIC_NUM; i++){
		city_group s_g;
		s_g.c_header = generate_city_group();
		s_g.sum_dis = cal_sum_dis(s_g.c_header);
		city_genetics.push_back(s_g);
	}
}
void show_city_group(city_group *cg)
{
	city* header = cg->c_header;
	city* now = header->next_city;
	cout << setw(3) << header->city_id;
	while (now != header){
		cout << setw(3) << now->city_id;
		now = now->next_city;
	}
	cout << setw(7) << cg->sum_dis << endl;
}
void change_city_group(city_group *cg, city *last, int c_id)
{
	if (last->next_city->city_id == c_id){
		return;
	}
	city *city_this = cg->c_header;
	city *city_last = last->next_city;
	while (city_this->next_city->city_id != c_id){
		city_this = city_this->next_city;
	}
	last->next_city = city_this->next_city;
	city_this->next_city = cg->c_header;
	city_this = last->next_city;
	while (city_this->next_city != cg->c_header){
		city_this = city_this->next_city;
	}
	city_this->next_city = city_last;
	return;
}
void city_group_cross(city_group *fath, city_group *math, city_group *res)
{
	city *fath_this = fath->c_header;
	city *math_this = math->c_header;
	city *fath_next = fath_this->next_city;
	city *math_next = math_this->next_city;
	city *res_this = res->c_header;
	/*if (fath->sum_dis < math->sum_dis){
		res->c_header->city_id = fath->c_header->city_id;
		while (math_this->city_id != fath_this->city_id){
			math_this = math_this->next_city;
		}
		math->c_header = math_this;
		math_next = math_this->next_city;
	}
	else{
		res->c_header->city_id = math->c_header->city_id;
		while (fath_this->city_id != math_this->city_id){
			fath_this = fath_this->next_city;
		}
		fath->c_header = fath_this;
		fath_next = fath_this->next_city;
	}*/
	res->c_header->city_id = fath->c_header->city_id;
	while (math_this->city_id != res->c_header->city_id){
		math_this = math_this->next_city;
	}
	math->c_header = math_this;
	math_next = math_this->next_city;
	res_this = res_this->next_city;
	for (int i = 1; i < CITY_NUM; i++){
		if (dis[fath_this->city_id][fath_next->city_id] < dis[math_this->city_id][math_next->city_id]){
			res_this->city_id = fath_next->city_id;
			change_city_group(math, math_this, fath_next->city_id);
		}
		else if (dis[fath_this->city_id][fath_next->city_id] > dis[math_this->city_id][math_next->city_id]){
			res_this->city_id = math_next->city_id;
			change_city_group(fath, fath_this, math_next->city_id);
		}
		else{
			res_this->city_id = math_next->city_id;
		}
		res_this = res_this->next_city;
		fath_this = fath_this->next_city;
		fath_next = fath_this->next_city;
		math_this = math_this->next_city;
		math_next = math_this->next_city;
	}
}

void genetics_cross(city_group *buf_1, city_group *buf_2)
{
	int i, j;
	for (int k = 0; k < GENETIC_NUM - INHERIT_NUM; k++){
		i = rand() % INHERIT_NUM;
		j = rand() % INHERIT_NUM;
		city_group fath = city_genetics[i];
		city_group math = city_genetics[j];
		city *buf1_this = buf_1->c_header;
		city *buf2_this = buf_2->c_header;
		city *fath_this = fath.c_header;
		city *math_this = math.c_header;
		for (int i = 0; i < CITY_NUM; i++){
			buf1_this->city_id = fath_this->city_id;
			buf2_this->city_id = math_this->city_id;
			buf1_this = buf1_this->next_city;
			buf2_this = buf2_this->next_city;
			math_this = math_this->next_city;
			fath_this = fath_this->next_city;
		}
		city_group *res = &city_genetics[INHERIT_NUM + k];
		city_group_cross(buf_1, buf_2, res);
		res->sum_dis = cal_sum_dis(res->c_header);
	}
}

void genetics_change()
{
	for (int i = 0; i < GENETIC_NUM; i++){
		if (rand() % CITY_NUM < 2){
			int i = rand() % CITY_NUM;
			int j = rand() % CITY_NUM;
			city_group *res = &city_genetics[i];
			city *h_i = res->c_header;
			city *h_j = res->c_header;
			while (i--){
				h_i = h_i->next_city;
			}
			while (j--){
				h_j = h_j->next_city;
			}
			int t = h_i->city_id;
			h_i->city_id = h_j->city_id;
			h_j->city_id = t;
			res->sum_dis = cal_sum_dis(res->c_header);
		}
	}
}

void show_res()
{
	for (int i = 0; i < city_genetics.size(); i++){
		city* header = city_genetics[i].c_header;
		city* now = header->next_city;
		cout << setw(3) << header->city_id;
		while (now != header){
			cout << setw(3) << now->city_id;
			now = now->next_city;
		}
		cout << setw(7) << city_genetics[i].sum_dis << endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned int)time(NULL));
	ifstream dis_file("city_dis.txt");
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			dis_file >> setw(7) >> dis[i][j];
			cout << setw(5) << dis[i][j];
		}
		cout << endl;
	}
	dis_file.close();
	city_group fath_buf, math_buf;
	fath_buf.c_header = generate_city_group();
	fath_buf.sum_dis = cal_sum_dis(fath_buf.c_header);
	math_buf.c_header = generate_city_group();
	math_buf.sum_dis = cal_sum_dis(math_buf.c_header);
	generate_genetics();
	int round = MAX_ROUND;
	while (round--){
		sort(city_genetics.begin(), city_genetics.end(), less<city_group>());
		show_city_group(&city_genetics[0]);
		//show_res();
		genetics_cross(&fath_buf, &math_buf);
		genetics_change();
	}
	sort(city_genetics.begin(), city_genetics.end(), less<city_group>());
	show_res();
	ofstream roads_file("city_roads.txt");
	city_group res = city_genetics[0];
	city *h = res.c_header;
	for (int i = 0; i < CITY_NUM; i++){
		roads_file << h->city_id << " ";
		h = h->next_city;
	}
	math_buf.destroy();
	fath_buf.destroy();
	for (int i = 0; i < city_genetics.size();)
	{
		city_group c = city_genetics[city_genetics.size()-1];
		city_genetics.pop_back();
		c.destroy();
	}
	return 0;
}

