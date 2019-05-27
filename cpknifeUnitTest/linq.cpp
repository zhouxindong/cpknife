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

			auto vec = result.to_vector();
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

			auto vec2 = result2.to_vector();
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
			{return s[0] == 'a'; }).to_list();
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
			}).to_list();
			Assert::AreEqual(2, (int)rng2.size());
		}

		TEST_METHOD(takeTest) {
			vector<int> vec = { 1,2,3,4,5,6 };
			auto rst = from(vec).take(10).to_vector();
			Assert::IsTrue(rst == vec);

			auto rst2 = from(vec).take(6).to_vector();
			Assert::IsTrue(rst2 == vec);

			auto rst21 = from(vec).take(2).to_vector();
			vector<int> exp21 = { 1,2 };
			Assert::IsTrue(exp21 == rst21);

			vector<int> exp = { 1,2,3 };
			auto rst3 = from(vec).take(3).to_vector();
			Assert::IsTrue(rst3 == exp);

			vector<int> exp2 = { 1 };
			auto rst4 = from(vec).take(1).to_vector();
			Assert::IsTrue(rst4 == exp2);

			vector<int> exp3 = {};
			auto rst5 = from(vec).take(0).to_vector();
			Assert::IsTrue(rst5 == exp3);

			vector<int> vec2 = { 5 };
			vector<int> exp4 = { 5 };
			auto rst6 = from(vec2).take(5).to_vector();
			Assert::IsTrue(rst6 == exp4);

			auto rst7 = from(vec2).take(1).to_vector();
			Assert::IsTrue(rst7 == exp4);

			auto rst8 = from(vec2).take(0).to_vector();
			Assert::IsTrue(rst8 == exp3);

			vector<int> vec3 = {};
			auto rst9 = from(vec3).take(0).to_vector();
			Assert::IsTrue(rst9 == exp3);

			auto rst10 = from(vec3).take(100).to_vector();
			Assert::IsTrue(rst10 == exp3);
		}

		vector<int> empty_vec = {};

		TEST_METHOD(takewhileTest2)
		{
			vector<int> vec5 = { 1,2,3,4,5,6,7,8,9 };
			vector<int> exp5 = { 2,4,6 };
			auto rst11 = from(vec5).take(6).where([](int e) {
				return e % 2 == 0;
			}).to_vector();
			Assert::IsTrue(rst11 == exp5);

			auto rst12 = from(vec5).take(3).where([](int e) {
				return e > 100;
			}).to_vector();
			vector<int> empty_vec = {};
			Assert::IsTrue(rst12 == empty_vec);
		}

		TEST_METHOD(skipTest)
		{
			vector<int> vec = { 1,2,3,4,5,6,7,8,9 };
			auto rst = from(vec).skip(0).to_vector();
			Assert::IsTrue(vec == rst);

			auto rst1 = from(vec).skip(100).to_vector();
			Assert::IsTrue(rst1 == empty_vec);

			vector<int> exp2 = { 6,7,8,9 };
			auto rst2 = from(vec).skip(5).to_vector();
			Assert::IsTrue(exp2 == rst2);

			vector<int> exp3 = { 7,9 };
			auto rst3 = from(vec).skip(5).where([](int e) {
				return e % 2 == 1;
			});
		}

		TEST_METHOD(selectTest)
		{
			vector<int> src = { 1,2,3,4 };
			vector<int> exp = { 2,4,6,8 };

			auto rst = from(src).select([](int a) {
				return a * 2;
			}).to_vector();
			Assert::IsTrue(rst == exp);

			char buf[16];
			auto rst2 = from(src).select([&](int a) {
				sprintf(buf, "%d", a);
				return string(buf);
			}).to_vector();
			vector<string> exp2 = { "1", "2", "3", "4" };
			Assert::IsTrue(rst2 == exp2);
		}

		struct Man
		{
			std::string name;
			int age;

			bool operator < (const Man & man) const
			{
				return (name < man.name)
					|| (name == man.name && age < man.age);
			}

			//bool operator == (const Man & man) const
			//{
			//	return (name == man.name);
			//}

			//bool operator == (const std::string & manName) const
			//{
			//	return (this->name == manName);
			//}
		};


		TEST_METHOD(distinctTest) {
			vector<int> src = { 4,5,3,1,4,2,1,4,6 };
			vector<int> exp = { 4,5,3,1,2,6 };
			auto rst = from(src).distinct().to_vector();
			Assert::IsTrue(rst == exp);

			Man src2[] =
			{
				{ "Anton",1 },
				{ "Taran",2 },
				{ "Poker",3 },
				{ "Agata",4 },
				{ "Anton",2 },
				{ "Banan",1 },

			};

			auto rst2 = from(src2).
				distinct([](const Man & man) {return man.name; }).to_vector();
			Assert::AreEqual(5, (int)rst2.size());
		}

		TEST_METHOD(order_byTest)
		{
			vector<int> src = { 4,5,3,1,4,2,1,4,6 };
			vector<int> exp = { 1,1,2,3,4,4,4,5,6 };

			auto rst = from(src).order_by().to_vector();
			Assert::IsTrue(exp == rst);

			vector<string> src2 = {
				"microsoft", "intel", "nokia", "apple", "oracle", "sun"
			};
			vector<string> exp2 = {
				"sun", "intel", "nokia", "apple", "oracle", "microsoft"
			};
			auto rst2 = from(src2).order_by(
				[](const string&s) {
				return s.size(); }).to_vector();
				Assert::IsTrue(exp2 == rst2);
		}

		TEST_METHOD(reverseTest) {
			vector<int> src = { 1,2,3,4 };
			vector<int> exp = { 4,3,2,1 };
			auto rst = from(src).reverse().to_vector();
			Assert::IsTrue(exp == rst);

			auto rst2 = from(src).reverse().reverse().to_vector();
			Assert::IsTrue(rst2 == src);
		}

		//TEST_METHOD(bytesTest) {
		//	vector<unsigned int> src = { 0x12345678, 0xAABBCCDD };
		//	vector<int> exp = { 0x78, 0x56, 0x34, 0x12, 0xDD, 0xCC, 0xBB, 0xAA };
		//	auto rst = from(src).bytes(FirstToLast).to_vector();
		//	Assert::IsTrue(exp == rst);
		//}

		//TEST_METHOD(unbytesTest) {
		//	vector<unsigned char> src = { 0x78, 0x56, 0x34, 0x12, 0xAA, 0xBB, 0xCC, 0xDD };
		//	vector<unsigned int> exp = { 0x12345678, 0xDDCCBBAA };
		//	auto rst = from(src).unbytes(FirstToLast).to_vector();
		//}
	};
}