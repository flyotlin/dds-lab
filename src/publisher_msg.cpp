#include "publisher_msg.h"

using namespace eprosima::fastdds::dds;


bool MyPublisher::changeDatawriterReliability(bool isDataWriterReliable)
{
    DataWriterQos qos = writer_->get_qos();

    ReliabilityQosPolicy policy;
    policy.kind = isDataWriterReliable ? RELIABLE_RELIABILITY_QOS : BEST_EFFORT_RELIABILITY_QOS;
    qos.reliability(policy);

    if (ReturnCode_t::RETCODE_OK != writer_->set_qos(qos)) {
        return false;
    }
    return true;
}

bool MyPublisher::changeTopicReliability(bool isTopicReliable)
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

bool MyPublisher::changeDatawriterResourceLimits(int datawriterMaxSample)
{
    DataWriterQos qos = writer_->get_qos();

    ResourceLimitsQosPolicy policy;
    policy.max_samples = datawriterMaxSample;
    qos.resource_limits(policy);

    if (ReturnCode_t::RETCODE_OK != writer_->set_qos(qos)) {
        return false;
    }
    return true;
}

bool MyPublisher::changeTopicResourceLimits(int topicMaxSample)
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

MyPublisher::MyPublisher(int nodeID, bool isReliable, int interval, int totalMsg, int maxSample)
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new SimpleStringPubSubType())
        , nodeID(nodeID)
        , isReliable(isReliable)
        , interval(interval)
        , totalMsg(totalMsg)
        , maxSample(maxSample)
    {
    }

MyPublisher::~MyPublisher()
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

bool MyPublisher::init()
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
    const std::string topic_name = "test_topic_" + nodeID;
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

    // Set Domain Entity Qos
    changeTopicReliability(isReliable);
    changeDatawriterReliability(isReliable);
    changeTopicResourceLimits(maxSample);
    changeDatawriterResourceLimits(maxSample);

    return true;
}

bool MyPublisher::publish()
{
    data_.index(data_.index() + 1);
    data_.data(pubData);
    writer_->write(&data_);

    return true;
}

void MyPublisher::run()
{
    for (int i = 0; i < totalMsg; i++) {
        if (publish() && i > 0) {
            std::cout << "S " << nodeID << " " << data_.index() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));   // sleep for certain amount of time (ms)
    }
}


int main(int argc, char *argv[])
{
    if (argc != 6) {
        std::cout << "Insufficient Arguments\n";
        std::cout << "Usage: ./MyPublisher NODE_ID IS_RELIABLE INTERVAL TOTAL_MSG MAX_SAMPLE" << std::endl;
        return 0;
    }

    int nodeID = atoi(argv[1]);
    bool isReliable = (!strcmp(argv[2], "true")) ? true : false;
    int interval = atoi(argv[3]);
    int totalMsg = atoi(argv[4]);
    int maxSample = atoi(argv[5]);

    MyPublisher *pub = new MyPublisher(nodeID, isReliable, interval, totalMsg, maxSample);
    if (pub->init()) {
        pub->run();
    }

    delete pub;
    return 0;
}
