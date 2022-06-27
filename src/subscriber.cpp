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

public:
    MySubscriber(int node_id)
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new SimpleStringPubSubType)
        , node_id(node_id)
        , listener_(new SubListener(node_id))
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
        const std::string topic_name = "test_topic";
        const std::string type_name = "SimpleString";
        topic_ = participant_->create_topic(topic_name, type_name, TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) {
            return false;
        }

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
    if (argc != 2) {
        std::cout << "Provide 2 arguments!\n";
        return 0;
    }

    int node_id = atoi(argv[1]);
    MySubscriber *sub = new MySubscriber(node_id);
    if (sub->init()) {
        sub->run();
    }

    delete sub;
    return 0;
}