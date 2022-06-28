#include "SimpleStringPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

using namespace eprosima::fastdds::dds;


class SubListener : public DataReaderListener
{
public:
    SimpleString data_;
    std::atomic_int samples_;

    int node_id;

    SubListener(int node_id)
        : node_id(node_id)
    {
        samples_ = 0;
    }

    ~SubListener() override
    {
    }

    void on_data_available(DataReader *reader) override
    {
        SampleInfo info;
        if (ReturnCode_t::RETCODE_OK == reader->take_next_sample(&data_, &info)) {
            if (info.valid_data) {
                samples_++;
                std::cout << "R " << node_id << " " << data_.index() << std::endl;
                // std::cout << info.sourc e_timestamp.to_ns() << ", " << info.reception_timestamp.to_ns() << std::endl;
            }
        }
    }
};


class MySubscriber
{
private:
    const static int DOMAIN_ID = 0;
    DomainParticipant *participant_;
    Subscriber *subscriber_;
    DataReader *reader_;
    Topic *topic_;
    TypeSupport type_;
    SubListener *listener_;

    int node_id;
    bool is_reliable;

    bool change_datareader_reliability(bool is_reliable)
    {
        DataReaderQos qos = reader_->get_qos();

        ReliabilityQosPolicy policy;
        policy.kind = is_reliable ? RELIABLE_RELIABILITY_QOS : BEST_EFFORT_RELIABILITY_QOS;
        qos.reliability(policy);

        if (ReturnCode_t::RETCODE_OK != reader_->set_qos(qos)) {
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
    MySubscriber(int node_id, bool is_reliable)
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new SimpleStringPubSubType)
        , node_id(node_id)
        , listener_(new SubListener(node_id))
        , is_reliable(is_reliable)
    {
    }

    virtual ~MySubscriber()
    {
        if (reader_ != nullptr) {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        if (subscriber_ != nullptr) {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    bool init()
    {
        DomainParticipantQos participantQos;
        participantQos.name("participant_subscriber");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(DOMAIN_ID, participantQos);
        if (participant_ == nullptr) {
            return false;
        }

        // register the data type
        type_.register_type(participant_);

        // create Topic
        const std::string topic_name = "test_topic_" + node_id;
        const std::string type_name = "SimpleString";
        topic_ = participant_->create_topic(topic_name, type_name, TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) {
            return false;
        }
        change_topic_reliability(is_reliable);

        // create Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
        if (subscriber_ == nullptr) {
            return false;
        }

        // create DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, listener_);
        if (reader_ == nullptr) {
            return false;
        }
        change_datareader_reliability(is_reliable);

        return true;
    }

    void run()
    {
        while (true) {
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};


int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Provide 3 arguments!\n";
        return 0;
    }

    int node_id = atoi(argv[1]);
    bool is_reliable = (!strcmp(argv[2], "true")) ? true : false;

    std::cout << "Sub Node: " << node_id << ", Reliability: " << argv[2] << " " << is_reliable << std::endl;

    MySubscriber *sub = new MySubscriber(node_id, is_reliable);
    if (sub->init()) {
        sub->run();
    }

    delete sub;
    return 0;
}