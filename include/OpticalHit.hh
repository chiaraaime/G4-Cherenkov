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
/// \file OpticalHit.hh
/// \brief Definition of the OpticalHit class
//

#ifndef OpticalHit_h
#define OpticalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

namespace RUT
{

class OpticalHit : public G4VHit
{
public:
  OpticalHit();
  ~OpticalHit() override;
  OpticalHit(const OpticalHit& right);
  const OpticalHit& operator=(const OpticalHit& right);
  int operator==(const OpticalHit &right) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void* hit);
  
  void Print() override;
  
  // define here setter methods
  //------------------------------
  //Chiara
  void SetEvent(G4int event)		{ fEvent   = event;	}
  void SetKinEne(G4double ke)		{ fKinEne  = ke;	}
  void SetPos(G4ThreeVector position) 	{ fPos     = position; 	}
  void SetProcessName(G4String name) 	{ fProcess = name; 	}

  
  // define here getter methods	 
  G4int 	GetEvent()	 const { return fEvent;  }
  G4double	GetKinEne()	 const { return fKinEne; }
  G4ThreeVector GetPos()	 const { return fPos;	 }
  G4String	GetProcessName() const { return fProcess;}

  
private:
  // define here data members
  G4int 	 fEvent = -1;
  G4double	 fKinEne = 0;
  G4ThreeVector  fPos;    
  G4String	 fProcess;
};

typedef G4THitsCollection<OpticalHit> OpticalHitsCollection;

extern G4Allocator<OpticalHit>* OpticalHitAllocator;

inline void* OpticalHit::operator new(size_t)
{
  if (! OpticalHitAllocator)
        OpticalHitAllocator = new G4Allocator<OpticalHit>;
  return (void*)OpticalHitAllocator->MallocSingle();
}

inline void OpticalHit::operator delete(void* hit)
{
  OpticalHitAllocator->FreeSingle((OpticalHit*) hit);
}

}

#endif


