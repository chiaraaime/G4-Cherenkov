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
/// \file OpticalSD.cc
/// \brief Implementation of the OpticalSD class
//
//Chiara: decide quando creare un hit

#include "OpticalSD.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh"
//#include "g4root.hh"

namespace RUT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalSD::OpticalSD(const G4String& name)
 : G4VSensitiveDetector(name)
{
  collectionName.insert("opticalHitsCollection"); //Chiara debug
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalSD::~OpticalSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpticalSD::Initialize(G4HCofThisEvent* hce)
{
  G4String hcName = SensitiveDetectorName + "HitsCollection";

  fHitsCollection
    = new OpticalHitsCollection(SensitiveDetectorName, hcName);

  //Chiara
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  hce->AddHitsCollection(hcID, fHitsCollection);
  //Chiara
  //G4cout << "Chiara: " << SensitiveDetectorName << " " << hcID << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool OpticalSD::ProcessHits(G4Step* step,
                              G4TouchableHistory* /*history*/)
{
  auto track = step->GetTrack();
  // 1. FILTER: Only optical photons
  if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  G4StepPoint* prePoint = step->GetPreStepPoint();
  // 2. FILTER: is in the volume
  if (prePoint->GetStepStatus() != fGeomBoundary) return false;
  // Create new hit
  OpticalHit* newHit = new OpticalHit();
  // Add here the information to the hit
  //-----------------------------------------------
  //Chiara
  //-------------------event 
  G4int ev = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  newHit->SetEvent(ev);
  //-------------------energy

  G4double kene = prePoint->GetKineticEnergy();
  newHit->SetKinEne(kene);
  
  //------------------position
  newHit->SetPos(prePoint->GetPosition());
  //------------------process
  const G4VProcess* process = track->GetCreatorProcess();
  G4String processName = process ? process->GetProcessName() : "undefined";
  newHit->SetProcessName(processName);
 
  //------------------theta
  G4double theta = track->GetMomentumDirection().theta();
  newHit->SetTheta(theta);
  fHitsCollection->insert(newHit);
  //Kill the optical photon
  track->SetTrackStatus(fStopAndKill);
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpticalSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
  //G4cout << "\n-------->" <<  fHitsCollection->GetName()
         //<< ": in this event: " << G4endl;

  G4int nofHits = fHitsCollection->entries();
  for ( G4int i=0; i<nofHits; i++ ) {
    (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

