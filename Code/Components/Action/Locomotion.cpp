#include "Locomotion.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryGame/IGameFramework.h>

void register_locomotion_action_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Locomotion));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_locomotion_action_component);

void ReflectType(Schematyc::CTypeDesc<Locomotion>& desc)
{
	desc.SetGUID("{66830988-F074-4FD8-8338-0755663D8660}"_cry_guid);
	desc.SetEditorCategory("Game Actions");
	desc.SetName(Schematyc::CTypeName("locomotionactioncomponent"));
	desc.SetLabel("Locomotion");
	desc.SetDescription("Allow Entity to Move over the World");
	desc.SetComponentFlags(EEntityComponentFlags::Singleton);
	desc.AddMember(
			&Locomotion::walk_speed_
		,	'wspe'
		,	"walkspeed"
		,	"Walk Speed"
		,	"Speed of the Entity when is standing"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&Locomotion::jump_height_
		,	'jhei'
		,	"jumpheight"
		,	"Jump Height"
		,	"Force applied to the Entity when is jumping"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&Locomotion::jump_buffer_
		,	'jbuf'
		,	"jumpbuffer"
		,	"Jump Buffer"
		,	"Time to buffer the jump action"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&Locomotion::stand_eyes_height_
		,	'seye'
		,	"standingeyesheight"
		,	"Standing Eyes Height"
		,	"Height of the eyes when the Entity is standing"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&Locomotion::stand_height_
		,	'shgt'
		,	"standingheight"
		,	"Standing Height"
		,	"Height of the Entity when is standing"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&Locomotion::crouch_eyes_height_
		,	'ceye'
		,	"crouchingeyesheight"
		,	"Crouching Eyes Height"
		,	"Height of the eyes when the Entity is crouching"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&Locomotion::crouch_height_
		,	'chgt'
		,	"crouchingheight"
		,	"Crouching Height"
		,	"Height of the Entity when is crouching"
		,	Schematyc::PositiveFloat{}
	);

	desc.AddMember(
			&Locomotion::collider_radius_
		,	'crad'
		,	"colliderradius"
		,	"Collider Radius"
		,	"Radius of the Entity Collider"
		,	Schematyc::PositiveFloat{}
	);
}

void Locomotion::Initialize()
{
	DevelopmentTools::add_action_listener(this, "player");

#ifndef RELEASE
	debugger_ = gEnv->pGameFramework->GetIPersistantDebug();
#endif // !RELEASE
}

Cry::Entity::EventFlags Locomotion::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void Locomotion::ProcessEvent(const SEntityEvent& event)
{
	using namespace DevelopmentTools;

	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;

	const auto velocity = [this]
	{
		auto result = make_zero_vector3();
		if (input_actions_.test(get_underlying_value_from_enum(EInputActions::move_forward)))
			result.y += 1.0f;
		if (input_actions_.test(get_underlying_value_from_enum(EInputActions::move_back)))
			result.y -= 1.0f;
		if (input_actions_.test(get_underlying_value_from_enum(EInputActions::move_right)))
			result.x += 1.0f;
		if (input_actions_.test(get_underlying_value_from_enum(EInputActions::move_left)))
			result.x -= 1.0f;

		return result;
	}();

	const auto camera_rotation = Quat(gEnv->pSystem->GetViewCamera().GetMatrix());
	const auto yaw_rotation = Quat::CreateRotationZ(camera_rotation.GetRotZ());
	const auto direction = yaw_rotation * velocity.GetNormalized() * walk_speed_;
	move_entity_by_physics(*m_pEntity, direction, event.fParam[0]);

	auto& timer_manager = TimerManager::instance();
	if (input_actions_.test(get_underlying_value_from_enum(EInputActions::jumping)))
		timer_manager.make_timer(jump_buffer_timer_id_, jump_buffer_.value, false, nullptr);

	if (timer_manager.is_timer_active(jump_buffer_timer_id_)) {
		if (const auto status = get_physics_status<pe_status_living>(*m_pEntity); not status.bFlying) {
			launch_entity_by_physics(*m_pEntity, make_vector3(0.0f, 0.0f, jump_height_));
			timer_manager.remove_timer(jump_buffer_timer_id_);
		}
	}

	// CROUCH
	if (const auto is_crouch_action_active = input_actions_.test(get_underlying_value_from_enum(EInputActions::crouching));
		is_crouching_ != is_crouch_action_active) {
		is_crouching_ = is_crouch_action_active;

		if (is_crouching_) {
			target_eyes_height_ = crouch_eyes_height_;
			target_height_ = crouch_height_;
		}
		else {
			const auto stand_capsule = [&]
			{
				const auto radius = collider_radius_ * 0.5f;
				const auto height = stand_eyes_height_ * 0.25f;

				auto capsule = primitives::capsule{};
				capsule.center = m_pEntity->GetWorldPos() + make_vector3(0.0f, 0.0f, radius + height + 0.05f);
				capsule.axis = make_up_vector3();
				capsule.r = radius;
				capsule.hh = height;
#ifndef RELEASE
				debugger_->Begin("CrouchDebbuger", true);
				debugger_->AddCylinder(capsule.center, capsule.axis, capsule.r, capsule.hh / 0.5f, Col_Red, 10);
#endif // !RELEASE
				return capsule;
			}();

			auto entities_to_skip = std::array{ m_pEntity->GetPhysicalEntity() };

			auto inter_params = intersection_params{};
			inter_params.bSweepTest = false;

			auto pwi_params = IPhysicalWorld::SPWIParams{};
			pwi_params.itype = stand_capsule.type;
			pwi_params.pprim = &stand_capsule;
			pwi_params.pSkipEnts = entities_to_skip.data();
			pwi_params.nSkipEnts = 1;
			pwi_params.pip = &inter_params;

			if (gEnv->pPhysicalWorld->PrimitiveWorldIntersection(pwi_params) == 0.0f) {
				target_eyes_height_ = stand_eyes_height_;
				target_height_ = stand_height_;
			}
			else {
				input_actions_.set(get_underlying_value_from_enum(EInputActions::crouching));
				is_crouching_ = true;
			}
		}
	}

	auto dimensions = get_physics_params<pe_player_dimensions>(*m_pEntity);
	dimensions.heightEye = LERP(dimensions.heightEye, target_eyes_height_, 8.35f * event.fParam[0]);
	dimensions.sizeCollider.z = target_height_ * 0.25f;
	dimensions.heightCollider = dimensions.sizeCollider.x + dimensions.sizeCollider.z + 0.05f;

	update_physics_params(*m_pEntity, dimensions);

	input_actions_.set(get_underlying_value_from_enum(EInputActions::move_forward), false);
	input_actions_.set(get_underlying_value_from_enum(EInputActions::move_back), false);
	input_actions_.set(get_underlying_value_from_enum(EInputActions::move_right), false);
	input_actions_.set(get_underlying_value_from_enum(EInputActions::move_left), false);
	input_actions_.set(get_underlying_value_from_enum(EInputActions::jumping), false);
}

void Locomotion::OnAction(const ActionId& action, int activation_mode, float value)
{
	if ((strcmp(action.c_str(), "moveforward") == 0))
		if (activation_mode == eAAM_OnHold)
			input_actions_.set(DevelopmentTools::get_underlying_value_from_enum(EInputActions::move_forward));

	if ((strcmp(action.c_str(), "moveback") == 0))
		if (activation_mode == eAAM_OnHold)
			input_actions_.set(DevelopmentTools::get_underlying_value_from_enum(EInputActions::move_back));

	if ((strcmp(action.c_str(), "moveleft") == 0))
		if (activation_mode == eAAM_OnHold)
			input_actions_.set(DevelopmentTools::get_underlying_value_from_enum(EInputActions::move_left));

	if ((strcmp(action.c_str(), "moveright") == 0))
		if (activation_mode == eAAM_OnHold)
			input_actions_.set(DevelopmentTools::get_underlying_value_from_enum(EInputActions::move_right));

	if ((strcmp(action.c_str(), "moveup") == 0))
		if (activation_mode == eAAM_OnPress)
			input_actions_.set(DevelopmentTools::get_underlying_value_from_enum(EInputActions::jumping));

	if ((strcmp(action.c_str(), "movedown") == 0))
		if (activation_mode == eAAM_OnPress)
			input_actions_.set(
					DevelopmentTools::get_underlying_value_from_enum(EInputActions::crouching)
				,	!input_actions_.test(DevelopmentTools::get_underlying_value_from_enum(EInputActions::crouching))
			);
}