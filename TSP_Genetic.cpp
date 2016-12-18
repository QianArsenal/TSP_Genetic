// TSP_Genetic.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
using namespace std;

#define CITY_NUM	10
#define GENETIC_NUM		100
#define RANDOM_NUM	123456789

struct city{
	int city_id;
	city* next_city;
};

struct city_group{
	struct city* c_header;
	int sum_dis;
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
	city *header, *now, *last;
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

/*
void generate_genetics()
{
	city_genetics.clear();
	for (int i = 0; i < GENETIC_NUM; i++){
		CITYS_HEAD header = 
	}
}*/

int _tmain(int argc, _TCHAR* argv[])
{
	srand((unsigned int)time(NULL));
	city *c = generate_city_group();
	city *next = c->next_city;
	cout << c->city_id << " " ;
	while (next != c){
		cout << next->city_id << " ";
		next = next->next_city;
	}
	return 0;
}

