from flask import Flask, render_template, request
from coapthon.client.helperclient import HelperClient


app = Flask(__name__)


COAP_SERVER_IP = '192.168.43.96'
# COAP_SERVER_PORT = 5683
# COAP_LED_ENDPOINT = '/LED'


# def send_coap_request(data):
#     try:
#         client = HelperClient(server=(COAP_SERVER_IP, COAP_SERVER_PORT))
#         response = client.post(COAP_LED_ENDPOINT, data)
#         return response
#     except Exception as e:
#         return f"Error: {e}"
#     finally:
#         client.stop()

@app.route('/')
def index():

    return render_template('index.html', state='OFF')

@app.route('/led_on', methods=['POST'])
def led_on():
    client = HelperClient(server=(COAP_SERVER_IP, 5683))
    response = client.post('/LED', 'On')
    #response = send_coap_request('On')
    client.stop()
    return render_template('index.html', state='ON')

@app.route('/led_off', methods=['POST'])
def led_off():
    client = HelperClient(server=(COAP_SERVER_IP, 5683))
    response = client.post('/LED', 'Off')
    return render_template('index.html', state='OFF')

if __name__ == '__main__':
    app.run(debug=True)