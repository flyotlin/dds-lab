import os
import threading


IS_RELIABLE = "false"
NODE_NUM = 3


def publisher_job(path: str, node_id: int):
    publisher_path = os.path.join(path, "MyPublisher")
    os.system(f"{publisher_path} {node_id} {IS_RELIABLE}")


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
    os.system("cmake build/")
    os.system("make -C build/")


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
    main()
