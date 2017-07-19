
#include "TrackingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4VProcess.hh"

TrackingAction::TrackingAction(EventAction* eventAction)
: G4UserTrackingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

TrackingAction::~TrackingAction()
{}

void TrackingAction::UserTrackingAction(const G4Track* track)
{
  G4cout << "text";

  if (!fScoringVolume) {
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();
  }

  // get volume of the current step
  const G4LogicalVolume* volume
    = track->GetLogicalVolumeAtVertex();

  // Get the name of the process in the step
  //G4String process_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  //G4cout << process_name;
  // check if we are in scoring volume
  //if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double k_energy = track->GetKineticEnergy();
  // G4ThreeVector hit_position = step->GetPosition();
  fEventAction->Kin(k_energy);
  G4cout << k_energy;
}
