# 3D-Asteroids
Project idea copied over from a previous repo that never got going. Intended to be an experiment with the now fully released UE5 to make a 3D version of the classic game "Asteriods"

# Development Notes
The original plan for handling gameplay classes (GameMode, DefaultPawn, HUD, GameState, etc.) was to manage everything through C++. This can be done by setting GameMode member classes
in the constructor for a GameMode override using <SomeClass>::StaticClass(). This gets complicated with Pawns (at least), which have a mesh component. Assigning this in C++ is still
technically possible, but creates an undesireable hardcoded filepath reference to an asset. It is much easier to create a Blueprint child of the C++ class and assign visual components 
there. In turn this requires a Blueprint child of the GameMode, since references to Blueprints assets from within C++ are done in a similar way. The base AsteroidsGameMode then is used
for the Project-wide GameMode, which sets C++ class defaults. BP_AsteroidsGameModeBase should be used in individual worlds, with overrides to necessary Blueprint classes for its defaults.
Overall it seems best to keep actors containing visual components to a base C++ class with a Blueprint override setting the components as desired.