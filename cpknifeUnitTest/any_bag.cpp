#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\cpknife\any_bag.h"
#include "..\cpknife\json11.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace cpknife;

namespace cpknifeUnitTest
{
	TEST_CLASS(any_bag_Test)
	{
	public:

		TEST_METHOD(any_bag_Test1)
		{
			BAG_object root;
			root.set("username", BAG_string("pksunkara"));
			root.set("age", BAG_number(25));
			root.set("literate", BAG_bool(true));
			root.set("email", BAG_null());
			root.set("age", BAG_number(39));
			BAG_array interests;
			interests.push(BAG_string("cricket"));
			interests.push(BAG_string("programming"));
			interests.set(1, BAG_string("computers"));
			root.set("interests", interests);
			serialize_json serializer;
			ostringstream oss;
			serializer.process(root, oss);
			auto json_text = oss.str();

			string err;
			auto json1 = json11::Json::parse(oss.str(), err);

			Assert::AreEqual(json1["username"].string_value(), string("pksunkara"));
			Assert::AreEqual(json1["age"].number_value(), (double)39);
			Assert::IsTrue(json1["literate"].bool_value());
			Assert::IsTrue(json1["email"].is_null());
			Assert::AreEqual(
				json1["interests"].array_items().at(0).string_value(),
				string("cricket"));
			Assert::AreEqual(
				json1["interests"].array_items().at(1).string_value(),
				string("computers"));

			auto dump_text = json1.dump();
		}
	};
}
