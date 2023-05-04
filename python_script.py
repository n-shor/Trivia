import socket


def main():
    SERVER_IP = "127.0.0.1"
    PORT = 8080

    if PORT < 1024 or PORT > 65535:
        print("Invalid port number. It must be in the range 1024 - 65535.")
        return

    # Create a socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        client_socket.connect((SERVER_IP, PORT))

        # Receive 5 characters from the server
        data = client_socket.recv(5)
        print(f"Received from server: {data.decode()}")

        # If the received characters are "Hello", send "Hello" back
        if data.decode() == "Hello":
            client_socket.sendall(b"Hello")

    except socket.error as e:
        print(f"Error: {e}")
    
    finally:
        # Close the connection
        client_socket.close()

if __name__ == "__main__":
    main()
