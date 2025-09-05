#include "construction.hh"
//#include "MySensitiveDetector.hh"
#include "detector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4SubtractionSolid.hh"
#include "G4ProductionCuts.hh"
#include "G4Region.hh"



MyDetectorConstruction::MyDetectorConstruction()
{
	DefineMaterial();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        aluminum = nist->FindOrBuildMaterial("G4_Al");
	beryllium = nist->FindOrBuildMaterial("G4_Be");
	
	
	G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
	G4double rindexWorld[2] = {1.0, 1.0};
	worldMat = nist->FindOrBuildMaterial("G4_AIR");
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	
	// Define the material for EJ-212 (based on polyvinyl toluene)
	G4double density = 1.023 * g / cm3;
	EJ212 = new G4Material("EJ212", density, 2);
	EJ212->AddElement(nist->FindOrBuildElement("C"), 9);
	EJ212->AddElement(nist->FindOrBuildElement("H"), 10);

	// Add optical properties
	G4MaterialPropertiesTable* EJ212MPT = new G4MaterialPropertiesTable();
	
	G4OpticalSurface* opticalSurface = new G4OpticalSurface("OpticalSurface");
	opticalSurface->SetType(dielectric_dielectric);
	opticalSurface->SetModel(unified);
	opticalSurface->SetFinish(ground);

	// Scintillation properties (approximate values, adjust as needed)
	const G4int numEntries = 13;
	G4double photonEnergy[numEntries] = { 
	3.102 * eV, // 400 nm
    	3.026 * eV, // 410 nm
    	2.954 * eV, // 420 nm
    	2.885 * eV, // 430 nm
    	2.820 * eV, // 440 nm
    	2.757 * eV, // 450 nm
    	2.697 * eV, // 460 nm
    	2.540 * eV, // 470 nm
    	2.585 * eV, // 480 nm
    	2.532 * eV, // 490 nm
    	2.481 * eV, // 500 nm
    	2.433 * eV, // 510 nm
    	2.386 * eV  // 520 nm 
    	};
	G4double scintillationYield[numEntries] = { 
	0.2,  // 400 nm
    	0.4,  // 410 nm
    	1.0,  // 420 nm
    	0.75,  // 430 nm
    	0.7,  // 440 nm
    	0.54,  // 450 nm
    	0.38,  // 460 nm
    	0.24,  // 470 nm
    	0.2,  // 480 nm
    	0.17,  // 490 nm
    	0.13, // 500 nm
    	0.95, // 510 nm
    	0.08  // 520 nm
    	};

	// Light yield
	EJ212MPT->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintillationYield, numEntries);
	EJ212MPT->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
	EJ212MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
	EJ212MPT->AddConstProperty("FASTTIMECONSTANT", 2.4 * ns);

	// Set the refractive index
	G4double refractiveIndex[numEntries] = {1.58, 1.58 };
	EJ212MPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, numEntries);

	// Set absorption length
	G4double absorptionLength[numEntries] = { 
	400 * nm,
    	410 * nm,
    	420 * nm,
    	430 * nm,
    	440 * nm,
    	450 * nm,
    	460 * nm,
    	470 * nm,
    	480 * nm,
    	490 * nm,
    	500 * nm,
    	510 * nm,
    	520 * nm };
	EJ212MPT->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, numEntries);

	EJ212->SetMaterialPropertiesTable(EJ212MPT);
	
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	//World
	solidWorld = new G4Box("solidWorld", 2.5*m, 2.10*m, 6.50*m);
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	
	//Detector
	/*solidDetector = new G4Box("solidDetector", 2.0*m, 2.0*m, 3.*cm);
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
	physDetector = new G4PVPlacement(0, G4ThreeVector(0.0*m, 0, 4.0*m), logicDetector, "physDetector", logicWorld, false, 0, true);*/
	
	detectorSolid = new G4Box("DetectorSolid", 25.0 * cm, 25.0 * cm, 0.50 * cm);
	detectorLogical = new G4LogicalVolume(detectorSolid, vacuum, "DetectorLogical");
	

	G4RotationMatrix* rotCW1 = new G4RotationMatrix();
	rotCW1->rotateX(-7.0 * deg);
	
	BigDetectorPhysical= new G4PVPlacement(rotCW1,
		  G4ThreeVector(0.0, -43.0 * cm, 5.37 * m),
                  detectorLogical,
                  "BigDetector",
                  logicWorld,
                  false,
                  0,
                  true);
        
        
	// Construct Scintillator
	ConstructScintillator();
	
	ConstructSDandField();
	
	
	  // Converter (for photon interaction)
        converterBox = new G4Box("Converter", 3.0 * cm, 7.0 * cm, 37.50 * um);
        converterLV = new G4LogicalVolume(converterBox, beryllium, "Converter");
        G4VisAttributes* converterVis = new G4VisAttributes(G4Colour::Red());
		converterVis->SetForceSolid(true);
		converterLV->SetVisAttributes(converterVis);
        physConverter = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0 * m), converterLV, "Converter", logicWorld, false, 0, true);



        // Dipole Magnet (outside vacuum chamber)
        magnetBox = new G4Box("Magnet", 0.05 * m, 0.5 * m, 0.45 * m);
        magnetLV = new G4LogicalVolume(magnetBox, worldMat, "Magnet");
        physMagnet = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 1.02 * m), magnetLV, "Magnet", logicWorld, false, 0, true);

        // Magnetic Field in Dipole Magnet
        G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(-1.662 * tesla, 0, 0));
        G4FieldManager* fieldManager = new G4FieldManager(magField);
        magnetLV->SetFieldManager(fieldManager, true);
        
        

	// Define Trapezoidal Vacuum Chamber Cross-section
	std::vector<G4TwoVector> trapezoidPoints = {
    	G4TwoVector(-0.75 * m, -0.20 * m),  // Bottom-left
    	G4TwoVector(-0.75 * m, 0.20 * m),  // Bottom-right
    	G4TwoVector(0.75 * m, 0.50 * m),  // Top-right (wider part)
    	G4TwoVector(0.75 * m, -0.50 * m)   // Top-left (wider part)
	};

	// Set extrusion depth along Z-axis
	G4double halfDepth = 10.0 * cm / 2.0;

	G4ExtrudedSolid* vacuumChamberShape = new G4ExtrudedSolid("VacuumChamber", trapezoidPoints, halfDepth, 		G4TwoVector(0,0), 1.0, G4TwoVector(0,0), 1.0);

	// Create Logical Volume
	vacuumChamberLV = new G4LogicalVolume(vacuumChamberShape, vacuum, "VacuumChamber");

	// Rotate by 90 degrees around X to align along Y
	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateY(90 * deg);
	
	// Place inside World
	new G4PVPlacement(rotation, G4ThreeVector(0, 0, 2.22 * m), vacuumChamberLV, "VacuumChamber", logicWorld, false, 0);

	// Shield dimensions
G4NistManager* nist = G4NistManager::Instance();
// Define absorbing material (kills particles)
G4Material* BlackHole = new G4Material("BlackHole", 1., 1.*g/mole, 1.*g/cm3);

	// Low-Z front layer
G4Box* frontPolyBox = new G4Box("FrontPoly", 1.0*m, 1.0*m, 5.0*cm);
//G4LogicalVolume* frontPolyLV = new G4LogicalVolume(frontPolyBox, polyethylene, "FrontPolyLV");


// Create slits using subtraction
G4Box* slit1 = new G4Box("Slit1", 1.0*cm, 16.0*cm, 10.0*cm);
G4Box* slit2 = new G4Box("Slit2", 1.0*cm, 16.0*cm, 10.0*cm);

G4ThreeVector slitPos1(0, 42*cm, 0);
G4ThreeVector slitPos2(0, -42*cm, 0);

G4SubtractionSolid* frontWithSlits = new G4SubtractionSolid("FrontWithSlits", frontPolyBox, slit1, 0, slitPos1);
frontWithSlits = new G4SubtractionSolid("FrontWithSlits2", frontWithSlits, slit2, 0, slitPos2);
G4LogicalVolume* frontLVwithSlits = new G4LogicalVolume(frontWithSlits, BlackHole, "FrontLVwithSlits");

new G4PVPlacement(0, G4ThreeVector(0,0,500*cm - 25*cm), frontLVwithSlits, "FrontPoly", logicWorld, false, 0);

	return physWorld;
	
}

void MyDetectorConstruction::ConstructScintillator()
{
	G4RotationMatrix* rotCCW = new G4RotationMatrix();
	rotCCW->rotateX(-5.0 * deg);

	G4RotationMatrix* rotCW = new G4RotationMatrix();
	rotCW->rotateX(+5.0 * deg);


	// Define the large scintillator box
	mainScintillatorSolid = new G4Box("MainScintillator", 1.50*cm, 12.50*cm, 0.50*cm);
	mainScintillatorLogical = new G4LogicalVolume(mainScintillatorSolid, EJ212, "MainScintillatorLogical");
	
	
	// Physical placement
	mainScintillatorPhysical = new G4PVPlacement(rotCW,                   // No rotation
                      			G4ThreeVector(0.*cm, 33*cm, 397.0*cm),  // Center position
                      			mainScintillatorLogical, // Logical volume
                      			"MainScintillator1",      // Name
                      			logicWorld,            // Mother volume
                      			false,                   // No boolean operation
                      			0,                       // Copy number
                      			true);                   // Check overlaps
                      			
        mainScintillatorPhysical = new G4PVPlacement(rotCCW,                   // No rotation
                      			G4ThreeVector(0, -33*cm, 397.*cm),  // Center position
                      			mainScintillatorLogical, // Logical volume
                      			"MainScintillator2",      // Name
                      			logicWorld,            // Mother volume
                      			false,                   // No boolean operation
                      			0,                       // Copy number
                      			true);                   // Check overlaps



	
	// Define the small scintillator box
	smallScintillatorSolid = new G4Box("SmallScintillator", 2.20*cm, 3.0*cm, 1.0*cm);
	smallScintillatorLogical = new G4LogicalVolume(smallScintillatorSolid, EJ212, "SmallScintillatorLogical");
	
	G4Box* wrapperBox = new G4Box("Wrapper", 0.05 * m, 0.32 * m, 0.055 * m);
	G4LogicalVolume* wrapperLV = new G4LogicalVolume(wrapperBox, vacuum, "Wrapper");
	
	
	// Placement loop for small scintillators
	G4double spacing = 12.0 * cm; // Distance between blocks
	G4int numBlocks = 4;         // Number of blocks in the array

	for (G4int i = 0; i < numBlocks; i++) 
	{
   	G4double posY = (i - numBlocks / 2) * spacing + 9.0*cm; // Adjust for alignment
    G4double posX = 0.0*cm;
	G4double posZ = 0.0 * cm;
	
	// Physical placement
	smallScintillatorPhysical = new G4PVPlacement(0, 
                      //G4ThreeVector(posX+1*m, posY + 35*cm, posZ),
                      G4ThreeVector(posX, posY, posZ),  
                      smallScintillatorLogical,
                      "SmallScintillator1",
                      //logicWorld, 
                      wrapperLV,
                      false, 
                      i, // Unique copy number
                      true);
	}
	
	for (G4int j = 0; j < 4; j++) 
	{
   	G4double posY = (j - numBlocks / 2) * spacing + 3.0*cm; // Adjust for alignment
    G4double posX = 0.0*cm;
	G4double posZ = 02.0 * cm;
    	
	// Physical placement
	smallScintillatorPhysical = new G4PVPlacement(0, 
                      //G4ThreeVector(posX+1*m, posY+ 35*cm, posZ), 
                      G4ThreeVector(posX, posY, posZ),
                      smallScintillatorLogical, 
                      "SmallScintillator1",
                      //logicWorld, 
                      wrapperLV,
                      false, 
                      j, // Unique copy number
                      true);
	}
	new G4PVPlacement(
    			rotCW,                             // No rotation inside wrapper
   			G4ThreeVector(0, 34*cm, 438*cm),              // Centered
    			wrapperLV, 
    			"ScintInsideWrapper",
    			logicWorld,
    			false,
    			0,
    			true
			);
	
	wrapperLV->SetVisAttributes(G4VisAttributes::GetInvisible());
	
	G4LogicalVolume* wrapperLV1 = new G4LogicalVolume(wrapperBox, vacuum, "Wrapper1");
	for (G4int k = 0; k < numBlocks; k++) 
	{
   	G4double posY = -((k - numBlocks / 2) * spacing + 9.0*cm); // Adjust for alignment
    	G4double posX = 0.0*cm;
   	G4double posZ = 0.0 * cm;
   	
	// Physical placement
	smallScintillatorPhysical = new G4PVPlacement(0, 
                      G4ThreeVector(posX, -posY, posZ),
                      smallScintillatorLogical, 
                      "SmallScintillator2", 
                      wrapperLV1, 
                      false, 
                      k, // Unique copy number
                      true);
	}
	
	for (G4int l = 0; l < 4; l++) 
	{
   	G4double posY = -((l - numBlocks / 2) * spacing + 3.0*cm); // Adjust for alignment
    	G4double posX = 0.0*cm;
   	G4double posZ = 02.0 * cm;
   	
	// Physical placement
	smallScintillatorPhysical = new G4PVPlacement(0, 
                      G4ThreeVector(posX, -posY, posZ),
                      smallScintillatorLogical, 
                      "SmallScintillator2", 
                      wrapperLV1, 
                      false, 
                      l, 
                      true);
                      
        new G4PVPlacement(
    			rotCCW,                             // No rotation inside wrapper
   			G4ThreeVector(0, -34*cm, 438*cm),              // Centered
    			wrapperLV1, 
    			"ScintInsideWrapper",
    			logicWorld,
    			false,
    			0,
    			true
			);
	
	
	}
	wrapperLV1->SetVisAttributes(G4VisAttributes::GetInvisible());
	
}


void MyDetectorConstruction::ConstructSDandField() {
    MySensitiveDetector* scintSD = new MySensitiveDetector("ScintillatorSD");

    if (mainScintillatorLogical) {
        mainScintillatorLogical->SetSensitiveDetector(scintSD);
    }
    
    if (smallScintillatorLogical) {
        smallScintillatorLogical->SetSensitiveDetector(scintSD);
    }
    
    if (detectorLogical){
    detectorLogical->SetSensitiveDetector(scintSD);
    }
    
}


