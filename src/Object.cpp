#include "objects/Object.h"

#include <string>

#include "exceptions.h"
#include "GIL2.h"
#include "objects/Object.h"
#include "objects/Bool.h"
#include "objects/NativeFunction.h"

using namespace std;
using namespace ck_exceptions;
using namespace ck_vobject;
using namespace ck_objects;
using namespace ck_core;

vobject* Object::create_proto() {
	if (ObjectProto != nullptr)
		return ObjectProto;
	
	ObjectProto = new Object();
	GIL::gc_instance()->attach_root(ObjectProto);
	
	// ...
	ObjectProto->put(L"__contains", new NativeFunction(
		[](vscope* scope, const vector<vobject*>& args) -> vobject* {
			// Validate __this
			if (!scope) return nullptr;
			vobject* __this = scope->get(L"__this", 1);
			if (!__this)
				return nullptr;
			
			// Validate args
			bool con = 1;
			for (auto &i : args)
				if (!i || !static_cast<Object*>(__this)->contains(i->string_value())) {
					con = 0;
					break;
				}
			
			if (con)
				return Bool::True();
			else
				return Bool::False();
		}));
	
	return ObjectProto;
};


Object::Object(const std::map<std::wstring, ck_vobject::vobject*>& objec) {
	objects = objec;
	
	put(wstring(L"proto"), ObjectProto);
};

Object::Object() {
	// if (ObjectProto == nullptr)
	// 	Object::create_proto();
	
	put(wstring(L"proto"), ObjectProto);
};
		
Object::~Object() {
	
};
		
		
vobject* Object::get(vscope* scope, const wstring& name) {
	vobject* ret = get(name);

	if (!ret && ObjectProto != this && ObjectProto)
		return ObjectProto->get(scope, name);
	return ret;
};

void Object::put(vscope* scope, const wstring& name, vobject* object) {
	put(name, object);
};

bool Object::contains(vscope* scope, const wstring& name) {
	return contains(name) || (ObjectProto != this && ObjectProto && ObjectProto->contains(scope, name));
};

bool Object::remove(vscope* scope, const wstring& name) {
	if (remove(name))
		return 1;
	if (ObjectProto != this && ObjectProto && ObjectProto->remove(scope, name))
		return 1;
	return 0;
};

vobject* Object::call(vscope* scope, const vector<vobject*>& args) {
	// XXX: Construct object from input
	throw UnsupportedOperation(L"Object is not callable");
};


void Object::gc_mark() {
	if (gc_reachable)
		return;
	
	gc_reach();
	
	for (const auto& any : objects) 
		if (any.second && !any.second->gc_reachable)
			any.second->gc_mark();
};

void Object::gc_finalize() {};
		
// Object functions only

void Object::append(Object* obj) {
	if (!obj)
		return;
	
	objects.insert(obj->objects.begin(), obj->objects.end());
};

vector<wstring> Object::keys() {
	vector<wstring> keys;
	
	for (const auto& any : objects) 
		keys.push_back(any.first);
	
	return keys;
};

// Scope-independent getter-setter-checker.
void Object::put(const wstring& name, vobject* object) {
	//wcout << this->string_value() << " putting " << name << " with value of " << (object == nullptr ? L"nullptr" : object->string_value()) << endl;
	objects[name] = object;
};

vobject* Object::get(const wstring& name) {
	
	#ifndef CK_SINGLETHREAD
		std::unique_lock<std::mutex> lck(mutex());
	#endif
	
	//wcout << this->string_value() << " getting " << name << endl;
	map<wstring, vobject*>::const_iterator pos = objects.find(name);
	if (pos == objects.end())
		return nullptr;
	return pos->second;
};

bool Object::contains(const wstring& name) {
	
	#ifndef CK_SINGLETHREAD
		std::unique_lock<std::mutex> lck(mutex());
	#endif
	
	map<wstring, vobject*>::const_iterator pos = objects.find(name);
	if (pos == objects.end())
		return 0;
	return 1;
};

bool Object::remove(const wstring& name) {
	
	#ifndef CK_SINGLETHREAD
		std::unique_lock<std::mutex> lck(mutex());
	#endif
	
	map<wstring, vobject*>::const_iterator pos = objects.find(name);
	if (pos == objects.end())
		return 0;
	objects.erase(pos);
	return 1;
};

// Must return integer representation of an object
long long Object::int_value() { 
	return (int) (intptr_t) this; 
};

// Must return string representation of an object
std::wstring Object::string_value() { 
	return std::wstring(L"[Object ") + std::to_wstring((int) (intptr_t) this) + std::wstring(L"]"); 
};

