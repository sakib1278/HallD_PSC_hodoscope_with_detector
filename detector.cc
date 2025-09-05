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

}

	

