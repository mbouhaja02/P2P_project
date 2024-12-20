import socket

def main():
    host = 'localhost'
    port = 12345

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        message = "Hello, Tracker!"
        s.sendall(message.encode())
        data = s.recv(1024)
        print('Received from server:', data.decode())

if __name__ == "__main__":
    main()
