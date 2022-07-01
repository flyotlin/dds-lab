import os
import subprocess
import sys
import threading


IS_RELIABLE = "true"
NODE_NUM = 1
SLEEP_TIME = 1000


def publisher_job(path: str, node_id: int):
    publisher_path = os.path.join(path, "MyPublisher")
    os.system(f"{publisher_path} {node_id} {IS_RELIABLE} {SLEEP_TIME}")


def subscriber_job(path: str, node_id: int):
    subcriber_path = os.path.join(path, "MySubscriber")
    os.system(f"{subcriber_path} {node_id} {IS_RELIABLE}")


def node_job(path: str, node_id: int):
    pub = threading.Thread(target=publisher_job, args=(path, node_id, ))
    sub = threading.Thread(target=subscriber_job, args=(path, node_id, ))

    pub.start()
    sub.start()

    pub.join()
    sub.join()


def build_nodes():
    subprocess.run(["cmake", "build/"], capture_output=subprocess.DEVNULL)
    subprocess.run(["make", "-C", "build/"], capture_output=subprocess.DEVNULL)


def main():
    build_nodes()

    path = "/home/flyotlin/Program/fastdds/workspace_MaxPacketLoss/build"

    nodes = []
    for i in range(NODE_NUM):
        node = threading.Thread(target=node_job, args=(path, i, ))
        node.start()
        nodes.append(node)

    for node in nodes:
        node.join()


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Provide 3 arguments!")
        exit()

    IS_RELIABLE = "true" if sys.argv[1] == "true" else "false"
    NODE_NUM = int(sys.argv[2])
    SLEEP_TIME = int(sys.argv[3])
    # print(IS_RELIABLE, NODE_NUM, SLEEP_TIME)

    main()


# 需要調整的 Qos 是 reliability

# - Publisher:
#     - DomainParticipantQos
#     - PublisherQos
#     - DataWriterQos -> 可以調整 reliability []
#     - TopicQos -> 可以調整 reliability []
# - Subscriber:
#     - DomainParticipantQos
#     - SubscriberQos
#     - DataReaderQos -> 可以調整 reliability []
#     - TopicQos -> 可以調整 reliability []

