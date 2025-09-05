#include "run.hh"



std::map<std::string, int> MyRunAction::particleCounter = {};
std::map<std::string, int> MyRunAction::particleCounter1 = {};

MyRunAction::MyRunAction()
{

auto analysisManager = G4AnalysisManager::Instance();

// One ntuple per particle type
analysisManager->CreateNtuple("Hits", "Local coords and momentum");
analysisManager->CreateNtupleDColumn("x_electron_cm");
analysisManager->CreateNtupleDColumn("y_electron_cm");
analysisManager->CreateNtupleDColumn("z_electron_cm");
analysisManager->CreateNtupleDColumn("Px_electron_MeV");
analysisManager->CreateNtupleDColumn("Py_electron_MeV");
analysisManager->CreateNtupleDColumn("Pz_electron_MeV");
analysisManager->CreateNtupleDColumn("P_total_electron_MeV");
analysisManager->CreateNtupleDColumn("Energy_electron_MeV");
analysisManager->CreateNtupleDColumn("angle_electron_deg");
//analysisManager->FinishNtuple(0);

//analysisManager->CreateNtuple("Electrons", "Local coords and momentum");
analysisManager->CreateNtupleDColumn("x_positron_cm");
analysisManager->CreateNtupleDColumn("y_positron_cm");
analysisManager->CreateNtupleDColumn("z_positron_cm");
analysisManager->CreateNtupleDColumn("Px_positron_MeV");
analysisManager->CreateNtupleDColumn("Py_positron_MeV");
analysisManager->CreateNtupleDColumn("Pz_positron_MeV");
analysisManager->CreateNtupleDColumn("P_total_positron_MeV");
analysisManager->CreateNtupleDColumn("Energy_positron_MeV");
analysisManager->CreateNtupleDColumn("angle_positron_deg");
//analysisManager->FinishNtuple(1);

//analysisManager->CreateNtuple("Gammas", "Local coords and momentum");
analysisManager->CreateNtupleDColumn("x_gamma_cm");
analysisManager->CreateNtupleDColumn("y_gamma_cm");
analysisManager->CreateNtupleDColumn("z_gamma_cm");
analysisManager->CreateNtupleDColumn("Px_gamma_MeV");
analysisManager->CreateNtupleDColumn("Py_gamma_MeV");
analysisManager->CreateNtupleDColumn("Pz_gamma_MeV");
analysisManager->CreateNtupleDColumn("P_total_gamma_MeV");
analysisManager->CreateNtupleDColumn("Energy_gamma_MeV");
analysisManager->CreateNtupleDColumn("angle_gamma_deg");
//analysisManager->FinishNtuple(2);

analysisManager->CreateNtupleDColumn("PID");
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
	

	man->OpenFile("/home/sakib/Geant4/output_psc/hallD_psc_with_shield_"+strRunID.str()+".root");

	
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
 
