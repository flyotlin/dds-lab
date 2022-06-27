#include "SimpleStringPubSubTypes.h"

#include <cstdlib>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

using namespace eprosima::fastdds::dds;


class MyPublisher
{
private:
    const static int DOMAIN_ID = 0;
    SimpleString data_;     // object represents the SimpleString Data
    DomainParticipant *participant_;
    Publisher *publisher_;
    Topic *topic_;
    DataWriter *writer_;
    TypeSupport type_;
    DataWriterListener listener_;

    int node_id;

public:
    MyPublisher(int node_id)
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new SimpleStringPubSubType())
        , node_id(node_id)
    {
    }

    virtual ~MyPublisher()
    {
        if (writer_ != nullptr) {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr) {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    bool init()
    {
        data_.index(0);
        data_.data("Hello World!");     // the data inside SimpleString

        // create participant
        DomainParticipantQos participantQos;
        participantQos.name("participant_publisher");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(DOMAIN_ID, participantQos);
        if (participant_ == nullptr) {
            return false;
        }

        // register data type
        type_.register_type(participant_);

        // create Topic
        const std::string topic_name = "test_topic";
        const std::string type_name = "SimpleString";   // 要跟 .idl 檔名一樣

        topic_ = participant_->create_topic(topic_name, type_name, TOPIC_QOS_DEFAULT); // topic name, type name
        if (topic_ == nullptr) {
            return false;
        }

        // create Publisher
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        if (publisher_ == nullptr) {
            return false;
        }

        // create DataWriter
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
        if (writer_ == nullptr) {
            return false;
        }

        return true;
    }

    bool publish()
    {
        data_.index(data_.index() + 1);
        writer_->write(&data_);

        return true;
    }

    void run()
    {
        while (true) {
            if (publish()) {
                std::cout << "S " << node_id << " " << data_.index() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));   // sleep for 1000 ms
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Provide 2 arguments!\n";
        return 0;
    }

    int node_id = atoi(argv[1]);
    MyPublisher *pub = new MyPublisher(node_id);
    if (pub->init()) {
        pub->run();
    }

    delete pub;
    return 0;
}