#pragma once

#include <solanaceae/util/utils.hpp>

#include <solanaceae/message3/components.hpp>

#include <nlohmann/json.hpp>

namespace Message::Components {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Timestamp, ts)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TimestampProcessed, ts)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TimestampWritten, ts)
	//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ContactFrom, c) // ms special
	//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ContactTo, c) // ms special
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Read, ts)


	//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ReceivedBy, ts) // ms special
	// ReadBy
	// SyncedBy

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MessageText, text)

	// TODO: remove
	//namespace Transfer {
		//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FileInfo::FileDirEntry, file_name, file_size)
		//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FileInfo, file_list, total_size)
		//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FileInfoLocal, file_list)
	//} // Transfer

} // Message::Components


namespace entt {
	// is this really not needed?
	//template<typename Key, typename Value>
	//inline void to_json(nlohmann::json& j, const dense_map<Key, Value>& m) {
	//}

	// TODO: using the internal stuff might be bad for abi
	template<typename Key, typename Value>
	inline void from_json(const nlohmann::json& j, dense_map<Key, Value>& m) {
		if (!j.is_array()) {
			throw (nlohmann::detail::type_error::create(302, nlohmann::detail::concat("type must be array, but is ", j.type_name()), &j));
		}

		m.clear();

		for (const auto& p : j) {
			if (!p.is_array()) {
				throw (nlohmann::detail::type_error::create(302, nlohmann::detail::concat("type must be array, but is ", p.type_name()), &j));
			}
			m.emplace(p.at(0).template get<Key>(), p.at(1).template get<Value>());
		}
	}
} // entt

