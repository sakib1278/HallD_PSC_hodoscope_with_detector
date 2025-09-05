#include "detector.hh"
#include "run.hh"
#include "G4SystemOfUnits.hh"
#include <map>
#include <string>
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4TouchableHistory.hh"
#include "G4AffineTransform.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Units/PhysicalConstants.h"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}
<<<<<<< HEAD

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
   	
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	

	// Only log electrons and positrons
	G4double totalMomentum_e = -10000;
	G4double totalMomentum_p = -10000;
	
  	if (PDG == 11 && aStep->IsFirstStepInVolume()) {
    	auto volumeName = preStepPoint->GetTouchableHandle()
                                     ->GetVolume()
                                     ->GetName();

   	 if (volumeName == "BigDetector") {
    	 posElectron = preStepPoint->GetPosition();
         momElectron = preStepPoint->GetMomentum();
         totalMomentum_e = momElectron.mag();
         energyElectron = track->GetTotalEnergy();
         vertexPosition = track->GetVertexPosition();
         
        }
        }
        
        if (PDG == -11 && aStep->IsFirstStepInVolume()) {
    	auto volumeName = preStepPoint->GetTouchableHandle()
                                     ->GetVolume()
                                     ->GetName();

   	 if (volumeName == "BigDetector") {
    	 posPositron = preStepPoint->GetPosition();
         momPositron = preStepPoint->GetMomentum();
         totalMomentum_p = momPositron.mag();
         energyPositron = track->GetTotalEnergy();
         //vertexPosition = track->GetVertexPosition();
        }
}
         
        
    G4double angleIncident = -999.;
	G4double angleIncident_e  = -999.;
	G4double angleIncident_ep = -999.;
	G4double angleIncident_g  = -999.;

	
	if (aStep->IsFirstStepInVolume()) {
    	auto volumeName = preStepPoint->GetTouchableHandle()
                                     ->GetVolume()
                                     ->GetName();

   	 if (volumeName == "BigDetector") {

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
    	normal = -normal; // flip normal if it's pointing same way as momentum, avoiding negative angle
	}

    	angleIncident = 90.0 - preStepPoint->GetMomentumDirection()
                                   .angle(normal) * (180.0 / CLHEP::pi);

    	if (PDG == 11)  angleIncident_e  = angleIncident;
    	if (PDG == -11) angleIncident_ep = angleIncident;
    	if (PDG == 22)  angleIncident_g  = angleIncident;
    	}
}

      
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	G4ThreeVector posPhoton = G4ThreeVector(-9999., -9999., -9999.);
	G4ThreeVector momPhoton = G4ThreeVector(-9999., -9999., -9999.);
	G4double energyPhoton = -5000.0, total_momPhoton=-5000.0;
	
	
    if (PDG == 22 && aStep->IsFirstStepInVolume()) {
    	auto volumeName = preStepPoint->GetTouchableHandle()
                                     ->GetVolume()
                                     ->GetName();

   	 if (volumeName == "BigDetector") {
    	 auto track1 = aStep->GetTrack();
        G4String particleName3 = track1->GetParticleDefinition()->GetParticleName();
 	posPhoton = aStep->GetPreStepPoint()->GetPosition();
    	momPhoton = aStep->GetPreStepPoint()->GetMomentum();
    	energyPhoton = track->GetTotalEnergy();
    	total_momPhoton = momPhoton.mag();
    	}
}
	
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->FillNtupleIColumn(0, evt);
	
	man->FillNtupleDColumn(1, posElectron[0]);
	man->FillNtupleDColumn(2, posElectron[1]);
	man->FillNtupleDColumn(3, posElectron[2]);
	man->FillNtupleDColumn(4, momElectron[0]);
	man->FillNtupleDColumn(5, momElectron[1]);
	man->FillNtupleDColumn(6, momElectron[2]);
	man->FillNtupleDColumn(7, totalMomentum_e);
	man->FillNtupleDColumn(8, energyElectron);
	
	man->FillNtupleDColumn(9, posPositron[0]);
	man->FillNtupleDColumn(10, posPositron[1]);
	man->FillNtupleDColumn(11, posPositron[2]);
	man->FillNtupleDColumn(12, momPositron[0]);
	man->FillNtupleDColumn(13, momPositron[1]);
	man->FillNtupleDColumn(14, momPositron[2]);
	man->FillNtupleDColumn(15, totalMomentum_p);
	man->FillNtupleDColumn(16, energyPositron);
	
	man->FillNtupleDColumn(17, posPhoton[0]);
	man->FillNtupleDColumn(18, posPhoton[1]);
	man->FillNtupleDColumn(19, posPhoton[2]);
	man->FillNtupleDColumn(20, momPhoton[0]);
	man->FillNtupleDColumn(21, momPhoton[1]);
	man->FillNtupleDColumn(22, momPhoton[2]);
	man->FillNtupleDColumn(23, total_momPhoton);
	man->FillNtupleDColumn(24, energyPhoton);
	
	man->FillNtupleDColumn(25, angleIncident);
	man->FillNtupleDColumn(26, angleIncident_e);
	man->FillNtupleDColumn(27, angleIncident_ep);
	man->FillNtupleDColumn(28, angleIncident_g);
	
	man->FillNtupleDColumn(29, vertexPosition[0]);
	man->FillNtupleDColumn(30, vertexPosition[1]);
	man->FillNtupleDColumn(31, vertexPosition[2]);
	
	man->AddNtupleRow(0);
	
	
	if (aStep->IsFirstStepInVolume()) {
    auto volumeName = preStepPoint->GetTouchableHandle()
                                     ->GetVolume()
                                     ->GetName();

   	 if (volumeName == "BigDetector") {
	G4String particleName = track->GetParticleDefinition()->GetParticleName();
	MyRunAction::particleCounter[particleName]++;
	}
	}
	return true;
=======
   
G4bool MySensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *ROhist) {
    auto pre = step->GetPreStepPoint();
    //if (pre->GetStepStatus() != fGeomBoundary) return false;

    G4String volName = pre->GetTouchableHandle()->GetVolume()->GetName();
    if (volName != "BigDetector") return false;

    G4Track* track = step->GetTrack();
    G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
   
    G4ThreeVector xGlobal = pre->GetPosition();
    G4ThreeVector pGlobal = pre->GetMomentum();
    G4double energy = track->GetTotalEnergy();
    
    // --- shared transforms ---
    auto touch = pre->GetTouchableHandle();
    const G4AffineTransform& localToWorld = touch->GetHistory()->GetTopTransform();
    G4AffineTransform worldToLocal = localToWorld;
    worldToLocal.Invert();
	 
    // --- particle-specific handling ---
    auto analysisManager = G4AnalysisManager::Instance();

     // electron
       if (pdg == 11) {
     G4ThreeVector xLocalE = worldToLocal.TransformPoint(xGlobal);
     G4ThreeVector pLocalE = worldToLocal.TransformAxis(pGlobal);
     G4double energyE = energy;
     
     	G4VSolid* solid = pre->GetTouchableHandle()
                    ->GetVolume()->GetLogicalVolume()->GetSolid();

	G4ThreeVector localNormal = solid->SurfaceNormal(xLocalE);

     	G4double thetaIncE = 90 - pLocalE.unit().angle(localNormal) * 180.0 / CLHEP::pi;
   
        analysisManager->FillNtupleDColumn(0, xLocalE.x()/cm);
        analysisManager->FillNtupleDColumn(1, xLocalE.y()/cm);
        analysisManager->FillNtupleDColumn(2, xLocalE.z()/cm);
        analysisManager->FillNtupleDColumn(3, pLocalE.x()/MeV);
        analysisManager->FillNtupleDColumn(4, pLocalE.y()/MeV);
        analysisManager->FillNtupleDColumn(5, pLocalE.z()/MeV);
        analysisManager->FillNtupleDColumn(6, pLocalE.mag()/MeV);
        analysisManager->FillNtupleDColumn(7, energyE/MeV);
        analysisManager->FillNtupleDColumn(8, thetaIncE);
          
	}
    // positron
    if (pdg == -11) {
     G4ThreeVector xLocalP = worldToLocal.TransformPoint(xGlobal);
     G4ThreeVector pLocalP = worldToLocal.TransformAxis(pGlobal);
     G4double energyP = energy;
     
     G4VSolid* solid = pre->GetTouchableHandle()
                    ->GetVolume()->GetLogicalVolume()->GetSolid();

	G4ThreeVector localNormal = solid->SurfaceNormal(xLocalP);

     
     G4double thetaIncP = 90 - pLocalP.unit().angle(localNormal) * 180.0 / CLHEP::pi;
    
        analysisManager->FillNtupleDColumn(9, xLocalP.x()/cm);
        analysisManager->FillNtupleDColumn(10, xLocalP.y()/cm);
        analysisManager->FillNtupleDColumn(11, xLocalP.z()/cm);
        analysisManager->FillNtupleDColumn(12, pLocalP.x()/MeV);
        analysisManager->FillNtupleDColumn(13, pLocalP.y()/MeV);
        analysisManager->FillNtupleDColumn(14, pLocalP.z()/MeV);
        analysisManager->FillNtupleDColumn(15, pLocalP.mag()/MeV);
        analysisManager->FillNtupleDColumn(16, energyP/MeV);
        analysisManager->FillNtupleDColumn(17, thetaIncP);
        
  }
    // photon
    if (pdg == 22) {
     G4ThreeVector xLocalG = worldToLocal.TransformPoint(xGlobal);
     G4ThreeVector pLocalG = worldToLocal.TransformAxis(pGlobal);
     G4double energyG = energy;
     
     G4VSolid* solid = pre->GetTouchableHandle()
                    ->GetVolume()->GetLogicalVolume()->GetSolid();

	G4ThreeVector localNormal = solid->SurfaceNormal(xLocalG);

     G4double thetaIncG = 90 - pLocalG.unit().angle(localNormal) * 180.0 / CLHEP::pi;
    
        analysisManager->FillNtupleDColumn(18, xLocalG.x()/cm);
        analysisManager->FillNtupleDColumn(19, xLocalG.y()/cm);
        analysisManager->FillNtupleDColumn(20, xLocalG.z()/cm);
        analysisManager->FillNtupleDColumn(21, pLocalG.x()/MeV);
        analysisManager->FillNtupleDColumn(22, pLocalG.y()/MeV);
        analysisManager->FillNtupleDColumn(23, pLocalG.z()/MeV);
        analysisManager->FillNtupleDColumn(24, pLocalG.mag()/MeV);
        analysisManager->FillNtupleDColumn(25, energyG/MeV);
        analysisManager->FillNtupleDColumn(26, thetaIncG);
        
    }

    analysisManager->FillNtupleDColumn(27, static_cast<double>(pdg));
    analysisManager->AddNtupleRow(0);
    
    return true;
>>>>>>> Update: describe what you changed
}

	

