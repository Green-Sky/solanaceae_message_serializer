#include "./message_serializer.hpp"

#include <solanaceae/message3/components.hpp>
#include <solanaceae/contact/components.hpp>

#include <nlohmann/json.hpp>

#include <iostream>

static Contact3 findContactByID(Contact3Registry& cr, const std::vector<uint8_t>& id) {
	// TODO: id lookup table, this is very inefficent
	for (const auto& [c_it, id_it] : cr.view<Contact::Components::ID>().each()) {
		if (id == id_it.data) {
			return c_it;
		}
	}

	return entt::null;
}

nlohmann::json MessageSerializerNJ::serlContactByID(Contact3 c) const {
	if (!cr.valid(c)) {
		// while this is invalid registry state, it is valid serialization
		std::cerr << "MSC warning: encountered invalid contact\n";
		return nullptr;
	}

	if (!cr.all_of<Contact::Components::ID>(c)) {
		// unlucky, this contact is purely ephemeral
		std::cerr << "MSC warning: encountered contact without ID\n";
		return nullptr;
	}

	return nlohmann::json::binary(cr.get<Contact::Components::ID>(c).data);
}

Contact3 MessageSerializerNJ::deserlContactByID(const nlohmann::json& j) {
	std::vector<uint8_t> id;
	if (j.is_binary()) {
		id = j.get_binary();
	} else {
		j["bytes"].get_to(id);
	}

	Contact3 other_c = findContactByID(cr, id);
	if (!cr.valid(other_c)) {
		// create sparse contact with id only
		other_c = cr.create();
		cr.emplace_or_replace<Contact::Components::ID>(other_c, id);
	}

	return other_c;
}

template<>
bool MessageSerializerNJ::component_get_json<Message::Components::ContactFrom>(MessageSerializerNJ& msc, const Handle h, nlohmann::json& j) {
	const Contact3 c = h.get<Message::Components::ContactFrom>().c;
	j = msc.serlContactByID(c);

	return true;
}

template<>
bool MessageSerializerNJ::component_emplace_or_replace_json<Message::Components::ContactFrom>(MessageSerializerNJ& msc, Handle h, const nlohmann::json& j) {
	if (j.is_null()) {
		std::cerr << "MSC warning: encountered null contact\n";
		h.emplace_or_replace<Message::Components::ContactFrom>();
		return true;
	}

	h.emplace_or_replace<Message::Components::ContactFrom>(msc.deserlContactByID(j));

	// TODO: should we return false if the contact is unknown??
	return true;
}

template<>
bool MessageSerializerNJ::component_get_json<Message::Components::ContactTo>(MessageSerializerNJ& msc, const Handle h, nlohmann::json& j) {
	const Contact3 c = h.get<Message::Components::ContactTo>().c;
	j = msc.serlContactByID(c);

	return true;
}

template<>
bool MessageSerializerNJ::component_emplace_or_replace_json<Message::Components::ContactTo>(MessageSerializerNJ& msc, Handle h, const nlohmann::json& j) {
	if (j.is_null()) {
		std::cerr << "MSC warning: encountered null contact\n";
		h.emplace_or_replace<Message::Components::ContactTo>();
		return true;
	}

	h.emplace_or_replace<Message::Components::ContactTo>(msc.deserlContactByID(j));

	// TODO: should we return false if the contact is unknown??
	return true;
}

template<>
bool MessageSerializerNJ::component_get_json<Message::Components::ReceivedBy>(MessageSerializerNJ& msc, const Handle h, nlohmann::json& j) {
	const auto& comp = h.get<Message::Components::ReceivedBy>();
	if (comp.ts.empty()) {
		// empty array
		j = nlohmann::json::array();
		return true;
	}

	for (const auto& [c, v] : comp.ts) {
		if (!msc.cr.valid(c) || !msc.cr.all_of<Contact::Components::ID>(c)) {
			// while this is invalid registry state, it is valid serialization
			// we just skip this contact entirely and drop the time
			std::cerr << "MSC warning: encountered invalid contact / contact without ID\n";
			continue;
		}
		auto& new_entry = j.emplace_back(nlohmann::json::array());
		new_entry.emplace_back(msc.serlContactByID(c));
		new_entry.emplace_back(v);
	}

	return true;
}

template<>
bool MessageSerializerNJ::component_emplace_or_replace_json<Message::Components::ReceivedBy>(MessageSerializerNJ& msc, Handle h, const nlohmann::json& j) {
	if (j.is_null()) {
		std::cerr << "MSC warning: encountered null ReceivedBy\n";
		h.emplace_or_replace<Message::Components::ReceivedBy>();
		return true;
	}

	if (!j.is_array()) {
		throw (nlohmann::detail::type_error::create(302, nlohmann::detail::concat("type must be array, but is ", j.type_name()), &j));
	}

	auto& comp = h.emplace_or_replace<Message::Components::ReceivedBy>();

	if (j.empty()) {
		// empty comp
		return true;
	}

	for (const auto& p : j) {
		if (!p.is_array()) {
			throw (nlohmann::detail::type_error::create(302, nlohmann::detail::concat("type must be array, but is ", p.type_name()), &j));
		}

		comp.ts.emplace(
			msc.deserlContactByID(p.at(0)), // TODO: error checking?
			p.at(1).get<decltype(comp.ts)::mapped_type>()
		);
	}

	// TODO: should we return false if the contact is unknown??
	return true;
}

