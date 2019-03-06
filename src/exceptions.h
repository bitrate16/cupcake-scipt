#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <cwchar>


// Exception type used for sending messages, interrupts and 
// local information over the cupcake VM.
// Allows twrowing from any place of script context and
// will be safety processed by cupcake try/catch statements.
namespace ck_exceptions {		
	enum ck_message_type {
		CK_WMESSAGE,
		CK_MESSAGE,
		CK_STRING
	};
	
	class ck_message {
		ck_message_type message_type;
		
		// CK_WMESSAGE: Onlys tring message that is thrown to up
		const wchar_t* native_wmessage = nullptr;
		
		// CK_MESSAGE: Onlys tring message that is thrown to up
		const char* native_message = nullptr;
		
		// CK_STRING: Onlys tring message that is thrown to up
		std::wstring native_string = nullptr;
		
	public:		
		
		ck_message(const wchar_t* message) throw() : native_wmessage(message), message_type(CK_WMESSAGE) {};
		ck_message(const char* message) throw() : native_message(message), message_type(CK_MESSAGE) {};
		ck_message(const std::wstring& message) throw() : native_string(message), message_type(CK_STRING) {};

		
		virtual ~ck_message() throw();
		
		friend std::wostream& operator<<(std::wostream& os, const ck_message& m);
	};
	
	std::wostream& operator<<(std::wostream& os, const ck_message& m);
};