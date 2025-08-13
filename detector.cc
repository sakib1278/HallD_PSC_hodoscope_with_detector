#include "detector.hh"
#include "run.hh"
#include "G4SystemOfUnits.hh"
#include <map>
#include <string>
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"



std::map<std::string, G4ThreeVector> volumePositions;

std::map<std::string, int> particleCounter; 
MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	G4Track *track = aStep->GetTrack();
	G4int trackID = track->GetTrackID();       // Unique ID for this track
	G4int parentID = track->GetParentID();
	
	//track->SetTrackStatus(fStopAndKill);
	G4int PDG = track->GetParticleDefinition()->GetPDGEncoding();  // Particle ID
    	G4ThreeVector momentum = track->GetMomentum();
    	G4ThreeVector position = track->GetPosition();
    	
    	
    	G4String particleName = track->GetParticleDefinition()->GetParticleName();
	//MyRunAction::particleCounter[particleName]++;
	
	MyRunAction::particleCounter1[particleName]++;
	
	G4String volumeName = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();

    	// Store which volumes this track has passed through
    	trackHistory[trackID].insert(volumeName);
    	
    	
    	G4ThreeVector posElectron = G4ThreeVector(-9999., -9999., -9999.);
	G4ThreeVector momElectron = G4ThreeVector(-9999., -9999., -9999.);
	G4ThreeVector posPositron = G4ThreeVector(-9999., -9999., -9999.);
	G4ThreeVector momPositron = G4ThreeVector(-9999., -9999., -9999.);
	
	//G4ThreeVector vertexPosition = track->GetVertexPosition();
	G4ThreeVector vertexPosition = G4ThreeVector(-9999., -9999., -9999.);
	G4ThreeVector vertexPos_e = G4ThreeVector(-9999., -9999., -9999.);
	G4ThreeVector vertexPos_p = G4ThreeVector(-9999., -9999., -9999.);

	G4double energyElectron = -5000.0, energyPositron = -5000.0;
	
	if (PDG == 11 || PDG == -11) {
    	const G4VProcess* process = track->GetCreatorProcess();
    	if (process && process->GetProcessName() == "conv") {
         vertexPosition = track->GetVertexPosition();
        
     	}
     	}
   	//if (PDG == 11)vertexPos_e = track->GetVertexPosition();
   	if (PDG == -11)vertexPos_p = track->GetVertexPosition();
	vertexPos_e = track->GetVertexPosition();
	
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	
	//G4ThreeVector posPhoton = preStepPoint->GetPosition();
	
	//G4ThreeVector momPhoton = preStepPoint->GetMomentum();
	/*G4double momentumMag = momPhoton.mag() / GeV;  // Convert to GeV
	G4double posY_low = -499;  // Placeholder for momentum < 5 GeV
	G4double posY_high = -499; //Placeholder for momentum > 5 GeV
	*/

	// Only log electrons and positrons
	G4double totalMomentum_e = -1000;
	G4double totalMomentum_p = -1000;
	
  	//if (vertexPos_e[0]<-2000 && PDG == 11 && trackHistory[trackID].count("MainScintillator1") && trackHistory[trackID].count("MainScintillator1")){
  	 if (PDG == 22 && trackHistory[trackID].count("BigDetector"))	{  
         posElectron = preStepPoint->GetPosition();
         momElectron = preStepPoint->GetMomentum();
         totalMomentum_e = momElectron.mag();
         energyElectron = track->GetTotalEnergy();
         //vertexPosition = track->GetVertexPosition();
         //counter1++;
        }
        
        //if (vertexPos_e[0]<-2000 && PDG == -11 && trackHistory[trackID].count("MainScintillator2") && trackHistory[trackID].count("SmallScintillator2")) { 
         /*if (PDG == -11 && trackHistory[trackID].count("BigDetector")) {  
         posPositron = preStepPoint->GetPosition();
         momPositron = preStepPoint->GetMomentum();
         totalMomentum_p = momPositron.mag();
         energyPositron = track->GetTotalEnergy();
         //vertexPosition = track->GetVertexPosition();
        }*/
        
        if (PDG == -11 && trackHistory[trackID].count("BigDetector")) {
    	auto preStepPoint = aStep->GetPreStepPoint();
    	auto touchable = preStepPoint->GetTouchableHandle();

    	// Get rotation matrix of the current volume (BigDetector)
    	const G4AffineTransform& transform = touchable->GetHistory()->GetTopTransform();

    	//G4RotationMatrix invRot = rot.inverse();

    	// Get global position and momentum
    	G4ThreeVector posGlobal = preStepPoint->GetPosition();
    	G4ThreeVector momGlobal = preStepPoint->GetMomentum();

    	// Transform to local coordinates (detector frame)
    	G4ThreeVector posLocal = transform.TransformPoint(posGlobal);
    	G4ThreeVector momLocal = transform.TransformAxis(momGlobal);

    	// Now store/use posLocal and momLocal
    	posPositron = posLocal;
    	momPositron = momLocal;
   	totalMomentum_p = momLocal.mag();
   	energyPositron = track->GetTotalEnergy();

    // Optional: You can also get vertex position in global coords, transform if needed
    // vertexPosition = rot->inverse() * track->GetVertexPosition();
}
        
        G4double angleIncident = -999.;
	G4double angleIncident_e  = -999.;
	G4double angleIncident_ep = -999.;
	G4double angleIncident_g  = -999.;

	if (trackHistory[trackID].count("BigDetector") && aStep->IsFirstStepInVolume()) {

    // Get global pre-step position
    G4ThreeVector globalPos = preStepPoint->GetPosition();

    // Transform global -> local
    G4AffineTransform transform = preStepPoint->GetTouchableHandle()->GetHistory()->GetTopTransform();
    G4ThreeVector localPos = transform.TransformPoint(globalPos);

    // Get normal in local coordinates
    G4ThreeVector localNormal = preStepPoint->GetTouchableHandle()
        ->GetVolume()
        ->GetLogicalVolume()
        ->GetSolid()
        ->SurfaceNormal(localPos);

    // Transform normal back to global
    G4ThreeVector normal = transform.TransformAxis(localNormal);

        if (normal.dot(preStepPoint->GetMomentumDirection()) < 0) {
    	normal = -normal; // flip normal if it's pointing same way as momentum
	}

    	angleIncident = 90.0 - preStepPoint->GetMomentumDirection()
                                   .angle(normal) * (180.0 / CLHEP::pi);

    	if (PDG == 11)  angleIncident_e  = angleIncident;
    	if (PDG == -11) angleIncident_ep = angleIncident;
    	if (PDG == 22)  angleIncident_g  = angleIncident;
}

        /*if (PDG == 11 && trackHistory[trackID].count("MainScintillator") && trackHistory[trackID].count("SmallScintillator")){
        counter2++;
        }*/
        	
	//G4int nrgPhoton = preStepPoint->GetEnergy();
	//G4double energyDeposit = aStep->GetTotalEnergyDeposit();
//for (auto pv : *G4PhysicalVolumeStore::GetInstance()) G4cout << pv->GetName() << G4endl;

	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4ThreeVector pos3 = G4ThreeVector(-9999., -9999., -9999.);
	G4ThreeVector mom3 = G4ThreeVector(-9999., -9999., -9999.);
	G4double energy3 = -5000.0, total_mom3=-5000.0;
	
	/*G4ThreeVector pos4 = aStep->GetPreStepPoint()->GetPosition();


if (pos4.x() > 230.0*cm) {*/
	
	if (aStep->GetPreStepPoint()->GetTouchableHandle()
        ->GetVolume()->GetName() == "BigDetector" &&
    aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {

    auto track1 = aStep->GetTrack();
    G4String particleName3 = track1->GetParticleDefinition()->GetParticleName();
    pos3 = aStep->GetPreStepPoint()->GetPosition();
    mom3 = aStep->GetPreStepPoint()->GetMomentum();
    energy3 = track1->GetTotalEnergy();
    total_mom3 = mom3.mag();
}

	/*if (trackHistory[trackID].count("BigDetector")) {
        auto track1 = aStep->GetTrack();
        G4String particleName3 = track1->GetParticleDefinition()->GetParticleName();
 	pos3 = aStep->GetPreStepPoint()->GetPosition();
    	mom3 = aStep->GetPreStepPoint()->GetMomentum();
    	energy3 = track->GetTotalEnergy();
    	total_mom3 = mom3.mag();
}*/
	
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	//&& (posElectron[0] > 1300 || posPositron[0] > 1300)
	//if  (((energyElectron > 100 && energyElectron < 8900) || (energyPositron > 100 && energyPositron < 8900)) && (posElectron[0] > 1300 || posPositron[0] > 1300)){
	man->FillNtupleIColumn(0, evt);
	man->FillNtupleDColumn(1, pos3[0]);
	man->FillNtupleDColumn(2, pos3[1]);
	man->FillNtupleDColumn(3, pos3[2]);
	man->FillNtupleDColumn(4, total_mom3);
	//man->FillNtupleDColumn(5, momPhoton[1]);
	//man->FillNtupleDColumn(6, momPhoton[2]);
	man->FillNtupleDColumn(5, totalMomentum_e);
	man->FillNtupleDColumn(6, totalMomentum_p);
	man->FillNtupleDColumn(7, posElectron[0]);
	man->FillNtupleDColumn(8, posElectron[1]);
	man->FillNtupleDColumn(9, posElectron[2]);
	man->FillNtupleDColumn(10, momElectron[0]);
	man->FillNtupleDColumn(11, momElectron[1]);
	man->FillNtupleDColumn(12, momElectron[2]);
	man->FillNtupleDColumn(13, energyElectron);
	man->FillNtupleDColumn(14, posPositron[0]);
	man->FillNtupleDColumn(15, posPositron[1]);
	man->FillNtupleDColumn(16, posPositron[2]);
	man->FillNtupleDColumn(17, momPositron[0]);
	man->FillNtupleDColumn(18, momPositron[1]);
	man->FillNtupleDColumn(19, momPositron[2]);
	man->FillNtupleDColumn(20, energyPositron);
	man->FillNtupleDColumn(21, mom3[0]);
	man->FillNtupleDColumn(22, mom3[1]);
	man->FillNtupleDColumn(23, mom3[2]);
	/*man->FillNtupleDColumn(21, vertexPosition[0]);
	man->FillNtupleDColumn(22, vertexPosition[1]);
	man->FillNtupleDColumn(23, vertexPosition[2]);*/
	man->FillNtupleDColumn(24, vertexPos_e[0]);
	man->FillNtupleDColumn(25, vertexPos_p[0]);
	man->FillNtupleDColumn(26, energy3);  

	//man->FillNtupleDColumn(26, counter1);
	man->FillNtupleDColumn(27, angleIncident);
	man->FillNtupleDColumn(28, angleIncident_e);
	man->FillNtupleDColumn(29, angleIncident_ep);
	man->FillNtupleDColumn(30, angleIncident_g);
	
	man->AddNtupleRow(0);
	
	//man->FillH1(0, momPhoton[0]);
	//man->FillH1(1, energyDeposit);
	
	
	/*if ((trackHistory[trackID].count("MainScintillator1") && trackHistory[trackID].count("SmallScintillator1") && posElectron[0]>1300) || (trackHistory[trackID].count("MainScintillator2") && trackHistory[trackID].count("SmallScintillator2") && posPositron[0]>1300)){
	G4String particleName = track->GetParticleDefinition()->GetParticleName();
	MyRunAction::particleCounter[particleName]++;
	}
	
	if ((trackHistory[trackID].count("MainScintillator1") && trackHistory[trackID].count("SmallScintillator1") && posElectron[0]>1300) || (trackHistory[trackID].count("MainScintillator2") && trackHistory[trackID].count("SmallScintillator2") && posPositron[0]>1300)){
	G4String particleName1 = track->GetParticleDefinition()->GetParticleName();
	if (vertexPos_e[0] < -1500){
	MyRunAction::particleCounter1[particleName1]++;
	}
	}*/
   	 
	//}
	if (trackHistory[trackID].count("BigDetector")){
	G4String particleName = track->GetParticleDefinition()->GetParticleName();
	MyRunAction::particleCounter[particleName]++;
	}
	
	return true;
}

	

