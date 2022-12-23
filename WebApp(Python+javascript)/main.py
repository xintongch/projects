
from flask import Flask,request, make_response
import requests

app = Flask(__name__)


ipinfoToken = "f20bca2f6b9588"
ipinfoUrl = "https://ipinfo.io/?token="

geocodingUrl = "https://maps.googleapis.com/maps/api/geocode/json"
geocodingKey = "AIzaSyDX-JdhG_Ft90f6gtYGvBtNTqtdr9pZNE4"

ClientID = "OmUeVfDXl7H-CNWkcTq7Aw"
APIKey = "rYMJu1j6WPJ7XVFobeWpUqh9eHVHBmdPF-VbMr1uR4k1Aakh12gzs0evUrB2MEyZce_WOIqv_8TjgB-aYNEgbEpsVBiTaDppLLim6kbaQzNBNZkz2F995PaWGeAcY3Yx"

yelpURL = "https://api.yelp.com/v3/businesses/search"
businessURL = "https://api.yelp.com/v3/businesses/"

headers = {
    "Authorization": "Bearer rYMJu1j6WPJ7XVFobeWpUqh9eHVHBmdPF-VbMr1uR4k1Aakh12gzs0evUrB2MEyZce_WOIqv_8TjgB-aYNEgbEpsVBiTaDppLLim6kbaQzNBNZkz2F995PaWGeAcY3Yx"
}


@app.route("/", methods=['GET'])
def load_homepage():
    return app.send_static_file('index.html')



@app.route("/search", methods=['GET'])
def get_search_result():
    term = request.args.get('key')
    latitude = request.args.get('lat')
    longitude = request.args.get('long')
    categories = request.args.get('category')
    radius = request.args.get('distance')

    url = f'{yelpURL}?term={term}&latitude={latitude}&longitude={longitude}&categories={categories}&radius={radius}'
    response = requests.get(url, headers=headers).json()
    return make_response(response, 200)



# @app.route("/searchyelp", methods=['GET'])
# def get_sample_search_result():
#     term = request.args.get('key')
#     latitude = request.args.get('lat')
#     longitude = request.args.get('long')
#     categories = request.args.get('category')
#     radius = request.args.get('distance')

#     url = f'{yelpURL}?term={term}&latitude={latitude}&longitude={longitude}&categories={categories}&radius={radius}'

#     response = requests.get(url, headers=headers).json()
#     return response



@app.route("/detail", methods=["GET"])
def get_business_detail():
    id = request.args.get('id')
    url = f'{businessURL}{id}'
    response = requests.get(url, headers=headers).json()
    return make_response(response, 200)


# @app.route("/geocoding", methods=["GET"])
# def get_geocoding():
#     address = request.args.get('location')
#     url = f'{geocodingUrl}?address={address}&key={geocodingKey}'
#     response = requests.get(url).json()
#     return make_response(response, 200)


# @app.route("/ipinfo", methods=["GET"])
# def get_loc_by_ip():
#     url = f'{ipinfoUrl}{ipinfoToken}'
#     response = requests.get(url).json()
#     return make_response(response, 200)



if __name__ == '__main__':
    app.run(host='127.0.0.1', port=8080, debug=True)
