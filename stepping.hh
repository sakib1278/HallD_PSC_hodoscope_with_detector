#ifndef MYSTEPPINGACTION_HH
#define MYSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class MySteppingAction : public G4UserSteppingAction
{
public:
    MySteppingAction();
    ~MySteppingAction() override;

    void UserSteppingAction(const G4Step* step) override;
};

#endif

