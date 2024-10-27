#pragma once

#include <entt/core/type_info.hpp>
#include <entt/container/dense_map.hpp>

#include <solanaceae/message3/registry_message_model.hpp>
#include <solanaceae/object_store/object_store.hpp>

#include <nlohmann/json_fwd.hpp>

struct MessageSerializerNJ {
	using Registry = Message3Registry;
	using Handle = Message3Handle;

	static constexpr const char* version {"2"};

	Contact3Registry& cr;
	ObjectStore2& os;

	// nlohmann
	// json/msgpack
	using serialize_fn = bool(*)(MessageSerializerNJ& msc, const Handle h, nlohmann::json& out);
	entt::dense_map<entt::id_type, serialize_fn> _serl_json;

	using deserialize_fn = bool(*)(MessageSerializerNJ& msc, Handle h, const nlohmann::json& in);
	entt::dense_map<entt::id_type, deserialize_fn> _deserl_json;

	template<typename T>
	static bool component_get_json(MessageSerializerNJ&, const Handle h, nlohmann::json& j) {
		if (h.template all_of<T>()) {
			if constexpr (!std::is_empty_v<T>) {
				j = h.template get<T>();
			}
			return true;
		}

		return false;
	}

	template<typename T>
	static bool component_emplace_or_replace_json(MessageSerializerNJ&, Handle h, const nlohmann::json& j) {
		if constexpr (std::is_empty_v<T>) {
			h.template emplace_or_replace<T>(); // assert empty json?
		} else {
			h.template emplace_or_replace<T>(static_cast<T>(j));
		}
		return true;
	}

	void registerSerializer(serialize_fn fn, const entt::type_info& type_info) {
		_serl_json[type_info.hash()] = fn;
	}

	template<typename CompType>
	void registerSerializer(
		serialize_fn fn = component_get_json<CompType>,
		const entt::type_info& type_info = entt::type_id<CompType>()
	) {
		registerSerializer(fn, type_info);
	}

	void registerDeserializer(deserialize_fn fn, const entt::type_info& type_info) {
		_deserl_json[type_info.hash()] = fn;
	}

	template<typename CompType>
	void registerDeserializer(
		deserialize_fn fn = component_emplace_or_replace_json<CompType>,
		const entt::type_info& type_info = entt::type_id<CompType>()
	) {
		registerDeserializer(fn, type_info);
	}

	// TODO: deregister



	// helper
	nlohmann::json serlContactByID(Contact3 c) const;
	Contact3 deserlContactByID(const nlohmann::json& j);
	nlohmann::json serlFileObjByID(ObjectHandle o) const;
	ObjectHandle deserlFileObjByID(const nlohmann::json& j);
};

// fwd
namespace Message::Components {
struct ContactFrom;
struct ContactTo;
struct ReceivedBy;
struct MessageFileObject;
}

// make specializations known
template<>
bool MessageSerializerNJ::component_get_json<Message::Components::ContactFrom>(MessageSerializerNJ& msc, const Handle h, nlohmann::json& j);
template<>
bool MessageSerializerNJ::component_emplace_or_replace_json<Message::Components::ContactFrom>(MessageSerializerNJ& msc, Handle h, const nlohmann::json& j);
template<>
bool MessageSerializerNJ::component_get_json<Message::Components::ContactTo>(MessageSerializerNJ& msc, const Handle h, nlohmann::json& j);
template<>
bool MessageSerializerNJ::component_emplace_or_replace_json<Message::Components::ContactTo>(MessageSerializerNJ& msc, Handle h, const nlohmann::json& j);
template<>
bool MessageSerializerNJ::component_get_json<Message::Components::ReceivedBy>(MessageSerializerNJ& msc, const Handle h, nlohmann::json& j);
template<>
bool MessageSerializerNJ::component_emplace_or_replace_json<Message::Components::ReceivedBy>(MessageSerializerNJ& msc, Handle h, const nlohmann::json& j);
template<>
bool MessageSerializerNJ::component_get_json<Message::Components::MessageFileObject>(MessageSerializerNJ& msc, const Handle h, nlohmann::json& j);
template<>
bool MessageSerializerNJ::component_emplace_or_replace_json<Message::Components::MessageFileObject>(MessageSerializerNJ& msc, Handle h, const nlohmann::json& j);
