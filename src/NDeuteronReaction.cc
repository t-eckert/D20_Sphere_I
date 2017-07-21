#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>

#include "NDeuteronReaction.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"

#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"

#include "G4IonTable.hh"


NDeuteronReaction::NDeuteronReaction(G4VPreCompoundModel* ptr)
: G4HadronicInteraction("Fusion"),
  theProjectileFragmentation(ptr),
  pA(0),pZ(0), tA(0),tZ(0),spectatorA(0),spectatorZ(0),
  projectile3dNucleus(0),target3dNucleus(0)
{
}

NDeuteronReaction::~NDeuteronReaction()
{}

void NDeuteronReaction::ModelDescription(std::ostream& outFile) const
{
	outFile << "G4Fusion "
			<< "test\n";
}

//--------------------------------------------------------------------------------
struct ReactionProduct4Mom
{
   G4LorentzVector operator()(G4LorentzVector a,G4ReactionProduct* b) {return a + G4LorentzVector(b->GetMomentum(), b->GetTotalEnergy() );}
};

G4HadFinalState *NDeuteronReaction::
ApplyYourself(const G4HadProjectile &aTrack, G4Nucleus & tNucleus )
{
//      G4cout << "NDeuteronReaction::ApplyYourself called" <<G4endl;


	pA=aTrack.GetDefinition()->GetBaryonNumber();
	pZ=G4lrint(aTrack.GetDefinition()->GetPDGCharge()/eplus);
	tA=tNucleus.GetA_asInt();
	tZ=tNucleus.GetZ_asInt();


    G4double tM=G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(tZ,tA);

	G4LorentzVector incMom(aTrack.Get4Momentum());
	G4LorentzVector tarMom(0,0,0,tM);

// 	G4cout << "proj mom : " << incMom << G4endl;
// 	G4cout << "target mom : " << tarMom << G4endl;
//  G4double iM=G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(pZ,pA);
// 	G4cout << "Fusion: "  <<  incMom.theta()*180/pi << " " << (incMom.e()-iM) << G4endl;

	// transform into CM frame
	const G4ThreeVector   cmBoost = incMom.findBoostToCM(tarMom);//The boost from CM to lab
    const G4LorentzVector cmincMom = incMom.boost(cmBoost);
	const G4LorentzVector cmtarMom = tarMom.boost(cmBoost);

//   G4cout << "proj mom : " << cmincMom << G4endl;
// 	G4cout << "target mom : " << cmtarMom << G4endl;


	G4double eInc = incMom.e() + tarMom.e(); // total energy

	G4ParticleDefinition*  products[2];
	GetFusionProducts( pA, pZ, tA, tZ, products);

	G4double rM=products[0]->GetPDGMass();
    G4double sM=products[1]->GetPDGMass();

//  	G4cout << "recoil: " << rM << " scatter: " << sM << G4endl;
//  	G4cout << "incEnergy : " << eInc << " resEnergy: " << eRes << " Q: " << (eRes-eInc) << G4endl;

	// devide energy
	/* Momentum conservation: pc^2 = Er^2-Mr^2 = Es^2-Ms^2  with M = m0*c^2
	 * Energy conservation  : E = Er + Es = Mr + Ms + Q
	 *
	 *				E^2 - Ms^2 + Mr^2
	 * ->     Er =  -----------------
	 *   			      2E
	 */

	// energy
	G4double rE = (eInc*eInc - sM*sM + rM*rM)/(2*eInc);
	G4double sE = eInc - rE;		  // Energy conservation or (eInc*eInc - rM*rM + sM*sM)/(2*eInc);

	// momentum
	G4double rP = sqrt(rE*rE-rM*rM);
	G4double sP = rP;                 // Momentum conservation or sqrt(sE*sE-sM*sM);

	// Lorentz vectors in CM Frame
	G4LorentzVector cmrMom = G4LorentzVector(0.0,0.0,-rP,rE);
	G4LorentzVector cmsMom = G4LorentzVector(0.0,0.0, sP,sE);
// 	G4cout << "recoil  mom (CM): " << cmrMom << G4endl;
// 	G4cout << "scatter mom (CM): " << cmsMom << G4endl;

    //MODIFY THIS FOR ANGULAR CROSS SECTION
	//vertex position uniform in spherical shell
    G4ThreeVector normalVector=G4ThreeVector(1.,1.,1.);
    G4ThreeVector ur;
    G4double prob;

    do {
        G4double cosTheta = 2*G4UniformRand() - 1;  //cosTheta uniform in [0, pi]
        G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
        G4double phi      = twopi*G4UniformRand();  //phi uniform in [0, 2*pi]
        ur=G4ThreeVector(sinTheta*std::cos(phi),sinTheta*std::sin(phi),cosTheta);

        G4double angle= normalVector.angle(ur);
        prob=1*(angle<2*CLHEP::pi);
    } while(G4UniformRand()>prob);
    G4cout << normalVector.angle(ur) << G4endl;

	cmrMom = G4LorentzVector(ur*rP,rE);
	cmsMom = G4LorentzVector(-ur*sP,sE);

// 	G4cout << "recoil  mom : " << cmrMom << G4endl;
// 	G4cout << "scatter mom : " << cmsMom << G4endl;

	//transform into Lab frame
	G4LorentzVector labrMom = cmrMom.boost(-cmBoost);
	G4LorentzVector labsMom = cmsMom.boost(-cmBoost);

//  	G4cout << "recoil  mom (Lab) : " << labrMom << G4endl;
//  	G4cout << "scatter mom (Lab) : " << labsMom << G4endl;

//      G4cout << "Fusion: "  <<  labsMom.theta()*180/pi << " " << (labsMom.t()-sM) << G4endl;

	// store particle info
	theResult.Clear();
    theResult.SetStatusChange(stopAndKill);  //isAlive alternative;

    G4DynamicParticle * rPart = new G4DynamicParticle(products[0], labrMom);
//  	rPart->GetDefinition()->GetProcessManager()->StartTracking();

	theResult.AddSecondary(rPart);

	G4DynamicParticle * sPart = new G4DynamicParticle(products[1], labsMom);
//  	sPart->GetDefinition()->GetProcessManager()->StartTracking();
	theResult.AddSecondary(sPart);

	return &theResult;
}

//--------------------------------------------------------------------------------


void NDeuteronReaction::GetFusionProducts(G4double pA, G4double pZ, G4double tA, G4double tZ, G4ParticleDefinition*  products[])
{

    G4ParticleDefinition* Boron10 = G4IonTable::GetIonTable()->GetIon(5,10);


    if ( (pA == 2 && pZ == 1 && tA == 9 && tZ == 4) || (pA == 9 && pZ == 4 && tA == 2 && tZ == 1) ) {
        products[0] = Boron10;
        products[1] = G4Proton::Proton();

        G4cout << "Boron10 generated" << G4endl;

    }

    G4cout << "GetFusionProducts called."<< G4endl;

  return;
}
