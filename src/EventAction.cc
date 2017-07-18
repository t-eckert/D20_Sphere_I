#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.), fKinetic(0.)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  // Set the energy deposited to zero.
  fEdep = 0.;
  fKinetic = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{
  // Get Analysis Manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Fill histogram
  analysisManager->FillH1(0, fEdep);

  if(fKinetic != 0.)
  G4cout << fKinetic << G4endl;
  // accumulate statistics in run action
  fRunAction->Collect(fEdep,fKinetic);
}
