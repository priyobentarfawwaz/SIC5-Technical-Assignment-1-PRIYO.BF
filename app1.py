from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/data', methods=['POST'])
def receive_data():
    if request.method == 'POST':
        data = request.get_json()
        if data:
            distance = data.get('distance')
            print(f"Received data: {data}")  # Print the received data for debugging
            
            response = {
                "message": "Data received",
                "distance": distance
            }
            return jsonify(response)
        else:
            return jsonify({"error": "No data received"}), 400
    else:
        return jsonify({"message": "Send POST requests to this endpoint"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
