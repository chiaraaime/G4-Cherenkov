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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "ChamberHit.hh"
#include "G4HCofThisEvent.hh"
#include "OpticalHit.hh" //Chiara per cherenkov
namespace RUT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
	G4cout << ">>> Start event: " << event->GetEventID() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
	G4cout << ">>> End event: " << event->GetEventID() << G4endl;
	
	auto hce = event->GetHCofThisEvent();
	if (!hce) return;
	auto analysisManager = G4AnalysisManager::Instance();
	
	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID("chamber/chamberHitsCollection");

        auto hitsCollection = static_cast<ChamberHitsCollection*>(hce->GetHC(hcID));
        if (hitsCollection){
		G4int nHits = hitsCollection->entries();
        	for (G4int i = 0; i < nHits; i++) {
        		ChamberHit* hit = (*hitsCollection)[i];
	        	analysisManager->FillNtupleDColumn(0, 0, hit->GetEvent());
        		analysisManager->FillNtupleDColumn(0, 1, hit->GetStepNum());    
        		analysisManager->FillNtupleDColumn(0, 2, hit->GetEdep());	  
			analysisManager->FillNtupleDColumn(0, 3, hit->GetPos().x());
			analysisManager->FillNtupleDColumn(0, 4, hit->GetPos().y());
			analysisManager->FillNtupleDColumn(0, 5, hit->GetPos().z());	  
			analysisManager->FillNtupleSColumn(0, 6, hit->GetProcessName());
        		analysisManager->FillNtupleDColumn(0, 7, hit->GetPDGID());	  
        		analysisManager->FillNtupleDColumn(0, 8, hit->GetTrackID());   
        		analysisManager->FillNtupleDColumn(0, 9, hit->GetParent());	  
        		analysisManager->FillNtupleDColumn(0, 10, hit->GetTime());	
        		analysisManager->FillNtupleDColumn(0, 11, hit->GetStepLen());	
        		analysisManager->FillNtupleDColumn(0, 12, hit->GetMomentum().x());
			analysisManager->FillNtupleDColumn(0, 13, hit->GetMomentum().y());
        		analysisManager->FillNtupleDColumn(0, 14, hit->GetMomentum().z());
			analysisManager->FillNtupleDColumn(0, 15, hit->GetFirst());
			analysisManager->FillNtupleDColumn(0, 16, hit->GetKinEne());

        		analysisManager->AddNtupleRow(0);
		}
	}
	

	G4int optHcID = G4SDManager::GetSDMpointer()->GetCollectionID("optical/opticalHitsCollection");
	auto optHitsCollection = static_cast<OpticalHitsCollection*>(hce->GetHC(optHcID));

	if (optHitsCollection) {
		G4int nOptHits = optHitsCollection->entries();
		for (G4int i = 0; i < nOptHits; i++) {
			OpticalHit* hit = (*optHitsCollection)[i];
			
			analysisManager->FillNtupleDColumn(1, 0, hit->GetEvent());
			analysisManager->FillNtupleDColumn(1, 1, hit->GetPos().x());
			analysisManager->FillNtupleDColumn(1, 2, hit->GetPos().y());
			analysisManager->FillNtupleDColumn(1, 3, hit->GetPos().z());
			analysisManager->FillNtupleSColumn(1, 4, hit->GetProcessName());
			analysisManager->FillNtupleDColumn(1, 5, hit->GetKinEne());
			analysisManager->FillNtupleDColumn(1, 6, hit->GetTheta());
			analysisManager->AddNtupleRow(1); 
		}
	}


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
