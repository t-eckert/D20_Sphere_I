#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


RunAction::RunAction()
: G4UserRunAction(),
  fEdep(0.),
  fEdep2(0.),
  fKinetic(0.)
{
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2);

  // Create an instance of analysisManager
  auto analysisManager = G4AnalysisManager::Instance();
/*
  analysisManager->CreateH1("edep_ni","Edep in SV by non-interacting neutrons", 1500, 0., 15.*MeV);
  analysisManager->CreateH1("edep_db","Edep in SV by deuteron breakup neutrons", 1500, 0., 15.*MeV);
  analysisManager->CreateH1("edep_es","Edep in SV by elastically scattered neutrons", 1500, 0., 15.*MeV);
  */
  analysisManager->CreateH1("ke","Kinetic Energy in SV", 1500, 0., 15.*MeV);
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  // Open a file for the historgram
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("histogram");
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetH1Plotting(0, true);

  //G4double edep  = fEdep.GetValue();
  //G4double edep2 = fEdep2.GetValue();

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // Print
  //
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }

  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;

  // Output the histogram
  analysisManager->Write();
  analysisManager->CloseFile();
}

void RunAction::Collect(G4double edep, G4double kinetic_energy)
{
  fEdep  += edep;
  fEdep2 += edep*edep;

  fKinetic = kinetic_energy;

  // Split threevector into x,y,z.
  /*4double x_pos = position.x();
  G4double y_pos = position.y();
  G4double z_pos = position.z();*/
}
