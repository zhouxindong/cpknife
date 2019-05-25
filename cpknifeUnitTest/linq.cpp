#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\linq.h"
#include "collectutils.h"
#include <fstream>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace cpknife;

namespace cpknifeUnitTest
{		
	ofstream of("linq_test.txt", ios::out | ios::trunc);

	TEST_CLASS(linqTest)
	{
	public:
		
		TEST_METHOD(foreachTest)
		{
			int ar[100];
			FillInBuildArray(ar, 100);
			int index = 1;
			from(ar).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(101, index);

			array<int, 1000> ary;
			FillArray(ary);
			index = 1;
			from(ary).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(1001, index);

			index = 1;
			from(ary).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(1001, index);


			vector<int> vec;
			FillVector(vec, 20);
			index = 1;
			from(vec).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(21, index);

			deque<int> deq;
			FillDeque(deq, 100);
			index = 1;
			from(deq).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(101, index);

			list<int> li;
			FillList(li, 50);
			index = 1;
			from(li).foreach([&](int e) {
				Assert::AreEqual(index++, e);
			});
			Assert::AreEqual(51, index);

			forward_list<int> foli;
			FillForwardList(foli, 50);
			index = 50;
			from(foli).foreach([&](int e) {
				Assert::AreEqual(index--, e);
				int j = e;
			});
			Assert::AreEqual(0, index);

			set<int> se;
			FillSet(se, 50);
			set<int> se_copy;
			from(se).foreach([&](int e) {
				se_copy.insert(e);
			});
			for (int i=0; i<50; i++)
			{
				Assert::IsTrue(contains(i + 1, se_copy));
			}

			map<int, string> mp;
			FillMap(mp, 500);
			map<int, string> mp_copy;
			from_map(mp).foreach([&](pair<int, string> e) {
				mp_copy.insert(e);
			});
			for (int i = 0; i < 500; i++)
			{
				Assert::IsTrue(contains(i + 1, mp_copy));
			}

			unordered_set<int> udse;
			FillUnorderedSet(udse, 50);
			unordered_set<int> udse_copy;
			from_unordered_set(udse).foreach([&](int e) {
				udse_copy.insert(e);
			});
			for (int i = 0; i < 50; i++)
			{
				Assert::IsTrue(contains(i + 1, udse_copy));
			}

			unordered_map<int, string> udmp;
			FillUnorderedMap(udmp, 500);
			unordered_map<int, string> udmp_copy;
			from(udmp).foreach([&](pair<int, string> e) {
				udmp_copy.insert(e);
			});
			for (int i = 0; i < 500; i++)
			{
				Assert::IsTrue(contains(i + 1, udmp_copy));
			}
		}

		TEST_METHOD(whereTest) {
			int src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

			auto result = from(src).where([](int a)
			{
				return a % 2 == 1;
			});

			auto vec = result.ToVector();
			Assert::AreEqual(5, (int)vec.size());
			Assert::AreEqual(1, vec[0]);
			Assert::AreEqual(3, vec[1]);
			Assert::AreEqual(5, vec[2]);
			Assert::AreEqual(7, vec[3]);
			Assert::AreEqual(9, vec[4]);

			auto result2 = from(src).where([](int a)
			{
				return a % 2 == 1;
			});

			auto vec2 = result2.ToVector();
			Assert::AreEqual(5, (int)vec2.size());
			Assert::AreEqual(1, vec2[0]);
			Assert::AreEqual(3, vec2[1]);
			Assert::AreEqual(5, vec2[2]);
			Assert::AreEqual(7, vec2[3]);
			Assert::AreEqual(9, vec2[4]);


			std::string str_src[] =
			{
				"apple",
				"blackberry",
				"adobe",
				"microsoft",
				"nokia",
			};

			auto rng = from(str_src).where([](string s)
			{return s[0] == 'a'; }).ToList();
			Assert::AreEqual(2, (int)rng.size());
			Assert::AreEqual(string("apple"), rng.front());
			rng.pop_front();
			Assert::AreEqual(string("adobe"), rng.front());

			struct NameAge
			{
				std::string name;
				int age;
			};

			NameAge obj_src[] =
			{
				{ "man1",20 },
				{ "man2",15 },
				{ "man3",30 },
				{ "man4",14 },
				{ "man5",18 },
			};

			auto rng2 = from(obj_src).where([](const NameAge& a) {
				return a.age < 18; 
			}).ToList();
			Assert::AreEqual(2, (int)rng2.size());
		}

		TEST_METHOD(takeTest) {
			vector<int> vec = { 1,2,3,4,5,6 };
			auto rst = from(vec).take(10).ToVector();
			Assert::IsTrue(rst == vec);

			auto rst2 = from(vec).take(6).ToVector();
			Assert::IsTrue(rst2 == vec);

			vector<int> exp = { 1,2,3 };
			auto rst3 = from(vec).take(3).ToVector();
			Assert::IsTrue(rst3 == exp);

			vector<int> exp2 = { 1 };
			auto rst4 = from(vec).take(1).ToVector();
			Assert::IsTrue(rst4 == exp2);

			vector<int> exp3 = {};
			auto rst5 = from(vec).take(0).ToVector();
			Assert::IsTrue(rst5 == exp3);

			vector<int> vec2 = { 5 };
			vector<int> exp4 = { 5 };
			auto rst6 = from(vec2).take(5).ToVector();
			Assert::IsTrue(rst6 == exp4);

			auto rst7 = from(vec2).take(1).ToVector();
			Assert::IsTrue(rst7 == exp4);

			auto rst8 = from(vec2).take(0).ToVector();
			Assert::IsTrue(rst8 == exp3);

			vector<int> vec3 = {};
			auto rst9 = from(vec3).take(0).ToVector();
			Assert::IsTrue(rst9 == exp3);

			auto rst10 = from(vec3).take(100).ToVector();
			Assert::IsTrue(rst10 == exp3);
		}

	};
}