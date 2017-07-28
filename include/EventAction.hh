#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;

/// Event action class

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void Tag(G4int tag, G4int step_number){
      fTag = tag;
    }

    // void Collect(G4double data[7], G4String name[2])
    // {
    //   G4double edep = data[2];
    //   fEdep += edep;
    // }

  private:
    RunAction* fRunAction;
    G4int fTag;

    // G4double   fEdep;
    // G4double fData[7];
    // G4double fName[2];
};

#endif
