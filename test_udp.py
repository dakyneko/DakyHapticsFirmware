#!/usr/bin/env python3

import socket
from serial import Serial
from struct import pack
from argparse import ArgumentParser

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('host', type=str, help='ip address or hostname')
    parser.add_argument('port', type=int, default=1234)
    args = parser.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    remote = (args.host, args.port)


def print_(*args, **kwargs):
    print(*args, end='', flush=True, **kwargs)

def write_raw(raw):
    s.sendto(raw, remote)

def write(command, data_fmts='', *args):
    data = pack('<c'+ data_fmts, command.encode(), *args)
    write_raw(data)

def blink():
    write('/')

def set_motor(motor, value):
    write('B', 'BB', motor, value)
