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
/// \file ChamberSD.cc
/// \brief Implementation of the ChamberSD class
//
//Chiara: decide quando creare un hit

#include "ChamberSD.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh" //Chiara per cherenkov
//#include "g4root.hh"

namespace RUT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChamberSD::ChamberSD(const G4String& name)
 : G4VSensitiveDetector(name)
{
  collectionName.insert("chamberHitsCollection"); //Chiara debug
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChamberSD::~ChamberSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChamberSD::Initialize(G4HCofThisEvent* hce)
{
  G4String hcName = SensitiveDetectorName + "HitsCollection";

  fHitsCollection
    = new ChamberHitsCollection(SensitiveDetectorName, hcName);

  //Chiara
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  hce->AddHitsCollection(hcID, fHitsCollection);
  //Chiara
  //G4cout << "Chiara: " << SensitiveDetectorName << " " << hcID << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ChamberSD::ProcessHits(G4Step* step,
                              G4TouchableHistory* /*history*/)
{
  auto track = step->GetTrack();
  if(track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) return false;
  
  G4double edep = step->GetTotalEnergyDeposit();
  //G4cout << "Chiara ProcessHit called, edep = " << edep << G4endl;
  if(edep==0.) return false;
  // Create new hit
  ChamberHit* newHit = new ChamberHit();
  // Add here the information to the hit
  //-----------------------------------------------
  //Chiara
  //-------------------event 
  G4int ev = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  newHit->SetEvent(ev);
  //-------------------energy
  
  newHit->SetEdep(edep);
  
  G4StepPoint* prePoint  = step->GetPreStepPoint();
  G4StepPoint* postPoint = step->GetPostStepPoint();
  //------------------position
  newHit->SetPos(postPoint->GetPosition());
  //------------------process
  const G4VProcess* process = track->GetCreatorProcess();
  G4String processName = process ? process->GetProcessName() : "undefined";
  newHit->SetProcessName(processName);
  //-----------------step
  G4int stp = track->GetCurrentStepNumber();
  newHit->SetStepNum(stp);
  //-----------------pdgid
  G4int pdg = track->GetDefinition()->GetPDGEncoding();
  newHit->SetPDGID(pdg);
  //-----------------trackid  
  G4int trkid = track->GetTrackID();
  newHit->SetTrackID(trkid);
  //-----------------momentum
  G4ThreeVector momentum = prePoint->GetMomentum(); 
  newHit->SetMomentum(momentum);
  //-----------------parent 
  G4int parent = track->GetParentID();
  newHit->SetParent(parent);
  //----------------time
  G4double time = postPoint->GetGlobalTime();
  newHit->SetTime(time);
  //----------------steplength
  G4double length = step->GetStepLength();
  newHit->SetStepLen(length);
  //----------------first
  G4bool first = (track->GetCurrentStepNumber() == 1);
  newHit->SetFirst(first);
  //----------------kinetic
  G4double kene = track->GetVertexKineticEnergy();
  newHit->SetKinEne(kene);
  /*
  if (std::abs(pdg) == 11 && first) {
    G4cout << "ci siamo " << track->GetKineticEnergy() << G4endl;
    if (track->GetKineticEnergy() > 3.0) {
        G4cout << ">>> ELETTRONE DIRETTO TROVATO! <<<" << G4endl;
        G4cout << "Energia: " << track->GetKineticEnergy() << " MeV " << kene << G4endl;
        G4cout << "Nome Processo: " << processName << G4endl;
        G4cout << "Il vero ParentID da usare è: " << parent << G4endl;
        G4cout << "-----------------------------------" << G4endl;
    }
}
*/
  
  // Add hit in the collection
  fHitsCollection->insert(newHit);
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChamberSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
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

