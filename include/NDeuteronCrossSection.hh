#ifndef NDeuteronCrossSection_h
#define NDeuteronCrossSection_h
//
// Class Description
// Implementation of formulas
// Shen et al. Nuc. Phys. A 491 130 (1989);
// Total Reaction Cross Section for Heavy-Ion Collisions
//
// Class Description - End
// 18-Sep-2003 First version is written by T. Koi
// 12-Nov-2003 Set upper limit at 10 GeV/n
// 12-Nov-2003 Insted of the lower limit,
//             0 is returned to a partilce with energy lowae than 10 MeV/n
// 15-Nov-2006 Change upper limit to 1 TeV/n
//             However above 10GeV/n XS become constant.
// 23-Dec-2006 Isotope dependence added by D. Wright
// 19-Aug-2011 V.Ivanchenko move to new design and make x-section per element
//

#include <vector>

#include "G4BinaryCascade.hh"
#include "G4PreCompoundModel.hh"

#include "G4VCrossSectionDataSet.hh"


class NDeuteronCrossSection : public G4VCrossSectionDataSet
{

public:

  NDeuteronCrossSection();


  virtual ~NDeuteronCrossSection();

  virtual
  G4bool IsElementApplicable(const G4DynamicParticle* aDP,
			     G4int Z, const G4Material*);

  virtual
  G4double GetElementCrossSection(const G4DynamicParticle*,
			     G4int Z, const G4Material*);

  virtual
  G4double GetIsoCrossSection(const G4DynamicParticle*, G4int Z, G4int A,
			      const G4Isotope* iso = 0,
			      const G4Element* elm = 0,
			      const G4Material* mat = 0);

  virtual void CrossSectionDescription(std::ostream&) const;

private:

  const G4double upperLimit;
  const G4double lowerLimit;
  const G4double r0;

  G4double GetCMEnergy(const G4DynamicParticle* aDParticle, G4int tZ, G4int tA);

  G4int    GetCrossSectionIndex(const G4double E, const G4int pZ, const G4int pA,
										          const G4int tZ, const G4int tA);

  G4double CalcCrossSection(const G4double E);
};




#endif
