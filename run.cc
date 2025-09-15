#include "run.hh"



std::map<std::string, int> MyRunAction::particleCounter = {};
std::map<std::string, int> MyRunAction::particleCounter1 = {};

MyRunAction::MyRunAction()
{

auto analysisManager = G4AnalysisManager::Instance();

// One ntuple per particle type
analysisManager->CreateNtuple("Hits", "Local coords and momentum");
analysisManager->CreateNtupleDColumn("x");
analysisManager->CreateNtupleDColumn("y");
analysisManager->CreateNtupleDColumn("z");
analysisManager->CreateNtupleDColumn("Px");
analysisManager->CreateNtupleDColumn("Py");
analysisManager->CreateNtupleDColumn("Pz");
analysisManager->CreateNtupleDColumn("P");
analysisManager->CreateNtupleDColumn("energy");
analysisManager->CreateNtupleDColumn("angle");
//analysisManager->FinishNtuple(0);
analysisManager->CreateNtupleDColumn("PID");
analysisManager->CreateNtupleDColumn("x_global");
analysisManager->CreateNtupleDColumn("y_global");
analysisManager->CreateNtupleDColumn("z_global");

analysisManager->FinishNtuple(0);


}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	G4int runID = run->GetRunID();
	
	std::stringstream strRunID;
	strRunID << runID;
	

	man->OpenFile("/home/sakib/Geant4/output_psc/hallD_psc_with_shield_and_slits_"+strRunID.str()+".root");

	
}



void MyRunAction::EndOfRunAction(const G4Run* run) {
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();	
	man->CloseFile();
	
	G4int runID1 = run->GetRunID();
	
	std::stringstream strRunID1;
	strRunID1 << runID1;
	
   	G4cout << "Run " << run->GetRunID() << " ended." << G4endl;

    	
    

}
 
