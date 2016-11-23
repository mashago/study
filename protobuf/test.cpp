#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <memory>

#include "masha.pb.h"

using namespace std;

void print_person(const MashaProto::Person &person)
{
	printf("person.ByteSize()=%d\n", person.ByteSize());
	printf("person.id()=%d\n", person.id());
	printf("person.name()=%s\n", person.name().c_str());
	if (person.has_age()) {
		printf("person.age=%d\n", person.age());
	} else {
		printf("person has no age\n");
	}
	int numlist_size = person.numlist_size();
	printf("person.numlist_size()=%d\n", numlist_size);
	for (int i=0; i<numlist_size; i++) {
		printf("person.numlist[%d]=%d\n", i, person.numlist(i));
	}
	int strlist_size = person.strlist_size();
	printf("person.strlist_size()=%d\n", strlist_size);
	for (int i=0; i<strlist_size; i++) {
		printf("person.strlist[%d]=%s\n", i, person.strlist(i).c_str());
	}
	printf("person.color()=%d\n", person.color());
	printf("\n");
	
}

int my_serialize(char *buffer, google::protobuf::Message &data)
{
	data.SerializeToArray(buffer, data.ByteSize());
	return data.ByteSize();
}

google::protobuf::Message * createMessage(const char *typeName)
{
	// create msg by type name
	// 1. create descriptor by typeName
	// 2. create msg by descriptor
	// 3. new msg
	
	// 1.
	const google::protobuf::Descriptor *descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (!descriptor)
	{
		return NULL;
	}
	
	// 2.
	const google::protobuf::Message *prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	if (!prototype)
	{
		return NULL;
	}

	// 3.
	return prototype->New();
}

typedef std::shared_ptr<google::protobuf::Message> GoogleMsgPtr;
GoogleMsgPtr createMessageShared(const char *typeName)
{
	// 1.
	const google::protobuf::Descriptor *descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (!descriptor)
	{
		return GoogleMsgPtr();
	}
	
	// 2.
	const google::protobuf::Message *prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
	if (!prototype)
	{
		return GoogleMsgPtr();
	}

	// 3.
	return GoogleMsgPtr(prototype->New());
}


int test_person()
{
	MashaProto::Person p1;
	p1.set_id(20);
	p1.set_name("Masha");
	p1.set_age(26);
	p1.add_numlist(1); // repeated int32 numList
	p1.add_numlist(3);
	std::string *s1 = p1.add_strlist(); // repeated string strList
	*s1 = "mmm";
	std::string *s2 = p1.add_strlist(); 
	*s2 = "aaa";
	p1.set_color(MashaProto::Color::BLACK);

	print_person(p1);

	MashaProto::Person p2;
	p2.set_id(21);
	p2.set_age(28);
	p2.clear_age(); // clear
	p2.set_name("Kelton");
	p2.add_numlist(2); // repeated int32 numList
	p2.add_numlist(4);
	p2.clear_numlist();
	std::string *s3 = p2.add_strlist(); // repeated string strList
	*s3 = "kkk";
	std::string *s4 = p2.add_strlist(); 
	*s4 = "eee";
	p2.set_color(MashaProto::Color::GARY);

	print_person(p2);

	////////////////////////////////

	{
	printf("SerializeToArray\n");
	const int size = 100;
	char buffer[size];
	bzero(buffer, size);

	p2.SerializeToArray(buffer, p2.ByteSize());

	// printf("serialize buffer=[%s]\n", buffer);
	
	MashaProto::Person p3;
	p3.ParseFromArray(buffer, p2.ByteSize());
	print_person(p3);
	}

	////////////////////////////////

	{
	printf("SerializeToString\n");
	std::string string_buffer;
	p2.SerializeToString(&string_buffer);
	
	MashaProto::Person p4;
	p4.ParseFromString(string_buffer);
	print_person(p4);
	}

	////////////////////////////////

	{
	printf("my_serialize\n");
	MashaProto::Person p;
	p.set_id(100);
	p.set_name("MashaX");
	p.set_age(30);
	p.add_numlist(1); // repeated int32 numList
	p.add_numlist(3);
	p.add_numlist(5);
	std::string *s1 = p.add_strlist(); // repeated string strList
	*s1 = "hello";
	std::string *s2 = p.add_strlist(); 
	*s2 = "world";
	p.set_color(MashaProto::Color::RED);

	const int size = 100;
	char buffer[size];
	bzero(buffer, size);

	int data_size = my_serialize(buffer, p);

	printf("my_serialize:data_size = %d \n", data_size);
	
	MashaProto::Person p3;
	p3.ParseFromArray(buffer, data_size);
	print_person(p3);

	}

	////////////////////////////////

	{
	printf("Parse by type name\n");
	const int size = 100;
	char buffer[size];
	bzero(buffer, size);

	p2.SerializeToArray(buffer, p2.ByteSize());

	// printf("serialize buffer=[%s]\n", buffer);
	
	MashaProto::Person p3;
	p3.ParseFromArray(buffer, p2.ByteSize());
	// print_person(p3);
	printf("typeName=%s\n", p3.GetTypeName().c_str());

	google::protobuf::Message *pMsg = createMessage(p3.GetTypeName().c_str());
	if (pMsg == NULL)
	{
		printf("createMessage fail\n");
		return -1;
	}
	pMsg->ParseFromArray(buffer, p2.ByteSize());

	MashaProto::Person *pp1 = dynamic_cast<MashaProto::Person *>(pMsg);
	print_person(*pp1);
	delete pMsg;

	printf("\n");

	GoogleMsgPtr pMsg2 = createMessageShared(p3.GetTypeName().c_str());
	if (pMsg2 == NULL)
	{
		printf("createMessageShared fail\n");
		return -1;
	}
	pMsg2->ParseFromArray(buffer, p2.ByteSize());

	MashaProto::Person *pp2 = dynamic_cast<MashaProto::Person *>(pMsg2.get());
	print_person(*pp2);

	}

	
	return 0;
}

int main(int argc, char **argv)
{
	printf("hello protobuf test\n");
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	test_person();

	// must call in the end, avoid memory leek
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}
