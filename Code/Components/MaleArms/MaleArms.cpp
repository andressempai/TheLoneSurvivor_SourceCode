#include "Components/MaleArms/MaleArms.h"
#include "Components/DVEntity.h"
#include "Components/LivingPhysic/LivingPhysic.h"
#include "Components/SkeletalMesh/SkeletalMesh.h"
#include "Components/ItemCollector/ItemCollector.h"
#include "Components/Footstep/Footstep.h"
#include "Components/WeaponHolder/WeaponHolder.h"
#include "Components/AIObservable/AIObservable.h"
#include "Components/Input/InputComponent.h"
#include "Components/Action/Look.h"
#include "Components/Action/Locomotion.h"
#include "Components/Action/Shoot.h"
#include "Components/Action/SwitchWeapon.h"
#include "Components/Action/PauseGame.h"
#include "Types/BodyProxyTypes.h"
#include "Types/AnimationType.h"
#include "Types/InventoryTypes.h"
#include "Types/WeaponTypes.h"
#include "Types/UserInterfaceType.h"
#include "EntityEventListeners/EntityAnimationEventListener.h"
#include "EntityEventListeners/EntityAttachEventListener.h"
#include "DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryMath/Cry_Camera.h>
#include <DefaultComponents/Audio/ListenerComponent.h>

void register_male_arms_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(MaleArms));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_male_arms_component);

void ReflectType(Schematyc::CTypeDesc<MaleArms>& desc)
{
	desc.AddBase<Shooter>();
	desc.SetGUID("{6CE6094A-4F07-4EDC-A16C-6AB1F4653215}"_cry_guid);
	desc.SetEditorCategory("Characters");
	desc.SetName(Schematyc::CTypeName{ "malearmscomponent" });
	desc.SetLabel("Male Arms");
	desc.SetDescription("Male Arms Character");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::HiddenFromUser });
}

Shooter::ViewPoint MaleArms::get_view_point() const
{
	const auto camera_matrix = gEnv->pSystem->GetViewCamera().GetMatrix();
	return { camera_matrix.GetColumn3(), camera_matrix.GetColumn1() };
}

void MaleArms::Initialize()
{
	using MaleArmsMeshChild = MP::type_list<Types::Components::PistolComponent, Types::Components::AmericanShotgunComponent>;
	using MaleArmsMeshChildEventListener = MP::type_list<EntityEventListeners::EntityAttachEventListener>;
	using MaleArmsMeshComponentList = MP::type_list<
			Types::Components::PlayerHUD
		,	SkeletalMesh
		,	Types::Components::PlayerBodyProxyComponent
		,	Types::Components::PlayerAnimationComponent
		,	Types::Components::WeaponInventoryComponent
		,	WeaponHolder
		,	Shoot
		,	SwitchWeapon
	>;
	using PlayerMeshEntity = DVEntity<
			int
		,	MaleArmsMeshChild
		,	MaleArmsMeshChildEventListener
		,	MaleArmsMeshComponentList
		,	Data::MaleArms
		,	ComponentData::PlayerEntityMesh
	>;

	using MaleArmsPhysicChild = MP::type_list<PlayerMeshEntity>;
	using MaleArmsPhysicChildEventListener = MP::type_list<EntityEventListeners::EntityAnimationEventListener>;
	using MaleArmsPhysicComponentList = MP::type_list<
			LivingPhysic
		,	ItemCollector
		,	Footstep
		,	InputComponent
		,	Locomotion
		,	Look
		,	PauseGame
		,	Cry::Audio::DefaultComponents::CListenerComponent
	>;
	using MaleArmsPhysicEntity = DVEntity<
			int
		,	MaleArmsPhysicChild
		,	MaleArmsPhysicChildEventListener
		,	MaleArmsPhysicComponentList
		,	Data::MaleArms
		,	ComponentData::PlayerEntityPhysic
	>;

	m_pEntity->GetOrCreateComponentClass<MaleArmsPhysicEntity>();
}