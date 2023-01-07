const express=require('express')
const app=express()
var bodyParser = require('body-parser')
const fs = require('fs');
// var csv = require("csvtojson");
var jsonParser = bodyParser.json()
const axios = require("axios");

const PORT = process.env.PORT || 5050
app.listen(PORT, () => {
    console.log(`Server listening on portt ${PORT}...`)
})

const headers = {
    headers: {
        'Authorization': 'Bearer rYMJu1j6WPJ7XVFobeWpUqh9eHVHBmdPF-VbMr1uR4k1Aakh12gzs0evUrB2MEyZce_WOIqv_8TjgB-aYNEgbEpsVBiTaDppLLim6kbaQzNBNZkz2F995PaWGeAcY3Yx'
    }
};


app.get('/api/geocode', async(req, res) => {
    ress={}
    let address = req.query.address;
    let key='AIzaSyDX-JdhG_Ft90f6gtYGvBtNTqtdr9pZNE4'
    let geocodingUrl = "https://maps.googleapis.com/maps/api/geocode/json?address="+address+"&key="+key;
    
    await axios.get(geocodingUrl)
    .then(response => {
        ress['data']=response.data
    });
    res.status(200).send(ress.data);
})

app.get('/api/yelpSearch', async(req, res) => {
    ress={}
    let term = req.query.term
    let latitude = req.query.latitude
    let longitude = req.query.longitude
    let categories = req.query.categories
    let radius = req.query.radius
    let yelpURL = "https://api.yelp.com/v3/businesses/search?term=" + term + "&latitude=" + latitude + "&longitude=" + longitude + "&categories=" + categories + "&radius=" + radius;
    
    let params = {
        params: {
            'term': term,
            'latitude': latitude,
            'longitude': longitude,
            'categories': categories,
            'radius': radius
        }
    }
    
    await axios.get(yelpURL, headers)
    .then(response => {
        ress['data']=response.data
    });

    res.status(200).send(ress.data);
});

app.get('/api/detail', async(req, res) => {
    ress={}
    let id = req.query.id;
    let yelpDetailURL = 'https://api.yelp.com/v3/businesses/' + id;
    
    await axios.get(yelpDetailURL, headers)
    .then(response => {
        ress['data']=response.data
    });
    res.status(200).send(ress.data);
})

app.get('/api/reviews', async (req, res) => {
    ress={}
    let id = req.query.id;
    let yelpReviewsURL = "https://api.yelp.com/v3/businesses/" + id + "/reviews";
    
    await axios.get(yelpReviewsURL, headers)
    .then(response => {
        ress['data']=response.data
    });
    res.status(200).send(ress.data);
})

app.get('/api/autoComplete', async(req, res) => {
    ress={}
    let value = req.query.text;
    let autoCompleteURL = "https://api.yelp.com/v3/autocomplete?text=" + value;
    
    await axios.get(autoCompleteURL, headers)
    .then(response => {
        ress['data']=response.data
    });
    res.status(200).send(ress.data);
})

app.get('/api', (req, res) => {
    res.status(200).send(JSON.stringify({'res':'hello'}));
})

app.use(express.static(process.cwd()+"/frontend/build/"));
// app.use(express.static(process.cwd()+"/frontend/public/"));

module.exports=app;