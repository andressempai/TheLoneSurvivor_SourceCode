cmake_minimum_required (VERSION 3.14)
set(CRYENGINE_DIR "C:/Users/andre/AppData/Local/Programs/CRYENGINE Launcher/Engines/crytek/cryengine-57-lts/5.7.1" CACHE STRING "CRYENGINE root directory.")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/InitialSetup.cmake")
include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"GamePlugin.cpp"
		"StdAfx.cpp"
		"BodyManager.h"
		"db_perlin.h"
		"DevelopmentTools.h"
		"EventSystem.h"
		"GamePlugin.h"
		"MetaProgramming.h"
		"StdAfx.h"
		"Timer.h"
		"TimerManager.h"
)
add_sources("BehaviorTreeNodes_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "BehaviorTreeNodes"
		"BehaviorTreeNodes/Cooldown.cpp"
		"BehaviorTreeNodes/DistanceCheck.cpp"
		"BehaviorTreeNodes/ForceFailure.cpp"
		"BehaviorTreeNodes/ForceRunning.cpp"
		"BehaviorTreeNodes/ForceSuccess.cpp"
		"BehaviorTreeNodes/MeleeAttack.cpp"
		"BehaviorTreeNodes/Search.cpp"
		"BehaviorTreeNodes/SetFlag.cpp"
		"BehaviorTreeNodes/StopSearch.cpp"
		"BehaviorTreeNodes/TryStunned.cpp"
		"BehaviorTreeNodes/TurnToEngageEnemy.cpp"
		"BehaviorTreeNodes/BehaviorTreeNodesRegistrator.h"
		"BehaviorTreeNodes/CompareKeyWithValue.h"
		"BehaviorTreeNodes/Cooldown.h"
		"BehaviorTreeNodes/DistanceCheck.h"
		"BehaviorTreeNodes/ForceFailure.h"
		"BehaviorTreeNodes/ForceRunning.h"
		"BehaviorTreeNodes/ForceSuccess.h"
		"BehaviorTreeNodes/MeleeAttack.h"
		"BehaviorTreeNodes/Search.h"
		"BehaviorTreeNodes/SetFlag.h"
		"BehaviorTreeNodes/StopSearch.h"
		"BehaviorTreeNodes/TryStunned.h"
		"BehaviorTreeNodes/TurnToEngageEnemy.h"
)
add_sources("BlackboardKeys_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "BlackboardKeys"
		"BlackboardKeys/BlackboardKeys.h"
)
add_sources("Components_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components"
		"Components/BodyProxy.h"
		"Components/DVEntity.h"
		"Components/Shooter.h"
		"Components/TAnimation.h"
		"Components/UserInterface.h"
)
add_sources("Action_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Action"
		"Components/Action/Locomotion.cpp"
		"Components/Action/Look.cpp"
		"Components/Action/PauseGame.cpp"
		"Components/Action/Shoot.cpp"
		"Components/Action/SwitchWeapon.cpp"
		"Components/Action/Locomotion.h"
		"Components/Action/Look.h"
		"Components/Action/PauseGame.h"
		"Components/Action/Shoot.h"
		"Components/Action/SwitchWeapon.h"
)
add_sources("AIBehaviorTree_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\AIBehaviorTree"
		"Components/AIBehaviorTree/AIBehaviorTree.cpp"
		"Components/AIBehaviorTree/AIBehaviorTree.h"
)
add_sources("AIListener_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\AIListener"
		"Components/AIListener/AIListener.cpp"
		"Components/AIListener/AIListener.h"
)
add_sources("AIObservable_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\AIObservable"
		"Components/AIObservable/AIObservable.cpp"
		"Components/AIObservable/AIObservable.h"
)
add_sources("AIObserver_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\AIObserver"
		"Components/AIObserver/AIObserver.cpp"
		"Components/AIObserver/AIObserver.h"
)
add_sources("Animation_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Animation"
		"Components/Animation/Animation.cpp"
		"Components/Animation/PerderosAnimation.cpp"
		"Components/Animation/ZombieAnimation.cpp"
		"Components/Animation/Animation.h"
		"Components/Animation/PerderosAnimation.h"
		"Components/Animation/ZombieAnimation.h"
)
add_sources("BulletproofVest_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\BulletproofVest"
		"Components/BulletproofVest/BulletproofVest.cpp"
		"Components/BulletproofVest/BulletproofVest.h"
)
add_sources("Checkpoint_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Checkpoint"
		"Components/Checkpoint/Checkpoint.cpp"
		"Components/Checkpoint/Checkpoint.h"
)
add_sources("CollectableSpawner_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\CollectableSpawner"
		"Components/CollectableSpawner/CollectableSpawner.cpp"
		"Components/CollectableSpawner/CollectableSpawner.h"
)
add_sources("DynamicMesh_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\DynamicMesh"
		"Components/DynamicMesh/DynamicMesh.cpp"
		"Components/DynamicMesh/DynamicMesh.h"
)
add_sources("EnemySpawner_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\EnemySpawner"
		"Components/EnemySpawner/EnemySpawner.cpp"
		"Components/EnemySpawner/EnemySpawner.h"
)
add_sources("ExplosiveBarrel_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\ExplosiveBarrel"
		"Components/ExplosiveBarrel/ExplosiveBarrel.cpp"
		"Components/ExplosiveBarrel/ExplosiveBarrel.h"
)
add_sources("Footstep_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Footstep"
		"Components/Footstep/Footstep.cpp"
		"Components/Footstep/Footstep.h"
)
add_sources("Input_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Input"
		"Components/Input/InputComponent.cpp"
		"Components/Input/InputComponent.h"
)
add_sources("InventoryController_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\InventoryController"
		"Components/InventoryController/InventoryController.h"
)
add_sources("ItemCollector_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\ItemCollector"
		"Components/ItemCollector/ItemCollector.cpp"
		"Components/ItemCollector/ItemCollector.h"
)
add_sources("LivingPhysic_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\LivingPhysic"
		"Components/LivingPhysic/LivingPhysic.cpp"
		"Components/LivingPhysic/LivingPhysic.h"
)
add_sources("Magazine_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Magazine"
		"Components/Magazine/Magazine.cpp"
		"Components/Magazine/Magazine.h"
)
add_sources("MaleArms_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\MaleArms"
		"Components/MaleArms/MaleArms.cpp"
		"Components/MaleArms/MaleArms.h"
)
add_sources("MedKit_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\MedKit"
		"Components/MedKit/MedKit.cpp"
		"Components/MedKit/MedKit.h"
)
add_sources("Muzzle_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Muzzle"
		"Components/Muzzle/Muzzle.cpp"
		"Components/Muzzle/Muzzle.h"
)
add_sources("MuzzleFlash_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\MuzzleFlash"
		"Components/MuzzleFlash/MuzzleFlash.cpp"
		"Components/MuzzleFlash/MuzzleFlash.h"
)
add_sources("Perderos_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Perderos"
		"Components/Perderos/Perderos.cpp"
		"Components/Perderos/Perderos.h"
)
add_sources("SkeletalMesh_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\SkeletalMesh"
		"Components/SkeletalMesh/SkeletalMesh.cpp"
		"Components/SkeletalMesh/SkeletalMesh.h"
)
add_sources("StoreToMagazine_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\StoreToMagazine"
		"Components/StoreToMagazine/StoreToMagazine.cpp"
		"Components/StoreToMagazine/StoreToMagazine.h"
)
add_sources("WeaponHolder_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\WeaponHolder"
		"Components/WeaponHolder/WeaponHolder.cpp"
		"Components/WeaponHolder/WeaponHolder.h"
)
add_sources("Zombie_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Components\\\\Zombie"
		"Components/Zombie/Zombie.cpp"
		"Components/Zombie/Zombie.h"
)
add_sources("Datas_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Datas"
		"Datas/AmmunitionsData.h"
		"Datas/BehaviorTreeNodesDatas.h"
		"Datas/CharacterData.h"
		"Datas/ComponentsData.h"
		"Datas/UserInterfaceData.h"
		"Datas/WeaponsData.h"
)
add_sources("EntityEventListeners_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "EntityEventListeners"
		"EntityEventListeners/EntityAnimationEventListener.cpp"
		"EntityEventListeners/EntityAttachEventListener.cpp"
		"EntityEventListeners/EntityAnimationEventListener.h"
		"EntityEventListeners/EntityAttachEventListener.h"
)
add_sources("GameEvents_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "GameEvents"
)
add_sources("Animation_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "GameEvents\\\\Animation"
		"GameEvents/Animation/SubscribeToBeforeSwitchWeaponEvent.cpp"
		"GameEvents/Animation/SubscribeToEquipWeaponEvent.cpp"
		"GameEvents/Animation/SubscribeToPullTriggerEvent.cpp"
		"GameEvents/Animation/SubscribeToRecoilEvent.cpp"
		"GameEvents/Animation/SubscribeToSwitchWeaponEvent.cpp"
		"GameEvents/Animation/SubscribeToBeforeSwitchWeaponEvent.h"
		"GameEvents/Animation/SubscribeToEquipWeaponEvent.h"
		"GameEvents/Animation/SubscribeToPullTriggerEvent.h"
		"GameEvents/Animation/SubscribeToRecoilEvent.h"
		"GameEvents/Animation/SubscribeToSwitchWeaponEvent.h"
)
add_sources("GameEventsBody_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "GameEvents\\\\Body"
		"GameEvents/Body/SubscribeToAIDeadEvent.cpp"
		"GameEvents/Body/SubscribeToDeadEvent.cpp"
		"GameEvents/Body/SubscribeToExplosiveDamageEvent.cpp"
		"GameEvents/Body/SubscribeToHealEvent.cpp"
		"GameEvents/Body/SubscribeToPointDamageEvent.cpp"
		"GameEvents/Body/SubscribeToRepairArmorEvent.cpp"
		"GameEvents/Body/SubscribeToAIDeadEvent.h"
		"GameEvents/Body/SubscribeToAIExplosiveDamageEvent.h"
		"GameEvents/Body/SubscribeToAIPointDamageEvent.h"
		"GameEvents/Body/SubscribeToDeadEvent.h"
		"GameEvents/Body/SubscribeToExplosiveDamageEvent.h"
		"GameEvents/Body/SubscribeToHealEvent.h"
		"GameEvents/Body/SubscribeToPointDamageEvent.h"
		"GameEvents/Body/SubscribeToRepairArmorEvent.h"
)
add_sources("GameEventsUI_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "GameEvents\\\\UI"
		"GameEvents/UI/SubscribeToArmorUpdateEvent.cpp"
		"GameEvents/UI/SubscribeToHealthUpdateEvent.cpp"
		"GameEvents/UI/SubscribeToMagazineUpdateEvent.cpp"
		"GameEvents/UI/SubscribeToArmorUpdateEvent.h"
		"GameEvents/UI/SubscribeToHealthUpdateEvent.h"
		"GameEvents/UI/SubscribeToMagazineUpdateEvent.h"
)
add_sources("Mixin_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Mixin"
		"Mixin/Explosive.h"
)
add_sources("Types_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Types"
		"Types/AmmunitionTypes.h"
		"Types/AnimationType.h"
		"Types/BehaviorTreeNodesTypes.h"
		"Types/BodyProxyTypes.h"
		"Types/InventoryTypes.h"
		"Types/UserInterfaceType.h"
		"Types/WeaponTypes.h"
)
add_sources("UIListeners_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UIListeners"
		"UIListeners/QuitGameListener.cpp"
		"UIListeners/ResumeGameListener.cpp"
		"UIListeners/RetryGameListener.cpp"
		"UIListeners/ReturnMainMenuListener.cpp"
		"UIListeners/StartGameListener.cpp"
		"UIListeners/QuitGameListener.h"
		"UIListeners/ResumeGameListener.h"
		"UIListeners/RetryGameListener.h"
		"UIListeners/ReturnMainMenuListener.h"
		"UIListeners/StartGameListener.h"
)
add_sources("UserCode_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UserCode"
)
add_sources("Animation_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "UserCode\\\\Animation"
		"UserCode/Animation/PlayerAnimation.cpp"
		"UserCode/Animation/Weapon.cpp"
		"UserCode/Animation/PlayerAnimation.h"
		"UserCode/Animation/Weapon.h"
)
add_sources("Utils_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Utils"
		"Utils/AmmoBox.h"
		"Utils/Armor.h"
		"Utils/AudioTriggerParams.h"
		"Utils/Body.h"
		"Utils/BuoyancyParams.h"
		"Utils/CameraInfo.h"
		"Utils/CharacterDimensions.h"
		"Utils/CharacterDynamics.h"
		"Utils/DamageInfo.h"
		"Utils/DynamicMeshType.h"
		"Utils/HealthPack.h"
		"Utils/MeshParams.h"
		"Utils/ObjectPointer.h"
		"Utils/Ray.h"
		"Utils/ShakeParams.h"
		"Utils/SimulationParams.h"
		"Utils/Tags.h"
		"Utils/WeaponBox.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryGameModule(Game FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-variable
        -Wno-reorder
        -Wno-unknown-pragmas
        -Wno-parentheses
        -Wno-switch
        -Wno-format
        -Wno-dynamic-class-memaccess
        -Wno-unused-private-field
        -Wno-unused-value
        -Wno-invalid-offsetof
        -Wno-multichar
        -Wno-char-subscripts
        -Wno-null-conversion
        -Wno-empty-body
        -Wno-unused-lambda-capture
        -Wno-unused-function
        -Wno-tautological-constant-out-of-range-compare
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-aligned-new
        -Wno-int-in-bool-context
        -Wno-invalid-offsetof
        -Wno-comment
        -Wno-sign-compare
        -Wno-unused-but-set-variable
        -Wno-maybe-uninitialized
        -Wno-misleading-indentation
        -Wno-unused-result
        -Wno-unknown-pragmas
        -Wno-multichar
        -Wno-strict-aliasing
        -Wno-char-subscripts
        -Wno-conversion-null
        -Wno-reorder
        -Wno-parentheses
        -Wno-format
        -Wno-switch
    )
endif()


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
        add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
        set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
        if (WIN32)
            set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
        endif()
    endif()
else()
    add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()

    add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"")
    endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/Game.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM