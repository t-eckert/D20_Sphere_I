#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

EventAction::EventAction(RunAction* runAction)
  : G4UserEventAction(),
   fRunAction(runAction),
   fTag(0)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
  // Set the tag to 0
  fTag = 0;
}

void EventAction::EndOfEventAction(const G4Event*)
{
  // Get Analysis Manager
  //auto analysisManager = G4AnalysisManager::Instance();

  // Fill histogram
  //analysisManager->FillH1(0, fEdep);

  fRunAction->Tag(fTag);
  //G4cout << fTag;
  //G4cout << fName[0];
  // accumulate statistics in run action
  //fRunAction->Collect(fData,fName)
}
