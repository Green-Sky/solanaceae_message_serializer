#include "./message_components_serializer.hpp"

#include "./message_components.hpp"

void registerMessageComponents(MessageSerializerNJ& msnj) {
	msnj.registerSerializer<Message::Components::Timestamp>();
	msnj.registerDeserializer<Message::Components::Timestamp>();
	msnj.registerSerializer<Message::Components::TimestampProcessed>();
	msnj.registerDeserializer<Message::Components::TimestampProcessed>();
	msnj.registerSerializer<Message::Components::TimestampWritten>();
	msnj.registerDeserializer<Message::Components::TimestampWritten>();
	msnj.registerSerializer<Message::Components::ContactFrom>();
	msnj.registerDeserializer<Message::Components::ContactFrom>();
	msnj.registerSerializer<Message::Components::ContactTo>();
	msnj.registerDeserializer<Message::Components::ContactTo>();

	msnj.registerSerializer<Message::Components::TagUnread>();
	msnj.registerDeserializer<Message::Components::TagUnread>();
	msnj.registerSerializer<Message::Components::Read>();
	msnj.registerDeserializer<Message::Components::Read>();

	msnj.registerSerializer<Message::Components::ReceivedBy>();
	msnj.registerDeserializer<Message::Components::ReceivedBy>();

	msnj.registerSerializer<Message::Components::MessageText>();
	msnj.registerDeserializer<Message::Components::MessageText>();
	msnj.registerSerializer<Message::Components::TagMessageIsAction>();
	msnj.registerDeserializer<Message::Components::TagMessageIsAction>();

	msnj.registerSerializer<Message::Components::MessageFileObject>();
	msnj.registerDeserializer<Message::Components::MessageFileObject>();
}

