#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "PhysicalConstants.hh"

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction(),
fScoringVolume(0)
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  // Materials ================================================================
  // Get NIST material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Create a low-density gas to act as vacuum
  density = universe_mean_density;
  pressure = 1.e-19*pascal;
  temperature = 0.1*kelvin;
  G4Material* vacuum = new G4material(name="Vacuum", z=1., a=1.01*g/mole,
    density, kStateGas, temperature, pressure);

  // Get deuterium isotope and create deuterium element
  G4Isotope* isoD = new G4Isotope(name="Deuterium (Isotope)", iz=1, n=2,
    a=2.014*g/mole);
  G4Element* elD = new G4Element(name="Deuterium", symbol="D", ncomponents=1);
  elD->AddIsotope(isoD, abundance=1*perCent);

  // Define Oxygen
  G4Element* elO = new G4Element(name="Oxygen", symbol="O", z=8., a=16.00*g/mole);

  // Create Heavy Water
  density = 1.11*g/cm3;
  G4Material* D2O = new G4Material(name="Heavy Water", density, ncomponents=2);
  D20->AddElement(elD, natoms=2);
  D2O->AddElement(elO, natoms=1);

  // World ====================================================================
  // World volume
  G4double world_sizeXY = 10*cm;
  G4double world_sizeZ = 200*cm;

  // Fill with vacuum
  G4Material* world_mat = FindOrBuildMaterial("Vacuum")

  G4Box* solidWorld = new G4Box( "Chamber",
    0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ );

  G4LogicalVolume* logicWorld = new G4LogicalVolume( solidWorld,
      world_mat, "Chamber" );

  G4VPhysicalVolume* physWorld = new G4PVPlacement(
      0,                      // rotation
      G4ThreeVector(),        // at (0,0,0)
      logicWorld,             // logical volume
      "Chamber",              // name
      0,                      // mother volume
      false,                  // no boolean operation
      0,                      // copy number
      true                    // check overlaps
  );

  // D2O Sphere ===============================================================


  // Define the material and location of the sphere
  G4Material* material_D20_Sphere = FindOrBuildMaterial("Heavy Water");
  G4ThreeVector origin = G4ThreeVector(0,0,0);

  // Define the sphere's geometry
  G4double rmin=0.*cm, rmax=2.44*cm;
  G4double phimin=0., phimax=pi;
  G4double thetamin=0., thetamax=twopi;

  // Create a solid volume
  G4Sphere* solid_D20_Sphere =
    new G4Sphere("D20 Sphere",                // name
                  rmin, rmax,                 // set radius
                  phimin, phimax,             // set phi angle
                  thetamin, thetamax);        // set theta angle

  // Create a logical volume
  G4LogicalVolume* logical_D20_Sphere =
    new G4LogicalVolume(solid_D20_Sphere,     // solid
                        material_D20_Sphere   // material
                        "D20 Sphere");        // name

  // Place the shape in space
  new G4PVPlacement(0,                        // no rotation
                    origin,                   // location at (0,0,0)
                    logical_D20_Sphere,       // logical volume
                    "D20 Sphere",             // name
                    logicWorld,               // mother volume
                    false,                    // no boolean operation
                    0,                        // copy number
                    true);                    // check for overlapping

  // Scoring Volume ===========================================================
  // Define material and position
  G4Material* material_Scoring_Volume = FindOrBuildMaterial("Vacuum");
  G4ThreeVector position_Scoring_Volume = G4ThreeVector(0,0,-80*cm);

  // Thin Plane 10x10x0.01 cm
  // Distances are given from halfpoint
  G4double Scoring_Volume_x = 5*cm, Scoring_Volume_y = 5*cm;
  G4double Scoring_Volume_z = 0.005*cm;

  // Create a solid volume
  G4Box* solid_Scoring_Volume =
    new G4Box("Scoring Volume",
              Scoring_Volume_x, Scoring_Volume_y, Scoring_Volume_z);

  G4LogicalVolume* logical_Scoring_Volume =
    new G4LogicalVolume(solid_Scoring_Volume,
                        material_Scoring_Volume,
                        "Scoring Volume");

  new G4PVPlacement(0,
                    position_Scoring_Volume,
                    logical_Scoring_Volume,
                    "Scoring Volume",
                    logicWorld,
                    false,
                    0
                    true);

  fScoringVolume = logical_Scoring_Volume;

  return physWorld
}
