// //
// // ********************************************************************
// // * License and Disclaimer                                           *
// // *                                                                  *
// // * The  Geant4 software  is  copyright of the Copyright Holders  of *
// // * the Geant4 Collaboration.  It is provided  under  the terms  and *
// // * conditions of the Geant4 Software License,  included in the file *
// // * LICENSE and available at  http://cern.ch/geant4/license .  These *
// // * include a list of copyright holders.                             *
// // *                                                                  *
// // * Neither the authors of this software system, nor their employing *
// // * institutes,nor the agencies providing financial support for this *
// // * work  make  any representation or  warranty, express or implied, *
// // * regarding  this  software system or assume any liability for its *
// // * use.  Please see the license in the file  LICENSE  and URL above *
// // * for the full disclaimer and the limitation of liability.         *
// // *                                                                  *
// // * This  code  implementation is the result of  the  scientific and *
// // * technical work of the GEANT4 collaboration.                      *
// // * By using,  copying,  modifying or  distributing the software (or *
// // * any work based  on the software)  you  agree  to acknowledge its *
// // * use  in  resulting  scientific  publications,  and indicate your *
// // * acceptance of all terms of the Geant4 Software license.          *
// // ********************************************************************
// //
// //
// #include <algorithm>
// #include <vector>
// #include <cmath>
// #include <numeric>
//
// #include "G4FusionCrossSection.hh"
//
// #include "G4PhysicalConstants.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4DynamicParticle.hh"
// #include "G4NistManager.hh"
// #include "G4ParticleTable.hh"
//
// #include "G4LorentzVector.hh"
// #include "G4LorentzRotation.hh"
//
// #include "G4PhysicsVector.hh"
//
//
// // ENTER ENERGY DEPENDENT CROSS SECTION HERE (DISCRETE POINTS)
// // {deuteron energy keV, cross section mbarn/sr
//
//
// // ENTER Z's FOR WHICH CROSS SECTION APPLIES
// static const G4int ZTargetApply=4;                   //9Be
// static const G4int ZProjectileApply=1;               //d
//
// // ENTER MAX/MIN ENERGY
// G4FusionCrossSection::G4FusionCrossSection()
//   : G4VCrossSectionDataSet("Fusion"),
//     upperLimit( 2*MeV ), lowerLimit( 0*MeV ), r0 ( 1.1 )
// {
// }
//
// G4FusionCrossSection::~G4FusionCrossSection()
// {}
//
// void
// G4FusionCrossSection::CrossSectionDescription(std::ostream& outFile) const
// {
//   outFile << "G4FusionCrossSection calculates the total reaction spectra\n"
//           << "for arbitrary input cross section\n";
// }
//
//
// G4bool G4FusionCrossSection::IsElementApplicable(const G4DynamicParticle* aDParticle,
// 						   G4int Z, const G4Material* aMaterial )
// // Z == target Z
// {
//   G4int tA=0;                                                         //target baryon number
//   G4int tZ=0;
//
//   G4int pA = aDParticle->GetDefinition()->GetBaryonNumber();          //projectile baryon number
//   G4int pZ = aDParticle->GetDefinition()->GetAtomicNumber();
//
// //   G4cout << "G4FusionCrossSection::IsElementApplicable " << pA << " " << pZ << G4endl;
//
//   const G4ElementVector * elements = aMaterial->GetElementVector();
//   for (unsigned int i=0; i < elements->size(); i++) {
// 	tA = (*elements)[i]->GetIsotope(0)->GetN();
// 	tZ = Z; // or use(*elements)[i]->GetZ();
// // 	G4cout << "G4FusionCrossSection::IsElementApplicable "  << tA << " " << tZ << G4endl;
//   }
//
//   return ( pZ == ZProjectileApply);
// }
//
//
// G4double
// G4FusionCrossSection::GetElementCrossSection(const G4DynamicParticle* aDParticle,
// 					       G4int tZ,
// 					       const G4Material* aMaterial)
// // Z == target Z
// {
//   // A == target A
//   G4int tA = aMaterial->GetElement(0)->GetIsotope(0)->GetN();
//
//   G4int pZ = aDParticle->GetDefinition()->GetAtomicNumber();
//   G4int pA = aDParticle->GetDefinition()->GetBaryonNumber();
//
//   if (tZ != ZTargetApply) return 0.0;
//
//   G4double xsection = GetIsoCrossSection(aDParticle, tZ, tA);
//
//   G4double nDens =  aMaterial->GetElement(0)->GetA();
//
// //  G4cout << "G4FusionCrossSection::GetElementCrossSection " <<  tZ << " " << tA <<
// //          " numberDens: " << nDens << " xsection " <<  (xsection*nDens) << G4endl;
//
//   return xsection;
// }
//
//
// G4double G4FusionCrossSection::GetIsoCrossSection(const G4DynamicParticle* aDParticle,
// 						    G4int tZ, G4int tA,
// 						    const G4Isotope* , const G4Element*, const G4Material* )
// {
//
//     if (tZ != ZTargetApply) return 0.0;
// 	// calculate CM energy in keV
// 	G4double E = GetCMEnergy(aDParticle, tZ, tA) * 1000.0;
//
//     G4int pZ = aDParticle->GetDefinition()->GetAtomicNumber();
// 	G4int pA = aDParticle->GetDefinition()->GetBaryonNumber();
//
//     G4double xsection = CalcCrossSection(E) * millibarn*4*CLHEP::pi;   // xsection is in units of mbarn/sr
//
// //    G4cout << "G4FusionCrossSection::GetIsoCrossSection called! "  << pZ << " " << pA << " " << E <<  " "
// //       << " xsection: " << xsection/millibarn << G4endl;
//
// 	return xsection;
// }
//
//
// G4double G4FusionCrossSection::GetCMEnergy(const G4DynamicParticle* aDParticle, G4int tZ, G4int tA)
// {
// 	// projectile
// 	const G4double pM = aDParticle->GetMass();
// 	G4LorentzVector incMom = aDParticle->Get4Momentum();
//
// 	//target
//     // const G4double tM = G4ParticleTable::GetParticleTable()->FindIon(tZ,tA,0,0)->GetPDGMass();
//     const G4double tM = 4.5*GeV;                  //MANUALLY ENTERED TARGET MASS
// 	G4LorentzVector tarMom(0,0,0,tM);
//
// // 	G4cout << "proj mom : " << incMom << "target mom : " << tarMom << " pM " << pM << " tM " << tM << G4endl;
//
// 	// transform into CM frame
// 	const G4ThreeVector   cmBoost = incMom.findBoostToCM(tarMom);//The boost from CM to lab
// 	const G4LorentzVector cmincMom = incMom.boost(cmBoost);
// 	const G4LorentzVector cmtarMom = tarMom.boost(cmBoost);
//
// 	G4double eCM = (incMom.e() + tarMom.e())- (pM + tM); // Kinetic Energy = Total energy - Rest Energy*/
//
// //     G4cout << "proj mom : " << cmincMom << "target mom : " << cmtarMom << " " << " KE " << eCM << G4endl;
//
//     return eCM;  // MEV
// }
//
//
// G4double
// G4FusionCrossSection::CalcCrossSection(const G4double E)
// {
//
//
//    // interpolate from discrete datapoints CrSec
//
//     G4double energies[40]={2000*eV, 1950*eV, 1900*eV, 1850*eV, 1800*eV, 1750*eV, 1700*eV, 1650*eV, 1600*eV, 1550*eV, 1500*eV, 1450*eV, 1400*eV,
//                            1350*eV, 1300*eV, 1250*eV, 1200*eV, 1150*eV, 1100*eV, 1050*eV, 1000*eV, 950*eV, 900*eV, 850*eV, 800*eV, 750*eV, 700*eV,
//                            650*eV, 600*eV, 550*eV, 500*eV, 450*eV, 400*eV, 350*eV, 300*eV, 250*eV, 200*eV, 150*eV, 100*eV, 50*eV};
//     G4double data[40]={17.4, 17.5, 17.7, 17.6, 17.8, 18, 18.2, 17.4, 17.6, 17.7, 17.5, 16.6, 16.2, 16.2, 16, 16.1, 15.2, 14.9, 14.6, 14.3, 12.9, 12.3,
//                      11.6, 10.9, 10.2, 9.23, 8.23, 6.99, 5.79, 4.43, 2.93, 1.2, 0.37, 0.16, 0.07, 0.03, 0.01, 0.01, 0};
//
//
//     G4PhysicsOrderedFreeVector interpVect(energies, data, 3);
//     double cross = interpVect.Value(E);
//
//     //G4cout << "CalcCrossSection called with energy (eV) " << E/eV << " Cross section " << cross << G4endl;
//
//
//   /*
//    G4int i=0;
//    while(CrSec[i][0] > E){i++;}
//
//    G4double slopem = (CrSec[i][1]-CrSec[i-1][1])/(CrSec[i][0]-CrSec[i-1][0]);
//    G4double intercb = CrSec[i][1]-slopem*CrSec[i][0];
//
//    G4double cross=slopem*E+intercb;
//
//    */
//
//
//    return cross;
// }
