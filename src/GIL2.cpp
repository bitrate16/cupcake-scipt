#include "GIL2.h"
#include "GC.h"

using namespace ck_core;

GIL::GIL() {
	// Assign self instance
	gil = this;
	
	// No lock needed. First start.
	// Allocate pointer to the new ckthread
	current_thread = new ckthread();
	threads->push_back(current_thread);
	
	// Current (creator) thread is being tracked as primary thread.
	// This thread mush override all system signals and woke System.sysint(code)
	// in the current thread and executer.
	// When current thread finishes befure other threads it has to start waiting 
	// for other threads to finish their work. Current thread must lock on sync_condition
	// and every woke up check if there is any other threads.
	
	// All this ^ actions is done from main().
};

// Ignore for signals
static void dummy_signal(int signal) {
	signal(SIGINT,  dummy_signal); // <-- user interrupt
	signal(SIGTERM, dummy_signal); // <-- Terminate request
	signal(SIGABRT, dummy_signal); // <-- abortion is murder
};

GIL::~GIL() {
	// Program termination.
	// Waiting till everything is completely dead.
	// Prevent creating new threads.
	std::unique_lock<std::mutex> lock(threads_lock);
	
	// At this moment ignoring all signals from OS.
	signal(SIGINT,  dummy_signal); // <-- user interrupt
	signal(SIGTERM, dummy_signal); // <-- Terminate request
	signal(SIGABRT, dummy_signal); // <-- abortion is murder
	
	// Derstroy all ck_threads (delete instances)
	for (int i = 0; i < threads.size(); ++i) {
		delete threads[i]->thread;
		delete threads[i];
	}
	
	// Call last garbage collection
	GC.dispose();
	
	// Finally commit suicide
	delete this;
};

