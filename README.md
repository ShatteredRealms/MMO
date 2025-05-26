# Shattered Realms Online
The game for [Shattered Realms Online](https://shatteredrealmsonline.com) built in [Unreal Engine](https://github.com/EpicGames/UnrealEngine).

# Overview
The goal is to use modular game frameworks and plugins wherever possible to increase reusability.

## Plugins
* [Agones](Plugins/Agones): Copied from [googleforgames/agones](https://github.com/googleforgames/agones) with minor modifications where necessary to make it work for the latest UE version. SDK client for communication with Agones backend (gameserver management.)
* [AsyncMixin](Plugins/AsyncMixin): Copied from [Lyra](https://github.com/EpicGames/UnrealEngine/blob/release/Samples/Games/Lyra/Plugins/AsyncMixin). Provides useful utilies for async tasks.
* [CommonLoadingScreen](Plugins/CommonLoadingScreen): Copied from [Lyra](https://github.com/EpicGames/UnrealEngine/blob/release/Samples/Games/Lyra/Plugins/CommonLoadingScreen). Provides loading screen for startup and moments when the game is
loading or still processing
* [GameplayMessageRouter](Plugins/GameplayMessageRouter): Copied from [Lyra](https://github.com/EpicGames/UnrealEngine/blob/release/Samples/Games/Lyra/Plugins/GameplayMessageRouter). Provides means to broadcast and receieve UStructs with GameplayTags.
* [GameSettings](Plugins/GameSettings): Copied from [Lyra](https://github.com/EpicGames/UnrealEngine/blob/release/Samples/Games/Lyra/Plugins/GameSettings). Provides framework for handling user setting such as video, audio, and input.
* [JWTPlugin](Plugins/JWTPlugin): Copied from [sha3sha3/JWTPlugin](https://github.com/sha3sha3/JWTPlugin). Provides UE native handling of JWT.
* [ModularWindows](Plugins/ModularWindows): Provides framework for game windows that can be moved or resized. Some features include titles, tabs and close buttons
* [TargetingSystem](Plugins/TargetingSystem): Framework defining `Targetable` and a sub-class `Attackable` actors. These actors have a component attached that adds a targeting widget that can be used to display information about the target. The widget can
also be used to create a HUD UI widget to show the target details. 
* [TuboLink](Plugins/TurboLink): Fork of [thejinchao/tubrolink](https://github.com/thejinchao/turbolink) and [thejinchao/protoc-gen-turbolink](https://github.com/thejinchao/protoc-gen-turbolink). Provides a framework for using gRPC services and Proto files within UE. The proto generator creates the UE classes from `proto` files.

# Development
## Generating Proto files
The build script assumes the [Go Backend](https://github.com/ShatteredRealms/go-backend) is a folder called `go-backend` in the same folder root as this git repository. It pulls the latest changes from git and uses `protoc-gen-turbolink` to create
UE TurboLink/gRPC files. The files are stored in [Plugins/TurboLink/Source/TurboLinkGrpc/](Plugins/TurboLink/Source/TurboLinkGrpc/) with the folder being the package name and service name. Simply run the `build_protos.cmd` command in the [Tools](Tools) folder
and the files will be generated in the correct location.
