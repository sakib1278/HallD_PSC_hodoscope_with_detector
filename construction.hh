#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "detector.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4Trap.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4Material.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4ExtrudedSolid.hh"
//#include "MySensitiveDetector.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();
	
	virtual G4VPhysicalVolume *Construct();
	void ConstructScintillator();
	
private:
	G4LogicalVolume *logicDetector;
	virtual void ConstructSDandField();
	
	G4int nCols, nRows;
	G4Trap *vacuumChamber;
	G4Box *solidWorld, *solidRadiator, *solidDetector, *mainScintillatorSolid, *smallScintillatorSolid, *converterBox, *magnetBox, *detectorSolid;
	G4LogicalVolume *logicWorld, *logicRadiator, *mainScintillatorLogical, *smallScintillatorLogical, *converterLV, *vacuumChamberLV, *magnetLV, *detectorLogical;
	G4VPhysicalVolume *physWorld, *physRadiator, *physDetector, *mainScintillatorPhysical, *smallScintillatorPhysical, *physConverter, *physChamber, *physMagnet, *BigDetectorPhysical;

	G4GenericMessenger *fMessenger;
	
	G4Material *SiO2, *H2O, *Aerogel, *worldMat;
	G4Material *EJ212, *vacuum, *aluminum, *beryllium;
	G4Element *C;
	
	void DefineMaterial();
	//void ConstructScintillator();
	void SetVisualizationAttributes(G4LogicalVolume* logVol, G4String name, G4Colour colour);
};

#endif
