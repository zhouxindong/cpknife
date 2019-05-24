#pragma once

#include <array>
#include <set>
#include <list>
#include <deque>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <climits>
#include <forward_list>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

void FillInBuildArray(int ar[], int size)
{
	for (int i = 0; i < size; i++)
	{
		ar[i] = i + 1;
	}
}

template<size_t N>
void FillArray(array<int, N>& ar)
{
	for (int i = 0; i < ar.size(); i++)
	{
		ar[i] = i + 1;
	}
}

void FillVector(vector<int>& vec, int size)
{
	vec.clear();
	for (int i = 0; i < size; i++)
	{
		vec.push_back(i + 1);
	}
}

void FillDeque(deque<int>& deq, int size)
{
	deq.clear();
	for (int i = 0; i < size; i++)
	{
		deq.push_back(i + 1);
	}
}

void FillList(list<int>& li, int size)
{
	li.clear();
	for (int i = 0; i < size; i++)
	{
		li.push_back(i + 1);
	}
}

void FillForwardList(forward_list<int>& foli, int size)
{
	foli.clear();
	for (int i = 0; i < size; i++)
	{
		foli.push_front(i + 1);
	}
}

void FillSet(set<int>& se, int size)
{
	se.clear();
	for (int i = 0; i < size; i++)
	{
		se.insert(i + 1);
	}
}

void FillMap(map<int, string>& mp, int size)
{
	mp.clear();
	char buf[255];
	for (int i = 0; i < size; i++)
	{
		sprintf(buf, "#%d", i + 1);
		mp[i + 1] = string(buf);
	}
}

void FillUnorderedSet(unordered_set<int>& coll, int size)
{
	coll.clear();
	for (int i = 0; i < size; i++)
	{
		coll.insert(i + 1);
	}
}

void FillUnorderedMap(unordered_map<int, string>& mp, int size)
{
	mp.clear();
	char buf[255];
	for (int i = 0; i < size; i++)
	{
		sprintf(buf, "#%d", i + 1);
		mp[i + 1] = string(buf);
	}
}

template<typename TC>
bool contains(int elem, TC& coll) {
	return coll.find(elem) != coll.end();
}

template<typename T>
bool InBuiltArrayEqual(const T a1[], const T a2[], int size)
{
	if (a1 == nullptr || a2 == nullptr)
		return false;

	try
	{
		for (int i = 0; i<size; i++)
		{
			if (a1[i] != a2[i])
				return false;
		}
		return true;
	}
	catch{
		return false;
	}
}

template<typename TC>
bool CollEqual(const TC& coll1, const TC& coll2)
{
	return coll1 == coll2;
}