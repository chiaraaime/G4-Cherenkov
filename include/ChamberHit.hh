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
/// \file ChamberHit.hh
/// \brief Definition of the ChamberHit class
//

#ifndef ChamberHit_h
#define ChamberHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

namespace RUT
{

class ChamberHit : public G4VHit
{
public:
  ChamberHit();
  ~ChamberHit() override;
  ChamberHit(const ChamberHit& right);
  const ChamberHit& operator=(const ChamberHit& right);
  int operator==(const ChamberHit &right) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void* hit);
  
  void Print() override;
  
  // define here setter methods
  //------------------------------
  //Chiara
  void SetEvent(G4int event)		{ fEvent   = event;	}
  void SetStepNum(G4int step)		{ fStepNum = step; 	}
  void SetEdep(G4double energy) 	{ fEdep	   = energy; 	}
  void SetPos(G4ThreeVector position) 	{ fPos     = position; 	}
  void SetProcessName(G4String name) 	{ fProcess = name; 	}
  void SetPDGID(G4int pdg)		{ fPdgID   = pdg; 	}
  void SetTrackID(G4int track) 		{ fTrackID = track;	}
  void SetParent(G4int parent)		{ fParent  = parent;	}
  void SetTime(G4double time)		{ fTime	   = time;	}
  void SetStepLen(G4double steplength)	{ fStepL   = steplength;}
  void SetMomentum(G4ThreeVector mom)   { fMomentum  = mom;	}
  void SetFirst(G4bool isfirst)		{ fFirst   = isfirst;	}
  void SetKinEne(G4double ke)		{ fKinEne  = ke;	}
  
  // define here getter methods	 
  G4int 	GetEvent()	 const { return fEvent;  }
  G4int  	GetStepNum()	 const { return fStepNum;}
  G4double	GetEdep()	 const { return fEdep;	 }
  G4ThreeVector GetPos()	 const { return fPos;	 }
  G4String	GetProcessName() const { return fProcess;}
  G4int  	GetPDGID()	 const { return fPdgID;  }
  G4int  	GetTrackID()	 const { return fTrackID;}
  G4int  	GetParent()	 const { return fParent; }
  G4double	GetTime()	 const { return fTime;	 }
  G4double	GetStepLen()	 const { return fStepL;  }
  G4ThreeVector GetMomentum()	 const { return fMomentum;}
  G4bool 	GetFirst()	 const { return fFirst;	 }
  G4double	GetKinEne()	 const { return fKinEne; }
  
private:
  // define here data members
  G4int 	 fEvent = -1;
  G4int  	 fStepNum = -1; 
  G4double	 fEdep = 0.;   
  G4ThreeVector  fPos;    
  G4String	 fProcess;
  G4int  	 fPdgID = 0;  
  G4int  	 fTrackID = -1;
  G4int  	 fParent  = 0; 
  G4double	 fTime = 0.;   
  G4double	 fStepL = 0.; 
  G4ThreeVector  fMomentum; 
  G4bool 	 fFirst;
  G4double	 fKinEne = 0;
};

typedef G4THitsCollection<ChamberHit> ChamberHitsCollection;

extern G4Allocator<ChamberHit>* ChamberHitAllocator;

inline void* ChamberHit::operator new(size_t)
{
  if (! ChamberHitAllocator)
        ChamberHitAllocator = new G4Allocator<ChamberHit>;
  return (void*)ChamberHitAllocator->MallocSingle();
}

inline void ChamberHit::operator delete(void* hit)
{
  ChamberHitAllocator->FreeSingle((ChamberHit*) hit);
}

}

#endif


