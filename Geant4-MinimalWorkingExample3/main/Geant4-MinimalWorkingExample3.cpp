#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyPrimaryGeneratorAction.hh"

#include "Randomize.hh"
#include "time.h"

int main(int argc, char** argv)
{
	G4RunManager *runManager = new G4RunManager;

	// Randomise the gun firing sequence, 
	// otherwise each simulation run would be the same.
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
	G4long seed = time(NULL);
	CLHEP::HepRandom::setTheSeed(seed);

	MyDetectorConstruction *detectorConstruction = new MyDetectorConstruction;
	runManager->SetUserInitialization(detectorConstruction);

	runManager->SetUserInitialization(new MyPhysicsList);

	G4VUserPrimaryGeneratorAction *generatorAction =
			new MyPrimaryGeneratorAction(
					detectorConstruction->GetHalfLabSize());
	runManager->SetUserAction(generatorAction);

	G4VisManager *visManager = new G4VisExecutive;
	visManager->Initialize();

	G4UImanager *uiManager = G4UImanager::GetUIpointer();
	G4UIExecutive *uiExecutive = new G4UIExecutive(argc, argv);

	uiManager->ApplyCommand("/control/execute visualisation.macro");
	uiExecutive->SessionStart();

	delete uiExecutive;
	delete visManager;
    delete runManager;
	return 0;
}
