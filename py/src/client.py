import sys
import zmq


port = sys.argv[1]

context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.connect('tcp://192.168.30.5:{}'.format(port))

kvs = {}

while True:
    msg = socket.recv_pyobj()

    if msg['type'] == 'GET':
        socket.send_string(kvs.get(msg['key'], 'DNE'))

    elif msg['type'] == 'SET':
        kvs[msg['key']] = msg['val']

    elif msg['type'] == 'LEN':
        socket.send_string(str(len(kvs)))
