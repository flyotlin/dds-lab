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
    bool is_reliable;
    int sleep_time;

    bool change_datawriter_reliability(bool is_reliable)
    {
        DataWriterQos qos = writer_->get_qos();

        ReliabilityQosPolicy policy;
        policy.kind = is_reliable ? RELIABLE_RELIABILITY_QOS : BEST_EFFORT_RELIABILITY_QOS;
        qos.reliability(policy);

        if (ReturnCode_t::RETCODE_OK != writer_->set_qos(qos)) {
            return false;
        }
        return true;
    }

    bool change_topic_reliability(bool is_reliable)
    {
        TopicQos qos = topic_->get_qos();

        ReliabilityQosPolicy policy;
        policy.kind = is_reliable ? RELIABLE_RELIABILITY_QOS : BEST_EFFORT_RELIABILITY_QOS;
        qos.reliability(policy);

        if (ReturnCode_t::RETCODE_OK != topic_->set_qos(qos)) {
            return false;
        }
        return true;
    }

public:
    MyPublisher(int node_id, bool is_reliable, int sleep_time)
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new SimpleStringPubSubType())
        , node_id(node_id)
        , is_reliable(is_reliable)
        , sleep_time(sleep_time)
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
        const std::string topic_name = "test_topic_" + node_id;
        const std::string type_name = "SimpleString";   // 要跟 .idl 檔名一樣

        topic_ = participant_->create_topic(topic_name, type_name, TOPIC_QOS_DEFAULT); // topic name, type name
        if (topic_ == nullptr) {
            return false;
        }
        change_topic_reliability(is_reliable);

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
        change_datawriter_reliability(is_reliable);

        return true;
    }

    bool publish()
    {
        data_.index(data_.index() + 1);
        data_.data("RErRxZp9fprLbyZD1GXtKujEMyIfxI15AyB5sxVRaN96kB4auk9a8NJ69gYlpMySpZ9jXWFuf1hOGaUVaHZr4zhoHh5wSOT6tUhu7UZpIaainlUBgL3N6xZSuczTKVL4Q7DgBW7mm2mOwKeJLQnosqGIPZb1V89z4toJUu7nTHKxx3TGQLjNOLnm7Hs3A6jlWkawQeEbnuoZUdDYsBHU4cZz7Hr7y0TqoZJWCjIYpyLCV3PaAqNYXLYloqQr9YM");  // 255
        writer_->write(&data_);

        return true;
    }

    void run()
    {
        while (true) {
            if (publish()) {
                std::cout << "S " << node_id << " " << data_.index() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));   // sleep for certain amount of time (ms)
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cout << "Provide 4 arguments!\n";
        return 0;
    }

    int node_id = atoi(argv[1]);
    bool is_reliable = (!strcmp(argv[2], "true")) ? true : false;
    int sleep_time = atoi(argv[3]);

    // std::cout << "Pub Node: " << node_id << ", Reliability: " << argv[2] << " " << is_reliable << std::endl;

    MyPublisher *pub = new MyPublisher(node_id, is_reliable, sleep_time);
    if (pub->init()) {
        pub->run();
    }

    delete pub;
    return 0;
}