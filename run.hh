#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "g4root.hh"
#include <map>
#include <string>

class MyRunAction : public G4UserRunAction
{
public:
	MyRunAction();
	~MyRunAction();
	
	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
	
	static std::map<std::string, int> particleCounter;
	static std::map<std::string, int> particleCounter1;
	
};

#endif
