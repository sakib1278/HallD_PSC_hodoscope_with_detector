#include "construction.hh"
//#include "MySensitiveDetector.hh"
#include "detector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"


MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	
	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
	
	nCols = 100;
	nRows = 100;
	
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
	solidWorld = new G4Box("solidWorld", 4.5*m, 2.10*m, 2.10*m);
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	
	//Detector
	solidDetector = new G4Box("solidDetector", 3.0*cm, 2.0*m, 2.*m);
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
	physDetector = new G4PVPlacement(0, G4ThreeVector(4.0*m, 0, 0), logicDetector, "physDetector", logicWorld, false, 0, true);
	
	detectorSolid = new G4Box("DetectorSolid", 0.50 * cm, 25.0 * cm, 25.0 * cm);
	detectorLogical = new G4LogicalVolume(detectorSolid, worldMat, "DetectorLogical");
	

	G4double detPosX = 2.40 * m;  // 1 m ahead
	G4double detPosY = -43.0 * cm;               // align vertically with scints
	G4double detPosZ = 0.0 * cm;                 // centered in Z

	G4RotationMatrix* rotCW1 = new G4RotationMatrix();
	rotCW1->rotateZ(7.0 * deg);
	
	BigDetectorPhysical= new G4PVPlacement(rotCW1,
		  G4ThreeVector(detPosX, detPosY, detPosZ),
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
        converterBox = new G4Box("Converter", 75.0 * um, 7.0 * cm, 3.0 * cm);
        converterLV = new G4LogicalVolume(converterBox, beryllium, "Converter");
        G4VisAttributes* converterVis = new G4VisAttributes(G4Colour::Red());
		converterVis->SetForceSolid(true);
		converterLV->SetVisAttributes(converterVis);
        physConverter = new G4PVPlacement(nullptr, G4ThreeVector(-2.97 * m, 0, 0), converterLV, "Converter", logicWorld, false, 0, true);



        // Dipole Magnet (outside vacuum chamber)
        magnetBox = new G4Box("Magnet", 0.45 * m, 0.5 * m, 0.1 * m);
        magnetLV = new G4LogicalVolume(magnetBox, worldMat, "Magnet");
        /*G4VisAttributes* magnetVis = new G4VisAttributes(G4Colour::Green());
	magnetVis->SetForceSolid(true);
	magnetLV->SetVisAttributes(magnetVis);*/
        physMagnet = new G4PVPlacement(nullptr, G4ThreeVector(-1.95 * m, 0, 0), magnetLV, "Magnet", logicWorld, false, 0, true);

        // Magnetic Field in Dipole Magnet
        G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(0, 0, 1.662 * tesla));
        G4FieldManager* fieldManager = new G4FieldManager(magField);
        magnetLV->SetFieldManager(fieldManager, true);
        
        

	// Define Trapezoidal Vacuum Chamber Cross-section
	std::vector<G4TwoVector> trapezoidPoints = {
    	G4TwoVector(-0.20 * m, -0.75 * m),  // Bottom-left
    	G4TwoVector( 0.20 * m, -0.75 * m),  // Bottom-right
    	G4TwoVector( 0.50 * m,  0.75 * m),  // Top-right (wider part)
    	G4TwoVector(-0.50 * m,  0.75 * m)   // Top-left (wider part)
	};

	// Set extrusion depth along Z-axis
	G4double halfDepth = 10.0 * cm / 2.0;

	G4ExtrudedSolid* vacuumChamberShape = new G4ExtrudedSolid("VacuumChamber", trapezoidPoints, halfDepth, 		G4TwoVector(0,0), 1.0, G4TwoVector(0,0), 1.0);

	// Create Logical Volume
	G4LogicalVolume* vacuumChamberLV = new G4LogicalVolume(vacuumChamberShape, vacuum, "VacuumChamber");

	// Rotate by 90 degrees around X to align along Y
	G4RotationMatrix* rotation = new G4RotationMatrix();
	rotation->rotateZ(90 * deg);

	// Place inside World
	new G4PVPlacement(rotation, G4ThreeVector(-0.75 * m, 0, 0), vacuumChamberLV, "VacuumChamber", logicWorld, false, 0);
	
	return physWorld;
	
}

void MyDetectorConstruction::ConstructScintillator()
{
	G4RotationMatrix* rotCCW = new G4RotationMatrix();
	rotCCW->rotateZ(5.0 * deg);

	G4RotationMatrix* rotCW = new G4RotationMatrix();
	rotCW->rotateZ(-5.0 * deg);


	// Define the large scintillator box
	mainScintillatorSolid = new G4Box("MainScintillator", 0.50*cm, 12.50*cm, 1.50*cm);
	mainScintillatorLogical = new G4LogicalVolume(mainScintillatorSolid, EJ212, "MainScintillatorLogical");
	
	
	G4Box* mainWrapperSolid = new G4Box("MainWrapper", 0.6*cm, 13*cm, 2*cm);
	G4LogicalVolume* mainWrapperLogical = new G4LogicalVolume(mainWrapperSolid, vacuum, "MainWrapperLogical");

	// Physical placement
	mainScintillatorPhysical = new G4PVPlacement(rotCW,                   // No rotation
                      			G4ThreeVector(100.*cm, 33*cm, 0),  // Center position
                      			mainScintillatorLogical, // Logical volume
                      			"MainScintillator1",      // Name
                      			logicWorld,            // Mother volume
                      			false,                   // No boolean operation
                      			0,                       // Copy number
                      			true);                   // Check overlaps
                      			
        mainScintillatorPhysical = new G4PVPlacement(rotCCW,                   // No rotation
                      			G4ThreeVector(100.*cm, -33*cm, 0),  // Center position
                      			mainScintillatorLogical, // Logical volume
                      			"MainScintillator2",      // Name
                      			logicWorld,            // Mother volume
                      			false,                   // No boolean operation
                      			0,                       // Copy number
                      			true);                   // Check overlaps



	
	// Define the small scintillator box
	smallScintillatorSolid = new G4Box("SmallScintillator", 1.0*cm, 3.0*cm, 2.2*cm);
	smallScintillatorLogical = new G4LogicalVolume(smallScintillatorSolid, EJ212, "SmallScintillatorLogical");
	
	G4Box* wrapperBox = new G4Box("Wrapper", 0.055 * m, 0.32 * m, 0.05 * m);
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
    G4double posX = 2.0*cm;
	G4double posZ = 0.0 * cm;
    	
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
   			G4ThreeVector(141*cm, 34*cm, 0),              // Centered
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
    	//G4double posX = 40.0*cm;                             // Centered vertically
   	//G4double posZ = 0.0 * cm;
   	G4double posX = 0.0*cm;
   	G4double posZ = 0.0 * cm;
   	
	// Physical placement
	smallScintillatorPhysical = new G4PVPlacement(0, 
                      //G4ThreeVector(posX+1*m, -(posY +35*cm), posZ), 
                      G4ThreeVector(posX, -posY, posZ),
                      smallScintillatorLogical, 
                      "SmallScintillator2", 
                      //logicWorld,
                      wrapperLV1, 
                      false, 
                      k, // Unique copy number
                      true);
	}
	
	for (G4int l = 0; l < 4; l++) 
	{
   	G4double posY = -((l - numBlocks / 2) * spacing + 3.0*cm); // Adjust for alignment
    	//G4double posX = 42.*cm;                             // Centered vertically
   	//G4double posZ = 0.0 * cm;                     // Position forward of the main block
	G4double posX = 2.0*cm;
   	G4double posZ = 0.0 * cm;
   	
	// Physical placement
	smallScintillatorPhysical = new G4PVPlacement(0, 
                      //G4ThreeVector(posX+1*m, -(posY+ 35*cm), posZ), 
                      G4ThreeVector(posX, -posY, posZ),
                      smallScintillatorLogical, 
                      "SmallScintillator2", 
                      //logicWorld,
                      wrapperLV1, 
                      false, 
                      l, // Unique copy number
                      true);
                      
        new G4PVPlacement(
    			rotCCW,                             // No rotation inside wrapper
   			G4ThreeVector(141*cm, -34*cm, 0),              // Centered
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

    /*if (mainScintillatorLogical) {
        mainScintillatorLogical->SetSensitiveDetector(scintSD);
    }
    
    if (smallScintillatorLogical) {
        smallScintillatorLogical->SetSensitiveDetector(scintSD);
    }*/
    //MySensitiveDetector* bigDetSD = new MySensitiveDetector("BigDetectorSD");
    G4cout << "In SDandField: detectorLogical = " << detectorLogical << G4endl;

    if (detectorLogical){
    detectorLogical->SetSensitiveDetector(scintSD);
    }
    //G4SDManager::GetSDMpointer()->AddNewDetector(bigDetSD);

}


