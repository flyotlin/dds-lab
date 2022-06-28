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
    total_sent = 0
    total_received = 0
    for line in lines:
        action, _, _ = line.split(" ")
        if action == "S":
            total_sent += 1
        elif action == "R":
            total_received += 1
    rates = total_received / total_sent

    print(rates)
    print(total_sent, total_received)

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
    log_path = "logs/fourth.log"
    get_packet_loss_rate(log_path)
