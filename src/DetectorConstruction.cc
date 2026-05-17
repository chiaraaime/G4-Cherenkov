//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4SDManager.hh" //Chiara for linking SD to the detector
#include "ChamberSD.hh" //Chiara
#include "OpticalSD.hh" //Chiara per cherenkov
#include "G4GenericMessenger.hh" //Chiara per passare il materiale

namespace RUT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fMessenger(nullptr), fMaterialName("MgF2"), fThickness(0.5)
  {
    fMessenger = new G4GenericMessenger(this, "/detector/", "Controllo della geometria");
    auto& matCmd = fMessenger->DeclareProperty("setMaterial", fMaterialName, "Imposta il materiale del radiatore");
    matCmd.SetGuidance("Scegli tra i materiali implementati, es: MgF2, PbF2, mio");
    matCmd.SetParameterName("material", false);
    auto& thiCmd = fMessenger->DeclareProperty("setThickness", fThickness, "Imposta lo spessore del piombo");
    thiCmd.SetGuidance("Scrivi in mm lo spessore di Pb che vuoi");
    thiCmd.SetParameterName("thick", false);
    matCmd.SetStates(G4State_PreInit); 
  }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  DetectorConstruction::~DetectorConstruction()
  {
    delete fMessenger;
  }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  G4VPhysicalVolume* DetectorConstruction::Construct()
  {
    // Get nist material manager
    G4NistManager* nistManager = G4NistManager::Instance();
    // Build materials
    G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
    
    //Per il radiatore 
    G4Material* rad_material = nullptr;
    G4cout << " >>>> [Detector] Configurazione materiale radiatore: " << fMaterialName << G4endl;
    std::vector<G4double> photonEnergy;
    std::vector<G4double> refractiveIndex;
    std::vector<G4double> adsorptionLength;
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    
    if(fMaterialName == "MgF2" || fMaterialName == "mio"){
      rad_material = nistManager->FindOrBuildMaterial("G4_MAGNESIUM_FLUORIDE");
      photonEnergy = {0.25*eV, 0.5*eV, 1.0*eV, 1.5*eV, 2.0*eV, 2.5*eV, 3.0*eV, 3.5*eV, 4.0*eV, 4.5*eV, 5.0*eV, 6.25*eV, 6.75*eV, 7.0*eV, 7.25*eV, 7.9*eV, 8.2*eV, 8.5*eV, 9.0*eV};
      refractiveIndex = {1.342, 1.366, 1.372, 1.375, 1.377, 1.380, 1.383, 1.387, 1.392, 1.397, 1.405, 1.423, 1.434, 1.440, 1.447, 1.465, 1.476, 1.489, 1.503};
      // https://refractiveindex.info/?shelf=main&book=MgF2&page=Li-o
      // https://www.crystran.com/optical-materials/magnesium-fluoride-mgf2/
      adsorptionLength = {10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 10.0*m, 46.25*cm, 15.25*cm, 9.05*cm, 7.50*cm, 5.56*cm, 3.32*cm, 2.06*cm, 1.85*cm};
    }
    
    else if (fMaterialName == "PbF2"){
      G4Element* elPb = nistManager->FindOrBuildElement("Pb");
      G4Element* elF  = nistManager->FindOrBuildElement("F");
      G4double density = 7.77*g/cm3;
      rad_material = new G4Material("LeadFluoride", density, 2);
      rad_material->AddElement(elPb,1);
      rad_material->AddElement(elF, 2);
      photonEnergy = {0.25*eV, 0.51*eV, 1.02*eV, 1.53*eV, 2.05*eV, 2.47*eV, 3.08*eV, 3.57*eV, 3.70*eV, 3.84*eV, 3.98*eV, 4.13*eV, 4.43*eV, 4.59*eV, 4.96*eV, 5.17*eV};
      refractiveIndex = {1.709, 1.727, 1.737, 1.748, 1.764, 1.782, 1.817, 1.857, 1.871, 1.887, 1.908, 1.937, 1.937, 1.937, 1.937, 1.937};
      //  https://www.crystran.com/optical-materials/lead-fluoride-pbf2/ costante dopo che arriva al limite
      adsorptionLength = {10.0*m, 10.0*m, 10.0*m, 39.50*cm, 26.16*cm, 15.49*cm, 12.83*cm, 7.49*cm, 6.00*cm, 5.00*cm, 3.00*cm, 2.67*cm, 2.13*cm, 1.02*cm, 0.36*cm, 0.0*cm};
      
    }
    
    else {
      G4Exception("DetectorConstruction::Construct", "InvalidMaterial", FatalException, 
                  ("Il materiale richiesto non e' supportato: " + fMaterialName).c_str());
    }
    
    mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex);
    mpt->AddProperty("ABSLENGTH", photonEnergy, adsorptionLength);
    rad_material->SetMaterialPropertiesTable(mpt);
    
    
    
    
    
    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;
    
    //
    // World
    //
    G4double hx = 20.*cm;
    G4double hy = 20.*cm;
    G4double hz = 20.*cm;
    
    // world volume
    G4Box* worldS = new G4Box("World", hx, hy, hz);
    
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldS, air, "World");
    
    G4VPhysicalVolume* worldPV
      = new G4PVPlacement(0,                     //no rotation
			  G4ThreeVector(),       //at (0,0,0)
			  worldLV,               //its logical volume
			  "World",               //its name
			  0,                     //its mother  volume
			  false,                 //no boolean operation
			  0,                     //copy number
			  checkOverlaps);        //overlaps checking
    
    //----------------------------------------------------------------------
    //Chiara: implementing Cherenkov radiator
    //setting HALF-dimensions for definition of the solid
    G4double x=5.*cm;
    G4double y=5.*cm;
    G4double z=1.5*mm;
    
    G4Box* radiatorS = new G4Box("Radiator", x, y, z);
    
    G4LogicalVolume* radiatorLV = new G4LogicalVolume(radiatorS, rad_material, "Radiator");
    
    G4VPhysicalVolume* radiatorPV 
      = new G4PVPlacement(0,
			  G4ThreeVector(),
			  radiatorLV, 
			  "Radiator",
			  worldLV,
			  false,
			  0,
			  checkOverlaps);
    
    //----------------------------------------------------------------------
    // Chiara: implementing a fakeLayer to count optical photons
    G4double z_fake = 0.05*mm; 
    
    G4Box* fakeLayerS = new G4Box("FakeLayer", x, y, z_fake);
    
    G4LogicalVolume* fakeLayerLV = new G4LogicalVolume(fakeLayerS, rad_material, "FakeLayer"); 
    
    G4VPhysicalVolume* fakeLayerPV 
      = new G4PVPlacement(0,
			  G4ThreeVector(0., 0., z + z_fake), 
			  fakeLayerLV, 
			  "FakeLayer",
			  worldLV,
			  false,
			  0,
			  checkOverlaps);
    //----------------------------------------------------------------------
    //----------------------------------------------------------------------
    if(fMaterialName == "mio"){
      G4cout << " >>>> [Detector] Sto costruendo un layer di Pb di spessore " << fThickness << G4endl;
      G4Material* layer_material = nistManager->FindOrBuildMaterial("G4_Pb");
      G4double lx=5.*cm;
      G4double ly=5.*cm;
      G4double lz=fThickness*mm;
      
      G4Box* layerPbS = new G4Box("LayerPb", lx, ly, lz);
      
      G4LogicalVolume* layerPbLV = new G4LogicalVolume(layerPbS, layer_material, "LayerPb");
      
      G4VPhysicalVolume* layerPbPV 
	= new G4PVPlacement(0, 
			    G4ThreeVector(0., 0., -(z+lz)),
			    layerPbLV, 
			    "LayerPb",
			    worldLV,
			    false,
			    0,
			    checkOverlaps);   
    }
    //always return the physical World
    //
    return worldPV;
  }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //---------------------------------------------------------
  //Chiara 
  
  void DetectorConstruction::ConstructSDandField()
  {
    
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    ChamberSD* chamber = new ChamberSD("chamber");
    sdManager->AddNewDetector(chamber);
    SetSensitiveDetector("Radiator", chamber);
    
    OpticalSD* optical = new OpticalSD("optical");
    sdManager->AddNewDetector(optical);
    SetSensitiveDetector("FakeLayer", optical);
    
  }
  //-----------------------------------------  
}
