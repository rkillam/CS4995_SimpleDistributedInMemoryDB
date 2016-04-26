import sys
import zmq


context = zmq.Context()


class SlaveConnection(object):
    socket_fmt = 'tcp://*:{}'

    def __init__(self, port):
        self.port = port
        self.endpoint = SlaveConnection.socket_fmt.format(self.port)

        self.socket = context.socket(zmq.PAIR)
        self.socket.bind(self.endpoint)

    def __getitem__(self, key):
        self.socket.send_pyobj({
            'type': 'GET',
            'key': key
        })

        return self.socket.recv()

    def __setitem__(self, key, val):
        self.socket.send_pyobj({
            'type': 'SET',
            'key': key,
            'val': val
        })

    def __len__(self):
        self.socket.send_pyobj({
            'type': 'LEN'
        })

        return int(self.socket.recv_string())


class DistDict(object):
    def __init__(self, starting_port, num_slaves, local_storage=True):
        self.slaves = [SlaveConnection(starting_port+i) for i in range(num_slaves)]

        if local_storage:
            self.slaves.append({})

    def __getitem__(self, key):
        idx = hash(key) % len(self.slaves)

        return self.slaves[idx][key]

    def __setitem__(self, key, val):
        idx = hash(key) % len(self.slaves)
        self.slaves[idx][key] = val

    def __len__(self):
        for slave in self.slaves:
            print('{} has {} items'.format(slave.endpoint, len(slave)))
        return sum(len(slave) for slave in self.slaves)


def main(inserts_file, num_slaves):
    dd = DistDict(4040, num_slaves, local_storage=False)

    num_lines = 0
    with open(inserts_file, 'r') as f:
        for line in f:
            num_lines += 1
            key, val = line.strip().split(',')
            dd[key] = val

    print('Added {} / {} items'.format(len(dd), num_lines))


if __name__ == '__main__':
    main(sys.argv[1], int(sys.argv[2]))
