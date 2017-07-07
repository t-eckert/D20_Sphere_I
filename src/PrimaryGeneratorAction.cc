#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEnvelopeBox(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="neutron");
  fParticleGun->SetParticleDefinition(particle);

  // Ramdomize initial momentum direction within a cone with
  // a base equal to the cross section of the D2O Sphere
  G4double theta = G4UniformRand()*twopi;
  G4double phi = G4UniformRand()*0.25;

  G4double x_momentum = std::cos(theta)*std::sin(phi);
  G4double y_momentum = std::sin(theta)*std::sin(phi);
  G4double z_momentum = std::cos(phi);

  G4ThreeVector momentum_dir = G4ThreeVector(x_momentum,y_momentum,-z_momentum);

  fParticleGun->SetParticleMomentumDirection(momentum_dir);
  fParticleGun->SetParticleEnergy(14.03*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of each event

  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,10.*cm));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
