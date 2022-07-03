#include "subscriber_msg.h"

using namespace eprosima::fastdds::dds;


SubListener::SubListener(int nodeID)
    : nodeID(nodeID)
{
    samples_ = 0;
}

SubListener::~SubListener() {}

void SubListener::on_data_available(DataReader *reader)
{
    SampleInfo info;
    if (ReturnCode_t::RETCODE_OK == reader->take_next_sample(&data_, &info)) {
        if (info.valid_data) {
            samples_++;
            std::cout << "R " << nodeID << " " << data_.index() << std::endl;
            // std::cout << info.sourc e_timestamp.to_ns() << ", " << info.reception_timestamp.to_ns() << std::endl;
        }
    }
}

bool MySubscriber::changeDatareaderReliability(bool isDatareaderReliable)
{
    DataReaderQos qos = reader_->get_qos();

    ReliabilityQosPolicy policy;
    policy.kind = isDatareaderReliable ? RELIABLE_RELIABILITY_QOS : BEST_EFFORT_RELIABILITY_QOS;
    qos.reliability(policy);

    if (ReturnCode_t::RETCODE_OK != reader_->set_qos(qos)) {
        return false;
    }

    return true;
}

bool MySubscriber::changeTopicReliability(bool isTopicReliable)
    {
        TopicQos qos = topic_->get_qos();

        ReliabilityQosPolicy policy;
        policy.kind = isTopicReliable ? RELIABLE_RELIABILITY_QOS : BEST_EFFORT_RELIABILITY_QOS;
        qos.reliability(policy);

        if (ReturnCode_t::RETCODE_OK != topic_->set_qos(qos)) {
            return false;
        }

        return true;
    }

bool MySubscriber::changeDatareaderResourceLimits(int datareaderMaxSample)
{
    DataReaderQos qos = reader_->get_qos();
    ResourceLimitsQosPolicy policy;
    policy.max_samples = datareaderMaxSample;
    qos.resource_limits(policy);

    if (ReturnCode_t::RETCODE_OK != reader_->set_qos(qos)) {
        return false;
    }
    return true;
}

bool MySubscriber::changeTopicResourceLimits(int topicMaxSample)
{
    TopicQos qos = topic_->get_qos();
    ResourceLimitsQosPolicy policy;
    policy.max_samples = topicMaxSample;
    qos.resource_limits(policy);

    if (ReturnCode_t::RETCODE_OK != topic_->set_qos(qos)) {
        return false;
    }
    return true;
}

MySubscriber::MySubscriber(int nodeID, bool isReliable, int totalMsg, int maxSample)
    : participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr)
    , type_(new SimpleStringPubSubType)
    , nodeID(nodeID)
    , listener_(new SubListener(nodeID))
    , isReliable(isReliable)
    , totalMsg(totalMsg)
    , maxSample(maxSample)
{
}

MySubscriber::~MySubscriber()
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

bool MySubscriber::init()
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
    const std::string topic_name = "test_topic_" + nodeID;
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

    changeTopicReliability(isReliable);
    changeDatareaderReliability(isReliable);

    return true;
}

void MySubscriber::run()
{
    while (true) {
        // if (listener_->samples_ >= totalMsg) {
        //     break;
        // }
    }
}


int main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cout << "Insufficient Arguments\n";
        std::cout << "Usage: ./MySubscriber NODE_ID IS_RELIABLE TOTAL_MSG MAX_SAMPLE" << std::endl;
        return 0;
    }

    int nodeID = atoi(argv[1]);
    bool isReliable = (!strcmp(argv[2], "true")) ? true : false;
    int totalMsg = atoi(argv[3]);
    int maxSample = atoi(argv[4]);


    MySubscriber *sub = new MySubscriber(nodeID, isReliable, totalMsg, maxSample);
    if (sub->init()) {
        sub->run();
    }

    delete sub;
    return 0;
}