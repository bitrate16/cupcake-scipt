#pragma once

#include "GIL2.h"
#include "GC.h"


namespace ck_core {		
	/*
	 * Garbage collector object, tracked at creation.
	 */
	class GC;
	class gc_list;
	class gc_object {
	
	protected:
		bool gc_reachable = 0;
		
	public:
		
		gc_object();
		virtual ~gc_object();
		
		// Called when GC indexes all reachable objects
		virtual void gc_mark();
		
		// Called when GC destroyes current object
		virtual void gc_finalize();
		
		// Mark current object as reachable
		inline void gc_reach() { gc_reachable = 1; };
		
	private:
	
		// Allow access only from GC class.
		friend class GC;
		
		// Set to 1 if object is being recorded
		bool gc_record;
		// Set to 1 if object is GC root
		bool   gc_root;
		// Set to 1 if object is locked (like root, but not root, okay?)
		bool   gc_lock;
		
		// Pointer to alignation in current GC chain
		gc_list *gc_chain;
		// Pointer to alignation to current GC lock
		gc_list *gc_lock_chain;
		// Pointer to alignation to current GC root
		gc_list *gc_root_chain;
		
		// Allow delete only from GC
		void operator delete  (void* ptr);
		void operator delete[](void* ptr);
	};
	
	/*
	 * Garbage collector chain.
	 */
	class gc_list {
	
	public:
	
		gc_list  *next;
		gc_object *obj;
		
		// For stupid users, that decide to GC by themself even after delete is forbidden.
		bool deleted_ptr;
		
		gc_list();
	};

	/*
	 * Garbage collector. Collects your shit.
	 */
	class GC {
	
	private:
	
		// Protects object from multiple threads access.
		std::mutex protect_lock;
		int collecting;
		int size;
		int roots_size;
		int locks_size;
		gc_list *objects;
		gc_list *roots;
		gc_list *locks;
		
		// Number of objects created since last gc_collect pass
		std::atomic<int> created_interval;
		// Number of minimum objects to be created before next GC
		// Yes, i like number 64.
		// 64 is like 8 * 8 and 2 << (8 - 2).
		// Or it can be represented as sum of eight 1's.
		// 
		// Yes, i'm capitan and that's my ship.
        // 		                                               _  _
        //                                                    ' \/ '
        //    _  _                        <|
        //     \/              __'__     __'__      __'__
        //                    /    /    /    /     /    /
        //                   /\____\    \____\     \____\               _  _
        //                  / ___!___   ___!___    ___!___               \/
        //                // (      (  (      (   (      (
        //              / /   \______\  \______\   \______\
        //            /  /   ____!_____ ___!______ ____!_____
        //          /   /   /         //         //         /
        //       /    /   |         ||         ||         |
        //      /_____/     \         \\         \\         \
        //            \      \_________\\_________\\_________\
        //             \         |          |         |
        //              \________!__________!_________!________/
        //               \|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_/|
        //                \    _______________                /
        // ^^^%%%^%^^^%^%%^\_"/_)/_)_/_)__)/_)/)/)_)_"_'_"_//)/)/)/)%%%^^^%^^%%%%^
        // ^!!^^"!%%!^^^!^^^!!^^^%%%%%!!!!^^^%%^^^!!%%%%^^^!!!!!!%%%^^^^%^^%%%^^^!
		// ^!!!!^$$$!^!^!^!^$^!^$^!^  THIS IS THE BOTTOM  ^!!!$$$$^$^!$^!^!^!$^$$$
		// &&&&!^$^^^^$$$!!!!!!^$^!^   > YOU ARE HERE <   ^!&$$$$^&&&&&&^^^^^!!$$$
        // ^!!^^&^^^!^^^!!^^^%%%^^!&!!^^^%%^^^!!%%%%^^^!!!!!!%%%^^^^%^^%%%^^^!!&&^
		//
		// We've been travelling for too long over that sea.
		//
        //                      _==|            
        //            _==|   )__)  |
        //              )_)  )___) ))
        //             )___) )____))_)
        //        _    )____)_____))__)\
        //         \---__|____/|___|___-\\---
        // ^^^^^^^^^\   oo oo oo oo     /~~^^^^^^^
        //   ~^^^^ ~~~~^^~~~~^^~~^^~~~~~
        //     ~~^^      ~^^~     ~^~ ~^ ~^
        //          ~^~~        ~~~^^~
        // 
		// But now we have reached the target
		//
        //     ,-'"""`-,    
        //   ,' \ _|_ / `.  
        //  /`.,'\ | /`.,'\ 
        // (  /`. \|/ ,'\  )
        // |--|--;=@=:--|--|
        // (  \,' /|\ `./  )
        //  \,'`./ | \,'`./ 
        //   `. / """ \ ,'  
        //     '-._|_,-`    
        // 
		// Here it is:
		//
		//     /\/\/\/\/\/\/\/\
        //    / ╔══╗╔╗╔╗╔╗╔══╗ \
        //    \ ║╔╗║║║║║║║║╔╗║ /
        //    / ║║║║║║║║║║║║║║ \
        //    \ ║║║║║║║║║║║║║║ /
        //    / ║╚╝║║╚╝╚╝║║╚╝║ \
        //    \ ╚══╝╚═╝╚═╝╚══╝ /
		//     \/\/\/\/\/\/\/\/
		//
		// 	   ( ͡° ͜ʖ ͡°) what's this?
		// 
		// >> https://www.youtube.com/watch?v=osR1jctb47Y <<
		// 
		const int MIN_CREATED_INTERVAL = 64;
		
	public:
		
		GC();
		~GC();
		
		public:
		// Called on object creation to attach it to the current instance of GC.
		void attach(ck_vobject::vobject *o);
		
		// Called to make given object root object
		void attach_root(ck_vobject::vobject *o);
		void deattach_root(ck_vobject::vobject *o);
		
		// Called to lock given object from deletion.
		void lock(ck_vobject::vobject *o);
		void unlock(ck_vobject::vobject *o);
		
		// Amount of objects registered by GC.
		int count();
		
		// Amount of roots
		int roots_count();
		
		// Amount of locked obejcts
		int locks_count();
		
		void collect();
		void dispose();
	};
};