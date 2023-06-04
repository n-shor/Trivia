import socket
import json
import struct
import time


def send_message(client_socket, message_type, message_data):
    message_json = json.dumps(message_data).encode()
    message_size = len(message_json)
    message_header = struct.pack('!BI', message_type, message_size)

    print(f"Sending message (type {message_type}, size {message_size}): {message_json}")

    client_socket.sendall(message_header + message_json)


def recv_message(client_socket):
    header_data = client_socket.recv(5)
    message_type, message_size = struct.unpack('!BI', header_data)
    message_json = client_socket.recv(message_size).decode()

    try:
        message_data = json.loads(message_json)
    except json.JSONDecodeError:
        print(f"Invalid JSON received: {message_json}")
        message_data = None

    return message_type, message_data


def main():
    SERVER_IP = "127.0.0.1"
    PORT = 8080

    if PORT < 1024 or PORT > 65535:
        print("Invalid port number. It must be in the range 1024 - 65535.")
        return

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect((SERVER_IP, PORT))

        # Attempt to login with user1 (should fail because not yet registered)
        login_data = {"username": "user1", "password": "1234"}
        send_message(client_socket, 1, login_data)
        response_type, response_data = recv_message(client_socket)
        print(f"Received from server (type {response_type}): {response_data}")

        # Register user1
        signup_data = {"username": "user1", "password": "1234", "email": "user1@gmail.com"}
        send_message(client_socket, 2, signup_data)
        response_type, response_data = recv_message(client_socket)
        print(f"Received from server (type {response_type}): {response_data}")

        # Try to register user1 again (should fail because user1 already registered)
        send_message(client_socket, 2, signup_data)
        response_type, response_data = recv_message(client_socket)
        print(f"Received from server (type {response_type}): {response_data}")

        # Login with user1 (should succeed)
        send_message(client_socket, 1, login_data)
        response_type, response_data = recv_message(client_socket)
        print(f"Received from server (type {response_type}): {response_data}")

        # Attempt to login again with the same user (should fail because user1 already logged in)
        send_message(client_socket, 1, login_data)
        response_type, response_data = recv_message(client_socket)
        print(f"Received from server (type {response_type}): {response_data}")

        # Send "end" message when done
        end_data = {"message" : "end"}
        send_message(client_socket, 3, end_data)

    except socket.error as e:
        print(f"Error: {e}")

    finally:
        client_socket.close()


if __name__ == "__main__":
    main()
