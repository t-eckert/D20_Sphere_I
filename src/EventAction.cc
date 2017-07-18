#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fNtag(0),
  fKinetic(0.)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  // Set the energy deposited to zero.
  fEdep = 0.;
  // Set the neutron tag number to 0
  fNtag = 0;
  // Set the kinetic energy to zero
  fKinetic = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{
  // Get Analysis Manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Fill histogram
/*  if(fNtag==0)          // neutrons that didn't interact
    analysisManager->FillH1(0, fEdep);
  else if (fNtag==1)    // deuteron breakup neutrons
    analysisManager->FillH1(1, fEdep);
  else if (fNtag==2)    // elastically scattered neutrons
    analysisManager->FillH1(2, fEdep);
*/
  analysisManager->FillH1(0,fKinetic);
  // accumulate statistics in run action
  fRunAction->Collect(fEdep);
}
