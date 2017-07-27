#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void Tag(G4int tag);

    // void Collect(G4double* data, G4String* name);

  private:
    G4int fTag;
    // G4Accumulable<G4double> fEdep;
    // G4Accumulable<G4double> fEdep2;
    // G4double fData[7];
    // G4double fName[2];
};

#endif
