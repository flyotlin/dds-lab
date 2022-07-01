#ifndef _PUBLISHER_H_
#define _PUBLISHER_H_

#include "SimpleStringPubSubTypes.h"

#include <cstdlib>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>


class MyPublisher
{
public:
    MyPublisher(int, bool, int, int);
    ~MyPublisher(); // TODO: why virtual cannot be applied to function implementation?
    bool init();
    bool publish();
    void run();
private:
    const static int DOMAIN_ID = 0;
    const std::string pubData = "RErRxZp9fprLbyZD1GXtKujEMyIfxI15AyB5sxVRaN96kB4auk9a8NJ69gYlpMySpZ9jXWFuf1hOGaUVaHZr4zhoHh5wSOT6tUhu7UZpIaainlUBgL3N6xZSuczTKVL4Q7DgBW7mm2mOwKeJLQnosqGIPZb1V89z4toJUu7nTHKxx3TGQLjNOLnm7Hs3A6jlWkawQeEbnuoZUdDYsBHU4cZz7Hr7y0TqoZJWCjIYpyLCV3PaAqNYXLYloqQr9YM";  // 255
    int nodeID;
    bool isReliable;
    int interval;
    int maxSample;

    SimpleString data_;
    eprosima::fastdds::dds::DomainParticipant *participant_;
    eprosima::fastdds::dds::Publisher *publisher_;
    eprosima::fastdds::dds::Topic *topic_;
    eprosima::fastdds::dds::DataWriter *writer_;
    eprosima::fastdds::dds::TypeSupport type_;
    eprosima::fastdds::dds::DataWriterListener listener_;

    bool changeDatawriterReliability(bool);
    bool changeTopicReliability(bool);
    bool changeDatawriterResourceLimits(int);
    bool changeTopicResourceLimits(int);
};

#endif  // _PUBLISHER_H_
