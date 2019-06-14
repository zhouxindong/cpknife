#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\ajson.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//struct person
//{
//	string name;
//	int age;
//};

enum PhoneType {
	MOBILE = 0,
	HOME = 1,
	WORK = 2
};

class PhoneNumber {
public:
	string number;
	PhoneType type = MOBILE;

	PhoneNumber(const string& number_ = "", PhoneType type_ = MOBILE)
		: number(number_), type(type_) {}
};

class FullPerson
{
public:
	string name;
	int id;
	string mail;
	vector<PhoneNumber> phones;

	FullPerson(string name_ = "", int id_ = 0, string mail_ = "")
		: name(name_), id(id_), mail(mail_) {}
};

class AddressBook
{
public:
	vector<FullPerson> peoples;
};

AJSON(person, name, age)

AJSON(PhoneNumber, number, type)
AJSON(FullPerson, name, id, mail, phones)
AJSON(AddressBook, peoples)

namespace cpknifeUnitTest
{


	TEST_CLASS(ajsontest)
	{
	public:

		TEST_METHOD(structtest)
		{
			string buf = R"({"name":"jsonname", "age":33})";
			person p;
			cpknife::load_from_buff(p, buf.c_str());
			Assert::AreEqual(string("jsonname"), p.name);
			Assert::AreEqual(33, p.age);

			FullPerson p1("Ajax1", 3293, "Ajax1.mail");
			p1.phones.push_back(PhoneNumber("1234567890", MOBILE));
			p1.phones.push_back(PhoneNumber("11132", HOME));
			p1.phones.push_back(PhoneNumber("323123", WORK));

			FullPerson p2("Boston", 9818, "Boston.uc.mail");
			p2.phones.push_back(PhoneNumber("39823356", MOBILE));

			AddressBook book;
			book.peoples.push_back(p1);
			book.peoples.push_back(p2);

			cpknife::string_stream ss;
			cpknife::save_to(ss, book);

			AddressBook book2;
			cpknife::load_from_buff(book2, ss.str().c_str());

			ostringstream oss;
			cpknife::save_to(oss, book);
			AddressBook book3;
			cpknife::load_from_buff(book3, oss.str().c_str());

		}
	};
}
