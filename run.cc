#include "run.hh"



std::map<std::string, int> MyRunAction::particleCounter = {};
std::map<std::string, int> MyRunAction::particleCounter1 = {};

MyRunAction::MyRunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->CreateNtuple("Hits", "Hits");
	man->CreateNtupleIColumn("Event_Distribution");
	man->CreateNtupleDColumn("posX_e");  
	man->CreateNtupleDColumn("posY_e");
	man->CreateNtupleDColumn("posZ_e");
	man->CreateNtupleDColumn("Px_e");
	man->CreateNtupleDColumn("Py_e");
	man->CreateNtupleDColumn("Pz_e");
	man->CreateNtupleDColumn("P_total_e");
	man->CreateNtupleDColumn("E_e");
	
	man->CreateNtupleDColumn("posX_p");  
	man->CreateNtupleDColumn("posY_p");
	man->CreateNtupleDColumn("posZ_p");
	man->CreateNtupleDColumn("Px_p");
	man->CreateNtupleDColumn("Py_p");
	man->CreateNtupleDColumn("Pz_p");
	man->CreateNtupleDColumn("P_total_p");
	man->CreateNtupleDColumn("E_p");
	
	man->CreateNtupleDColumn("posX_photon");
	man->CreateNtupleDColumn("posY_photon");
	man->CreateNtupleDColumn("posZ_photon");
	man->CreateNtupleDColumn("Px_photon");
	man->CreateNtupleDColumn("Py_photon");
	man->CreateNtupleDColumn("Pz_photon");
	man->CreateNtupleDColumn("P_total_photon");
	man->CreateNtupleDColumn("E_photon");
	
	man->CreateNtupleDColumn("angle_all_particles");
	man->CreateNtupleDColumn("angle_e");
	man->CreateNtupleDColumn("angle_p");
	man->CreateNtupleDColumn("angle_photon");
	
	man->CreateNtupleDColumn("vertexPositionX_e");
	man->CreateNtupleDColumn("vertexPositionY_e");
	man->CreateNtupleDColumn("vertexPositionZ_e");
		

	man->FinishNtuple(0);
	
	
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	G4int runID = run->GetRunID();
	
	std::stringstream strRunID;
	strRunID << runID;
	

	man->OpenFile("/home/sakib/Geant4/output_psc/hallD_check_vertex_"+strRunID.str()+".root");
	
}



void MyRunAction::EndOfRunAction(const G4Run* run) {
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();	
	man->CloseFile();
	
	G4int runID1 = run->GetRunID();
	
	std::stringstream strRunID1;
	strRunID1 << runID1;
	
   	 G4cout << "Run " << run->GetRunID() << " ended." << G4endl;
    
    	
    std::ofstream outfile("/home/sakib/Geant4/output_psc/particle_counts_det5_check2"+strRunID1.str()+".txt");
       
	outfile <<  "All Particles going through Detector 5: "  << std::endl;
	for (const auto &entry : particleCounter) {
   	 outfile << entry.first << ": " << entry.second << std::endl;
	}
	
	outfile.close();
	
}
 
