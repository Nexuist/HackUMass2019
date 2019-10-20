from websocket_server import WebsocketServer
import serial


def new_client(client, server):
    ser = serial.Serial("/dev/cu.usbmodem14601", 115200)
    try:
        while True:
            server.send_message(client, ser.readline())
    except:
        # Hackathons!
        ser.close()
        pass


server = WebsocketServer(1337, host="127.0.0.1")
server.set_fn_new_client(new_client)
server.run_forever()
