#ifndef _SUBSCRIBER_H_
#define _SUBSCRIBER_H_

#include "SimpleStringPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>


class SubListener : public eprosima::fastdds::dds::DataReaderListener
{
public:
    SubListener(int);
    virtual ~SubListener() override;
    virtual void on_data_available(eprosima::fastdds::dds::DataReader *) override;
private:
    int nodeID;

    SimpleString data_;
    std::atomic_int samples_;
};

class MySubscriber
{
public:
    MySubscriber(int, bool, int);
    ~MySubscriber();
    bool init();
    void run();
private:
    const static int DOMAIN_ID = 0;
    int nodeID;
    bool isReliable;
    int maxSample;

    eprosima::fastdds::dds::DomainParticipant *participant_;
    eprosima::fastdds::dds::Subscriber *subscriber_;
    eprosima::fastdds::dds::DataReader *reader_;
    eprosima::fastdds::dds::Topic *topic_;
    eprosima::fastdds::dds::TypeSupport type_;
    SubListener *listener_;

    bool changeDatareaderReliability(bool);
    bool changeTopicReliability(bool);
    bool changeDatareaderResourceLimits(int);
    bool changeTopicResourceLimits(int);
};

#endif
