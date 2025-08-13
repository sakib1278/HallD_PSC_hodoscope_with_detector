#include "run.hh"



std::map<std::string, int> MyRunAction::particleCounter = {};
std::map<std::string, int> MyRunAction::particleCounter1 = {};

MyRunAction::MyRunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->CreateNtuple("Hits", "Hits");
	man->CreateNtupleIColumn("Event_Distribution");
	man->CreateNtupleDColumn("pos_x");
	man->CreateNtupleDColumn("pos_y");
	man->CreateNtupleDColumn("pos_z");
	man->CreateNtupleDColumn("Px");
	man->CreateNtupleDColumn("P_total_e");
	man->CreateNtupleDColumn("P_total_p");
	man->CreateNtupleDColumn("posX_e");  
	man->CreateNtupleDColumn("posY_e");
	man->CreateNtupleDColumn("posZ_e");
	man->CreateNtupleDColumn("Px_e");
	man->CreateNtupleDColumn("Py_e");
	man->CreateNtupleDColumn("Pz_e");
	man->CreateNtupleDColumn("E_e");
	man->CreateNtupleDColumn("posX_p");  
	man->CreateNtupleDColumn("posY_p");
	man->CreateNtupleDColumn("posZ_p");
	man->CreateNtupleDColumn("Px_p");
	man->CreateNtupleDColumn("Py_p");
	man->CreateNtupleDColumn("Pz_p");
	man->CreateNtupleDColumn("E_p");
	man->CreateNtupleDColumn("vertexPosition_x");
	man->CreateNtupleDColumn("vertexPosition_y");
	man->CreateNtupleDColumn("vertexPosition_z");
	man->CreateNtupleDColumn("vertexPos_e_x");
	man->CreateNtupleDColumn("vertexPos_p_x");
	man->CreateNtupleDColumn("e_total_detector");
	man->CreateNtupleDColumn("angle_det");
	
	man->CreateNtupleDColumn("angle_e");
	man->CreateNtupleDColumn("angle_p");
	man->CreateNtupleDColumn("angle_gamma");
	
	
	
	/*man->CreateNtupleDColumn("trackID");
	man->CreateNtupleDColumn("parentID");*/
	man->FinishNtuple(0);
	
	
	/*man->CreateH1("px_hist", "Photon Momentum Distribution", 200, 0, 7000);
	man->SetH1XAxisTitle(0, "Energy (MeV)");  // X-axis title
	man->SetH1YAxisTitle(0, "Counts");  // Y-axis title
	man->CreateH1("E_hist", "Photon Energy Distribution", 100, 0, 0.03);*/
	
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	G4int runID = run->GetRunID();
	
	std::stringstream strRunID;
	strRunID << runID;
	
	//man->OpenFile("/home/sakib/Geant4/psc/output_45gev_"+strRunID.str()+".root");
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
    
    	// Print the particle count statistics at the end of the run
    	/*G4cout << "Particle counts hitting the detector:" << G4endl;
    	for (const auto &entry : particleCounter) {
        G4cout << entry.first << ": " << entry.second << G4endl;*/
        std::ofstream outfile("/home/sakib/Geant4/outputz_psc/particle_counts_det5_check2"+strRunID1.str()+".txt");
        //outfile <<  "All Particles going through both scintillators: "  << std::endl;
	outfile <<  "All Particles going through Detector 5: "  << std::endl;
	for (const auto &entry : particleCounter) {
   	 outfile << entry.first << ": " << entry.second << std::endl;
   	 //outfile << "electrons from vertex: " << counter1 << std::endl;
   	 //outfile << "electrons in total: " << counter2 << std::endl;
	}
	
	outfile.close();
	
	std::ofstream outfile1("/home/sakib/Geant4/output_psc/particle_count_scints_check2"+strRunID1.str()+".txt");
	outfile1 <<  "Particles with vertex <-1500 going through both scintillators: "  << std::endl;
	for (const auto &entry : particleCounter1) {
   	 outfile1 << entry.first << ": " << entry.second << std::endl;
	}
	outfile1.close();
	

}
 
