#include "stepping.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"

MySteppingAction::MySteppingAction() {}
MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    if (!volume) return;

    G4String volName = volume->GetName();

    // Kill all particles inside shield volumes
    if (volName == "FrontPoly") {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}

