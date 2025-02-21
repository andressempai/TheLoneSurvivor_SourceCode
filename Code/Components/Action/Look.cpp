#include "Components/Action/Look.h"
#include "EventSystem.h"
#include "DevelopmentTools.h"

#define DB_PERLIN_IMPL
#include "db_perlin.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_look_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(Look));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_look_component);

void ReflectType(Schematyc::CTypeDesc<Look>& desc)
{
	desc.SetGUID("{C125C1AD-6C2E-40A8-BC73-ECB444DFDF2D}"_cry_guid);
	desc.SetEditorCategory("Game Actions");
	desc.SetName(Schematyc::CTypeName("lookactioncomponent"));
	desc.SetLabel("Look");
	desc.SetDescription("Allow Entity to Look over World");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
			&Look::camera_info_
		,	'cinf'
		,	"camerainfo"
		,	"Camera Information"
		,	"Information of the Camera"
		,	SCameraInfo{}
	);
}

void Look::Initialize()
{
	DevelopmentTools::add_action_listener(this, "player");

	BoomerShooterEventSystem::instance().subscribe<RecoilEvent>(GetEntityId(), [this](const EventBase& event)
	{
		is_shaking_ = true;
		shake_params_ = static_cast<const RecoilEvent&>(event).shake_params_;
		TimerManager::instance().make_timer(shake_timer_id_, shake_params_.shake_duration_, false, [this]()
		{
			perlin_noise_coordinate_ = 0.0f;
			position_shake_noise_ = 0.0f;
			rotation_shake_noise_ = 0.0f;
			is_shaking_ = false;
		});
	});

	BoomerShooterEventSystem::instance().subscribe<DeadEvent>(GetEntityId(), [this](const EventBase& event)
	{
		is_dead_ = true;
	});
}

void Look::OnShutDown()
{
	BoomerShooterEventSystem::instance().unsubscribe<RecoilEvent>(GetEntityId());
	BoomerShooterEventSystem::instance().unsubscribe<DeadEvent>(GetEntityId());
}

Cry::Entity::EventFlags Look::GetEventMask() const
{
	return Cry::Entity::EEvent::Update;
}

void Look::ProcessEvent(const SEntityEvent& event)
{
	using namespace DevelopmentTools;

	if (gEnv->IsEditing() or m_pEntity->IsGarbage())
		return;
	
	const auto delta_time = event.fParam[0];
	if (is_shaking_) {
		perlin_noise_coordinate_ += delta_time;
		position_shake_noise_ = db::perlin(
			perlin_noise_coordinate_ * shake_params_.pos_shake_frequency_
		) * shake_params_.pos_shake_amplitude_;

		rotation_shake_noise_ = db::perlin(
			perlin_noise_coordinate_ * shake_params_.rot_shake_frequency_
		) * shake_params_.rot_shake_amplitude_;
	}

	auto current_transform = GetTransformMatrix();
	auto camera_euler_rotation = CCamera::CreateAnglesYPR(make_matrix33(current_transform));
	if (not mouse_location_delta_.IsEquivalent(ZERO)) {
		camera_euler_rotation.x += mouse_location_delta_.x * rotation_speed * camera_info_.mouse_sensitivity_;
		camera_euler_rotation.y = crymath::clamp(
				camera_euler_rotation.y + mouse_location_delta_.y * rotation_speed * camera_info_.mouse_sensitivity_
			,	camera_info_.min_pitch_rotation_.ToRadians()
			,	camera_info_.max_pitch_rotation_.ToRadians()
		);
	}
	
	camera_euler_rotation.z = rotation_shake_noise_;

	current_transform.SetRotation33(CCamera::CreateOrientationYPR(camera_euler_rotation));

	const auto player_dimensions = get_physics_params<pe_player_dimensions>(*m_pEntity);
	camera_location_ = [&]()
	{
		if (not is_dead_) {
			const auto shake_vector = make_vector3(0.0f, position_shake_noise_, 0.0f);
			return make_vector3(0.0f, 0.0f, player_dimensions.heightEye) + current_transform.TransformVector(shake_vector);
		}

		const auto dead_height_eye = player_dimensions.heightCollider * 0.15f;
		return LERP(camera_location_, make_vector3(0.0f, 0.0f, dead_height_eye), 10.0f * delta_time);
	}();

	current_transform.SetTranslation(camera_location_);
	SetTransformMatrix(current_transform);

	const auto& system_camera = gEnv->pSystem->GetViewCamera();
	main_.SetFrustum(
			system_camera.GetViewSurfaceX()
		,	system_camera.GetViewSurfaceZ()
		,	camera_info_.fov_.ToRadians()
		,	camera_info_.near_plane_
		,	camera_info_.far_plane_
		,	system_camera.GetPixelAspectRatio()
	);
	
	main_.SetMatrix(GetWorldTransformMatrix());
	gEnv->pSystem->SetViewCamera(main_);

	if (const auto child = DevelopmentTools::get_child(*m_pEntity)) {
		const auto r = CCamera::CreateOrientationYPR(make_ang3(camera_euler_rotation.x, camera_euler_rotation.y));
		const auto t = Matrix34::CreateTranslationMat(make_vector3(0.0f, 0.0f, player_dimensions.heightEye));
		child->SetWorldTM(m_pEntity->GetWorldTM() * (t * r));
	}

	mouse_location_delta_ = ZERO;
}

void Look::OnAction(const ActionId& action, int activation_mode, float value)
{
	if ((strcmp(action.c_str(), "mouse_rotateyaw") == 0))
		mouse_location_delta_.x -= value;

	if ((strcmp(action.c_str(), "mouse_rotatepitch") == 0))
		mouse_location_delta_.y -= value;
}