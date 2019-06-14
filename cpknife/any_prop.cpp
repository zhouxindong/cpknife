#include "stdafx.h"
#include "any_prop.h"
#include "linq.h"
#include <sstream>

using namespace std;

std::string cpknife::any_prop::PropertiesInfo()
{
	ostringstream oss;
	bool first = true;

	from_map(m_properties).foreach([&](auto& pair) {
		if (!first) {
			oss << ";";
		}
		first = false;
		oss << pair.second.second.c_str() << "#";
		oss << pair.second.first.get()->m_type->name();
	});

	return oss.str();
}
