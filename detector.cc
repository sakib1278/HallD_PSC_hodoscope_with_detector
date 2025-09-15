#include "detector.hh"
#include "construction.hh"
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
#include <cmath> 

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

   
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
    
    // Get BigDetectorPhysical from the construction
    auto detConstruction = static_cast<const MyDetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction()
);
G4VPhysicalVolume* bigPV = detConstruction->GetBigDetectorPhysical();

G4ThreeVector placementTranslation = bigPV->GetObjectTranslation();
G4RotationMatrix* placementRotation = bigPV->GetObjectRotation();

G4AffineTransform localToWorld(placementRotation, placementTranslation);

// Local origin in BigDetector coordinates
G4ThreeVector localOrigin(0., 0., 0.);

// Transform it to world coordinates
G4ThreeVector globalOrigin = localToWorld.TransformPoint(localOrigin);

G4AffineTransform worldToLocal = localToWorld.Inverse();

/*G4AffineTransform worldToLocal(rotCW1, G4ThreeVector(0.0, -43.0*cm, 537*cm));
worldToLocal.Invert();
G4ThreeVector xLocal = worldToLocal.TransformPoint(xGlobal);
G4cout << "Local hit: " << xLocal/cm << " cm" << G4endl;*/


    // --- particle-specific handling ---
    auto analysisManager = G4AnalysisManager::Instance();

     
     G4ThreeVector xLocal = worldToLocal.TransformPoint(xGlobal);
     G4ThreeVector pLocal = worldToLocal.TransformAxis(pGlobal);
   
     G4ThreeVector xBack = localToWorld.TransformPoint(xLocal);

G4cout << "World hit = " << xGlobal/cm << " cm" << G4endl;
G4cout << "Local hit = " << xLocal/cm << " cm" << G4endl;
G4cout << "Back-converted = " << xBack/cm << " cm" << G4endl;
     //G4double energy = energy;
     
     	G4VSolid* solid = pre->GetTouchableHandle()
                    ->GetVolume()->GetLogicalVolume()->GetSolid();

	G4ThreeVector localNormal = solid->SurfaceNormal(xLocal);

     	G4double thetaInc = 90 - pLocal.unit().angle(localNormal) * 180.0 / CLHEP::pi;
     	thetaInc = std::abs(thetaInc);

   
        analysisManager->FillNtupleDColumn(0, xLocal.x()/cm);
        analysisManager->FillNtupleDColumn(1, xLocal.y()/cm);
        analysisManager->FillNtupleDColumn(2, xLocal.z()/cm);
        analysisManager->FillNtupleDColumn(3, pLocal.x()/MeV);
        analysisManager->FillNtupleDColumn(4, pLocal.y()/MeV);
        analysisManager->FillNtupleDColumn(5, pLocal.z()/MeV);
        analysisManager->FillNtupleDColumn(6, pLocal.mag()/MeV);
        analysisManager->FillNtupleDColumn(7, energy/MeV);
        analysisManager->FillNtupleDColumn(8, thetaInc);
        
    	analysisManager->FillNtupleDColumn(9, static_cast<double>(pdg));
    	
    	analysisManager->FillNtupleDColumn(10, xGlobal.x()/cm);
    	analysisManager->FillNtupleDColumn(11, xGlobal.y()/cm);
    	analysisManager->FillNtupleDColumn(12, xGlobal.x()/cm);
    	analysisManager->AddNtupleRow(0);
    
    return true;

}

	

