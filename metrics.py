def get_packet_loss_rate(log_path: str):
    # nodes = {
    #     "1": {
    #         "S": [1, 2, 3],
    #         "R": [2, 3]
    #     }
    # }
    # # calculate packet loss rate by node dict
    # lines = get_lines_from_log(log_path)
    # nodes = get_nodes_dict(lines)
    # rates = get_total_loss_rate(nodes)

    # calculate by raw
    with open(log_path, "r") as f:
        lines = f.read().split("\n")
        if lines[-1] == "":
            lines.pop(-1)
    total_sent = 0
    total_received = 0
    for line in lines:
        dep = line.split(" ")
        if len(dep) != 3:
            continue
        action, _, _ = line.split(" ")
        if action == "S":
            total_sent += 1
        elif action == "R":
            total_received += 1
    # rates = total_received / total_sent   # 好像不太對欸
    rates = (total_sent - total_received) / total_sent

    # print(total_sent, total_received)
    print(rates)

def get_lines_from_log(path: str) -> list:
    with open(path, "r") as f:
        return f.read().split("\n")

def get_nodes_dict(lines: list) -> dict:
    nodes = {}

    for line in lines:
        action, node, index = line.split(" ")

        if node not in nodes:
            nodes[node] = {"S": [], "R": []}

        nodes[node][action].append(int(index))

    return nodes

def get_total_loss_rate(nodes: dict) -> dict:
    total_sent = 0
    total_received = 0

    print(nodes)

    for _, value in nodes.items():
        for _ in value["S"]:
            total_sent += 1
        for _ in value["R"]:
            total_received += 1

    return total_received / total_sent


if __name__ == "__main__":
    # log_path = "logs/reliable-1-10.log"
    # get_packet_loss_rate(log_path)

    for node in [1, 5, 10, 15, 20]:
        for time in [10, 50, 100, 300, 500]:
            log_path = f"logs/best-effort-4/best-effort-{node}-{time}.log"
            # print(log_path)
            get_packet_loss_rate(log_path)
            # print()
