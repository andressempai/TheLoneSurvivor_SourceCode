#include "ItemCollector.h"
#include "../../EventSystem.h"
#include "../../DevelopmentTools.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

void register_item_collector_component(Schematyc::IEnvRegistrar& registrar)
{
	registrar.Scope(IEntity::GetEntityScopeGUID()).Register(SCHEMATYC_MAKE_ENV_COMPONENT(ItemCollector));
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&register_item_collector_component);

void ReflectType(Schematyc::CTypeDesc<ItemCollector>& desc)
{
	desc.SetGUID("{0368A991-D7F0-4D46-8360-17EE33834176}"_cry_guid);
	desc.SetEditorCategory("Game Components");
	desc.SetName(Schematyc::CTypeName("itemcollectorcomponent"));
	desc.SetLabel("Item Collector");
	desc.SetDescription("Allow Entity to collect entities inside world");
	desc.SetComponentFlags({ EEntityComponentFlags::Singleton, EEntityComponentFlags::Transform });
	desc.AddMember(
			&ItemCollector::size_
		,	'size'
		,	"triggersize"
		,	"Trigger Size"
		,	"Size for the trigger Entity"
		,	DevelopmentTools::make_unit_vector3()
	);
}

void ItemCollector::Initialize()
{
	SetTransformMatrix(Matrix34::CreateTranslationMat(DevelopmentTools::make_vector3(0.0f, 0.0f, size_.z * 0.5f)));

	trigger_ = m_pEntity->GetOrCreateComponent<IEntityTriggerComponent>();
	trigger_->SetTriggerBounds(DevelopmentTools::create_transformed_aabb(GetTransformMatrix(), size_));
}

Cry::Entity::EventFlags ItemCollector::GetEventMask() const
{
	return Cry::Entity::EEvent::EditorPropertyChanged | Cry::Entity::EEvent::EntityEnteredThisArea;
}

void ItemCollector::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event) {
	case Cry::Entity::EEvent::EditorPropertyChanged:
	{
		trigger_->SetTriggerBounds(DevelopmentTools::create_transformed_aabb(GetTransformMatrix(), size_));
	}
	break;

	case Cry::Entity::EEvent::EntityEnteredThisArea:
	{
		if (gEnv->IsEditing() or m_pEntity->IsGarbage())
			break;

		const auto item = DevelopmentTools::search_entity_by_id(static_cast<EntityId>(event.nParam[0]));
		if (const auto child = DevelopmentTools::get_child(*m_pEntity))
			BoomerShooterEventSystem::instance().send(item->GetId(), CollectEvent{ *child });
	}
	break;
	}
}

#ifndef RELEASE
IEntityComponentPreviewer* ItemCollector::GetPreviewer()
{
	return this;
}

void ItemCollector::Render(const IEntity& entity, const IEntityComponent& component, SEntityPreviewContext& context) const
{
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawAABB(
			DevelopmentTools::create_transformed_aabb(component.GetTransformMatrix(), size_)
		,	entity.GetWorldTM()
		,	false
		,	Col_Red
		,	eBBD_Faceted
	);
}
#endif // !RELEASE