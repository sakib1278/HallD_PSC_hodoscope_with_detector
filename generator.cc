#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
	
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String ParticleName="gamma";
	//G4String ParticleName="e+";
	G4ParticleDefinition *particle = particleTable->FindParticle("gamma");
	//G4ParticleDefinition *particle = particleTable->FindParticle("e+");
	
	G4ThreeVector pos(-2.7*m,0.,0.);
	G4ThreeVector mom(1.,0.,0.);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	//fParticleGun->SetParticleMomentum(6*GeV);
	//fParticleGun->SetParticleEnergy(3.*GeV);
	fParticleGun->SetParticleDefinition(particle);
	
	//G4double photonEnergy = 8.4 * GeV + G4UniformRand() * (9.0 * GeV - 8.4 * GeV);
	//G4double photonEnergy = 9.0 * GeV;
	G4double photonEnergy = 12.0 * GeV;
    
    	// Set photon energy
    	fParticleGun->SetParticleEnergy(photonEnergy);

    	// Generate the primary particle
    	//fParticleGun->GeneratePrimaryVertex(anEvent);
	
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
