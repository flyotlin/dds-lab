import os
import threading


def publisher_job(path: str, node_id: int):
    publisher_path = os.path.join(path, "MyPublisher")
    os.system(f"{publisher_path} {node_id}")


def subscriber_job(path: str, node_id: int):
    subcriber_path = os.path.join(path, "MySubscriber")
    os.system(f"{subcriber_path} {node_id}")


def node_job(path: str, node_id: int):
    pub = threading.Thread(target=publisher_job, args=(path, node_id, ))
    sub = threading.Thread(target=subscriber_job, args=(path, node_id, ))

    pub.start()
    sub.start()

    pub.join()
    sub.join()


def main():
    path = "/home/flyotlin/Program/fastdds/workspace_MaxPacketLoss/build"

    node = threading.Thread(target=node_job, args=(path, 1, ))
    node.start()
    node.join()


if __name__ == "__main__":
    main()
