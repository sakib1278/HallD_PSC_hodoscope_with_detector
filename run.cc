#include "run.hh"



std::map<std::string, int> MyRunAction::particleCounter = {};
std::map<std::string, int> MyRunAction::particleCounter1 = {};

MyRunAction::MyRunAction()
{
<<<<<<< HEAD
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

	
=======
<<<<<<< HEAD
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
	
	
=======
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

	
>>>>>>> 0d3191cbba556d7ceb93aefb9e9c38733b65d6f3
	
>>>>>>> Update: describe what you changed
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	G4int runID = run->GetRunID();
	
	std::stringstream strRunID;
	strRunID << runID;
	
<<<<<<< HEAD
	man->OpenFile("/home/sakib/Geant4/output_psc/hallD_psc_with_shield_"+strRunID.str()+".root");
=======
<<<<<<< HEAD

	man->OpenFile("/home/sakib/Geant4/output_psc/hallD_check_vertex_"+strRunID.str()+".root");
=======
	man->OpenFile("/home/sakib/Geant4/output_psc/hallD_psc_with_shield_"+strRunID.str()+".root");
>>>>>>> Update: describe what you changed
>>>>>>> 0d3191cbba556d7ceb93aefb9e9c38733b65d6f3
	
}



void MyRunAction::EndOfRunAction(const G4Run* run) {
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();	
	man->CloseFile();
	
	G4int runID1 = run->GetRunID();
	
	std::stringstream strRunID1;
	strRunID1 << runID1;
	
   	G4cout << "Run " << run->GetRunID() << " ended." << G4endl;
<<<<<<< HEAD
    
    
=======
    
    
<<<<<<< HEAD
    	
    std::ofstream outfile("/home/sakib/Geant4/output_psc/particle_counts_det5_check2"+strRunID1.str()+".txt");
       
	outfile <<  "All Particles going through Detector 5: "  << std::endl;
	for (const auto &entry : particleCounter) {
   	 outfile << entry.first << ": " << entry.second << std::endl;
	}
	
	outfile.close();
	
=======
>>>>>>> 0d3191cbba556d7ceb93aefb9e9c38733b65d6f3

>>>>>>> Update: describe what you changed
}
 
