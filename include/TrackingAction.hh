#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class EventAction;

class G4LogicalVolume;

/// Stepping action class

class TrackingAction : public G4UserTrackingAction
{
  public:
    TrackingAction(EventAction* eventAction);
    virtual ~TrackingAction();

    // method from the base class
    virtual void UserTrackingAction(const G4Track*);

  private:
    EventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
};

#endif
