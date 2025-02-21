#ifndef COMPARE_KEY_WITH_VALUE_H
#define COMPARE_KEY_WITH_VALUE_H

#include <CryAISystem/BehaviorTree/Decorator.h>
#include "../DevelopmentTools.h"

namespace BehaviorTree {
	enum class ComparisonType {
			IsEqual
		,	IsNotEqual
		,	IsGreater
		,	IsLess
		,	IsGreaterOrEqual
		,	IsLessOrEqual
		,	Count
	};

	SERIALIZATION_ENUM_BEGIN(ComparisonType, "ComparisonType")
	SERIALIZATION_ENUM(ComparisonType::IsEqual, "isequal", "Is Equal")
	SERIALIZATION_ENUM(ComparisonType::IsNotEqual, "isnotequal", "Is not Equal")
	SERIALIZATION_ENUM(ComparisonType::IsGreater, "isgreater", "Is Greater")
	SERIALIZATION_ENUM(ComparisonType::IsLess, "isless", "Is Less")
	SERIALIZATION_ENUM(ComparisonType::IsGreaterOrEqual, "isgreaterorequal", "Is Greater or Equal")
	SERIALIZATION_ENUM(ComparisonType::IsLessOrEqual, "islessorequal", "Is Less or Equal")
	SERIALIZATION_ENUM_END()

	template <typename T, typename TData>
	class CompareKeyWithValue final : public Decorator {
	public:
		struct RuntimeData {};
	
		CompareKeyWithValue()
		{
			if constexpr (not std::is_same_v<T, bool>) {
				using namespace DevelopmentTools;

				static constexpr auto max_evaluation_types = get_underlying_value_from_enum(ComparisonType::Count);
	
				type_xml_.Reserve(max_evaluation_types);
				type_xml_.Add("IsEqual", ComparisonType::IsEqual);
				type_xml_.Add("IsNotEqual", ComparisonType::IsNotEqual);
				type_xml_.Add("IsGreater", ComparisonType::IsGreater);
				type_xml_.Add("IsLess", ComparisonType::IsLess);
				type_xml_.Add("IsGreaterOrEqual", ComparisonType::IsGreaterOrEqual);
				type_xml_.Add("IsLessOrEqual", ComparisonType::IsLessOrEqual);
	
				type_serialization_.reserve(max_evaluation_types);
				type_serialization_.push_back("IsEqual");
				type_serialization_.push_back("IsNotEqual");
				type_serialization_.push_back("IsGreater");
				type_serialization_.push_back("IsLess");
				type_serialization_.push_back("IsGreaterOrEqual");
				type_serialization_.push_back("IsLessOrEqual");
			}
		}

	private:
		LoadResult LoadFromXml(const XmlNodeRef& xml, const LoadContext& context, const bool isLoadingFromEditor) override
		{
			key_ = xml->getAttr("key");
			xml->getAttr("value", value_);
			
			if constexpr (not std::is_same_v<T, bool>)
				type_xml_.Get(xml, "type", type_);
			
			return Decorator::LoadFromXml(xml, context, isLoadingFromEditor);
		}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		XmlNodeRef CreateXmlDescription() override
		{
			auto xml = Decorator::CreateXmlDescription();

			xml->setTag(TData::node_name_.data());
			xml->setAttr("key", key_);
			xml->setAttr("value", value_);

			if constexpr (not std::is_same_v<T, bool>)
				xml->setAttr("type", Serialization::getEnumLabel<ComparisonType>(type_));

			return xml;
		}
#endif	// USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
		void Serialize(Serialization::IArchive& archive) override
		{
			archive(key_, "key", "^Key To Evaluate");
			archive.doc("Key to Evaluate");

			archive(value_, "value", "^Comparison Value");
			archive.doc("Value to Compare");

			if constexpr (not std::is_same_v<T, bool>) {
				SerializationUtils::SerializeEnumList<ComparisonType>(
						archive
					,	"type"
					,	"^Comparison Type"
					,	type_serialization_
					,	type_xml_
					,	type_
				);
				
				archive.doc("Type of Evaluation");
			}

			Decorator::Serialize(archive);
		}
#endif	// USING_BEHAVIOR_TREE_SERIALIZATION

		Status Update(const BehaviorTree::UpdateContext& context) override
		{
			using namespace DevelopmentTools;
			const auto blackboard_value = get_value_from_blackboard<T>(key_.c_str(), context.blackboard);

			if constexpr (not std::is_same_v<T, bool>) {
				switch (type_) {
				case ComparisonType::IsEqual:
					return blackboard_value == value_ ? Decorator::Update(context) : Status::Failure;
				case ComparisonType::IsNotEqual:
					return blackboard_value != value_ ? Decorator::Update(context) : Status::Failure;
				case ComparisonType::IsGreater:
					return blackboard_value > value_ ? Decorator::Update(context) : Status::Failure;
				case ComparisonType::IsLess:
					return blackboard_value < value_ ? Decorator::Update(context) : Status::Failure;
				case ComparisonType::IsGreaterOrEqual:
					return blackboard_value >= value_ ? Decorator::Update(context) : Status::Failure;
				case ComparisonType::IsLessOrEqual:
					return blackboard_value <= value_ ? Decorator::Update(context) : Status::Failure;
				}
			}
			else {
				return blackboard_value == value_ ? Decorator::Update(context) : Status::Failure;
			}

			return Running;
		}
	
		CXMLAttrReader<ComparisonType> type_xml_;
		Serialization::StringList type_serialization_{};

		string key_{};
		T value_{};
		std::conditional_t<not std::is_same_v<T, bool>, ComparisonType, void*> type_{};
	};
}	// namespace BehaviorTree

#endif // !COMPARE_KEY_WITH_VALUE_H