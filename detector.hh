#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "g4root.hh"
#include <set>  // <-- ADD THIS
#include <unordered_map>
#include <string>

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String);
	~MySensitiveDetector();
	
	
private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
	std::unordered_map<int, std::set<std::string>> trackHistory;
};

#endif 



