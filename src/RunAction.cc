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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"

//#include "g4root.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

namespace RUT
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // Create analysis manager
  //Chiara
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  analysisManager->CreateNtuple("Radiator", "Radiator hits");
  analysisManager->CreateNtupleDColumn("Event");
  analysisManager->CreateNtupleDColumn("Step");
  analysisManager->CreateNtupleDColumn("DepositedEnergy");
  analysisManager->CreateNtupleDColumn("xPos");
  analysisManager->CreateNtupleDColumn("yPos");
  analysisManager->CreateNtupleDColumn("zPos");
  analysisManager->CreateNtupleSColumn("Process");
  analysisManager->CreateNtupleDColumn("PDG");
  analysisManager->CreateNtupleDColumn("TrackID");
  analysisManager->CreateNtupleDColumn("ParentID");
  analysisManager->CreateNtupleDColumn("Time");
  analysisManager->CreateNtupleDColumn("StepLength");
  analysisManager->CreateNtupleDColumn("Px");
  analysisManager->CreateNtupleDColumn("Py");
  analysisManager->CreateNtupleDColumn("Pz");
  analysisManager->CreateNtupleDColumn("FirstStep");
  analysisManager->CreateNtupleDColumn("KineticEnergy");

  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("Optical", "Optical photons");
  analysisManager->CreateNtupleDColumn("Event");
  analysisManager->CreateNtupleDColumn("xPos");
  analysisManager->CreateNtupleDColumn("yPos");
  analysisManager->CreateNtupleDColumn("zPos");
  analysisManager->CreateNtupleSColumn("Process");
  analysisManager->CreateNtupleDColumn("KineticEnergy");
  
  analysisManager->FinishNtuple();
  // Creating histograms and ntuples
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // Get analysis manager and open the output file
  //Chiara
  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "myfile.root";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // save histograms and close the output files
  //Chiara
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
