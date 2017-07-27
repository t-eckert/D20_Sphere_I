
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Analysis.hh"

#include "G4UIcommand.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VProcess.hh"
#include "G4Track.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) {
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  // check if we are in scoring volume
  G4double inDet = 0.;
  if (volume == fScoringVolume) inDet = 1.;

  // Get the name of the process in the step
  G4String process_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  G4int nSecondaries = step->GetNumberOfSecondariesInCurrentStep();

  // Point to the track
  G4Track* track = step->GetTrack();
  // Get the ID of the track and convert type to G4double
  G4int trackID = track->GetTrackID();
  G4double track_ID = G4double(trackID);
  // Get the kinetic energy
  G4double kinetic_energy = track->GetKineticEnergy();
  // Get the position of the particle and separate it into x,y,z
  G4ThreeVector position = track->GetPosition();
  G4double x_pos = position.x();
  G4double y_pos = position.y();
  G4double z_pos = position.z();

  // Get the name of the particle
  G4String particle_name = track->GetDefinition()->GetParticleName();

  // Fill the data array
  G4double data[7] = {inDet,track_ID,edepStep,kinetic_energy,x_pos,y_pos,z_pos};
  // Fill the name array
  G4String name[2] = {particle_name,process_name};
  G4int tag = 0;

  G4cout << "nSecondaries: " << nSecondaries << G4endl;

  // G4cout << "\t"
	//   << G4UIcommand::ConvertToString(data[0]) << "\t"
  //         << G4UIcommand::ConvertToString(data[1]) << "\t"
  //         << G4UIcommand::ConvertToString(data[2]) << "\t"
  //         << G4UIcommand::ConvertToString(data[3]) << "\t"
  //         << G4UIcommand::ConvertToString(data[4]) << "\t"
  //         << G4UIcommand::ConvertToString(data[5]) << "\t"
  //         << G4UIcommand::ConvertToString(data[6]) << "\t"
  //         << name[0] << "\t"
  //         << name[1] << "\t" << G4endl;
  //   }
  fEventAction->Tag(tag);
  // //G4cout << name[0];
  // fEventAction->Collect(data, name);
}
