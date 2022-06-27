def get_packet_loss_rate():
    # nodes = {
    #     "1": {
    #         "S": [1, 2, 3],
    #         "R": [2, 3]
    #     }
    # }
    lines = get_lines_from_log("logs/example")
    nodes = get_nodes_dict(lines)
    rates = get_total_loss_rate(nodes)

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

    for _, value in nodes.items():
        for _ in value["S"]:
            total_sent += 1
        for _ in value["R"]:
            total_received += 1

    return total_received / total_sent


if __name__ == "__main__":
    get_packet_loss_rate()
