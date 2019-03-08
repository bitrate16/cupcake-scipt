#include "objects/String.h"

#include <string>

#include "exceptions.h"
#include "GIL2.h"

using namespace std;
using namespace ck_exceptions;
using namespace ck_vobject;
using namespace ck_objects;
using namespace ck_core;

vobject* String::create_proto() {
	StringProto = new Object();
	GIL::gc_instance()->attach_root(StringProto);
	
	// ...
	
	return StringProto;
};


String::String() {
	if (StringProto == nullptr)
		create_proto();
	
	Object::put(wstring(L"proto"), StringProto);
};

String::String(const std::wstring* s) {
	if (StringProto == nullptr)
		create_proto();
	
	str = *s;
	
	Object::put(wstring(L"proto"), StringProto);
};

String::String(const std::wstring& s) {
	if (StringProto == nullptr)
		create_proto();
	
	str = s;
	
	Object::put(wstring(L"proto"), StringProto);
};

String::~String() {
	
};

// Map any sign index to [0, size]
vobject* String::get(ck_vobject::vscope* scope, const std::wstring& name) {
	// Check if string is valid number
	bool is_int = 1;
	int chk_ind = 0;
	if (name[chk_ind] == U'-' || name[chk_ind] == U'+')
		++chk_ind;
	for (; chk_ind < name.size(); ++chk_ind)
		if (U'0' <= name[chk_ind] && U'9' <= name[chk_ind]) {
			is_int = 0;
			break;
		}
		
	if (is_int) {
		int index = std::stoi(name);
		index = ((index % str.size()) + str.size()) % str.size();
		
		// Substring of size 1
		return new String(str.substr(index, 1));
	}
	
	// Else return variable by name
	vobject* ret = Object::get(name);
	if (!ret && StringProto)
		return StringProto->get(scope, name);
	return ret;
};

// Map any sign index to [0, size]
// Disallow assignment of characters.
void String::put(ck_vobject::vscope* scope, const std::wstring& name, vobject* object) {
	// Check if string is valid number
	bool is_int = 1;
	int chk_ind = 0;
	if (name[chk_ind] == U'-' || name[chk_ind] == U'+')
		++chk_ind;
	for (; chk_ind < name.size(); ++chk_ind)
		if (U'0' <= name[chk_ind] && U'9' <= name[chk_ind]) {
			is_int = 0;
			break;
		}
	
	// If valid integer
	if (is_int) 
		throw ck_message(L"can not change const string", ck_message_type::CK_UNSUPPORTED_OPERATION);
	
	// Else put variable by name
	if (Object::contains(name))
		Object::put(name, object);
	else if(StringProto)
		StringProto->put(scope, name, object);
};

bool String::contains(ck_vobject::vscope* scope, const std::wstring& name) {
	// Check if string is valid number
	bool is_int = 1;
	int chk_ind = 0;
	if (name[chk_ind] == U'-' || name[chk_ind] == U'+')
		++chk_ind;
	for (; chk_ind < name.size(); ++chk_ind)
		if (U'0' <= name[chk_ind] && U'9' <= name[chk_ind]) {
			is_int = 0;
			break;
		}
	
	// If valid integer
	if (is_int) {
		int index = std::stoi(name);
		if (index < 0)
			return 0;
		
		if (index < str.size())
			return 1;
		
		return 0;
	}
	
	return Object::contains(name) || (StringProto && StringProto->contains(scope, name));
};

bool String::remove(ck_vobject::vscope* scope, const std::wstring& name) {
	// Check if string is valid number
	bool is_int = 1;
	int chk_ind = 0;
	if (name[chk_ind] == U'-' || name[chk_ind] == U'+')
		++chk_ind;
	for (; chk_ind < name.size(); ++chk_ind)
		if (U'0' <= name[chk_ind] && U'9' <= name[chk_ind]) {
			is_int = 0;
			break;
		}
	
	// If valid integer
	if (is_int) 
		throw ck_message(L"can not change const string", ck_message_type::CK_UNSUPPORTED_OPERATION);
	
	if (Object::remove(name))
		return 1;
	if (StringProto && StringProto->remove(scope, name))
		return 1;
	return 0;
};

vobject* String::call(ck_vobject::vscope* scope, std::vector<vobject*> args) {
	throw ck_message(L"String is not callable", ck_message_type::CK_UNSUPPORTED_OPERATION);
};

void String::gc_mark() {
	Object::gc_mark();
};

void String::gc_finalize() {
	Object::gc_finalize();
};


// String functions only

// I. If string length == 1 -> returns charcode at [0].
// II. If string length == 0 -> returns 0.
// III. If string is format of int -> return parsed int.
// IIII. Else return length.
long long String::int_value() {
	if (str.size() == 1) 
		return str[0];
	if (str.size() == 0)
		return 0;
	
	bool is_int = 1;
	int chk_ind = 0;
	if (str[chk_ind] == U'-' || str[chk_ind] == U'+')
		++chk_ind;
	for (; chk_ind < str.size(); ++chk_ind)
		if (U'0' <= str[chk_ind] && U'9' <= str[chk_ind]) {
			is_int = 0;
			break;
		}
	
	if (is_int)
		return std::stoi(str);;
	
	return str.size();
};

// Returns string
wstring String::string_value() {
	return str;
};

// Returns length of the string
int String::length() {
	return str.size();
};
/*
// Returns reference to an index of string
wchar_t& operator[](int index);

// Returns value of string index
wchar_t& operator[](int index) const;

// Compare two strings
bool operator==(const String&);

// Uncompare two strings
bool operator!=(const String&);
*/

// String operations

// Returns char at position.
//  Returns -1 if out of bounds.
wchar_t String::charAt(int index) {
	// XXX: Maybe throw range error?
	if (index < 0 || index >= str.size())
		return -1;

	return str[index];
};

std::wstring String::concatenate(std::wstring& string) {
	return str + string;
};

// Remove all whitespace characters in front of the string
std::wstring String::stripLeading() {
	int i = 0;
	while (i < str.size())
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r')
			++i;
		else
			break;
	
	if (i == str.size() - 1)
		return L"";
	
	return str.substr(i, str.size() - i);
};

// Remove all whitespace characters in back of the string
std::wstring String::stripTrailing() {
	int i = str.size() - 1;
	while (i >= 0)
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r')
			--i;
		else
			break;
	
	if (i == 0)
		return L"";
	
	return str.substr(0, i);
};

// ~ str.stripLeading().stripTrailing()
std::wstring String::strip() {
	int a = 0;
	while (a < str.size())
		if (str[a] == ' ' || str[a] == '\t' || str[a] == '\n' || str[a] == '\r')
			++a;
		else
			break;
	
	if (a == str.size() - 1)
		return L"";
	
	int b = str.size() - 1;
	while (b >= 0)
		if (str[b] == ' ' || str[b] == '\t' || str[b] == '\n' || str[b] == '\r')
			--b;
		else
			break;
	
	if (b == 0)
		return L"";
	
	return str.substr(a, b - a);
	
};

// Returns 1 if string is empty
bool String::isEmpty() {
	return str.size() == 0;
};

// Returns 1 if string consists of whitespace characters
bool String::isBlank() {
	int a = 0;
	while (a < str.size())
		if (str[a] == ' ' || str[a] == '\t' || str[a] == '\n' || str[a] == '\r')
			++a;
		else
			return 0;
	return 1;
};

// Returns index of character in string or -1
int indexOf(wchar_t);

// Returns index of character in string from back or -1
int lastIndexOf(wchar_t);

// Returns index of sobstring in string from or -1
int indexOf(std::wstring);

// Returns index of sobstring in string from back or -1
int lastIndexOf(std::wstring);

// Replace $1 with $2
std::wstring replace(wchar_t, wchar_t);

// Replace $1 with $2
std::wstring replace(std::wstring, std::wstring);

// Replace all $1 with $2
std::wstring replaceAll(std::wstring, std::wstring);

// Check for $1 is contained in string
std::wstring contains(wchar_t);

// Check for $1 is substring
std::wstring contains(std::wstring);

// Returns sobstring [0, $1]
std::wstring substring(int);

// Returns sobstring [$1, $2]
std::wstring substring(int, int);