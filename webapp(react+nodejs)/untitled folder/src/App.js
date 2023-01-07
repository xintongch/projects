// import logo from './logo.svg';
import './App.css';
import React from 'react';
import Header from './components/Header'
import Form from './components/Form';
// import { useState } from 'react';
import Table from './components/Table';
import $ from 'jquery';
// import Nav from './components/Nav'
import { Outlet, Link } from "react-router-dom";
import { BrowserRouter as Router, Route, Routes } from "react-router-dom";
import Bookings from './components/Bookings'

function App() {
  const [data, setData] = React.useState(null);

  // React.useEffect(() => {
  //   fetch("/api")
  //     .then((res) => res.json())
  //     .then((dat) => {
  //       console.log("ddate:", dat);
  //       // setData(dat.businesses)
  //       // console.log("data=", { data });
  //     });
  //     // console.log("after set data=", data);
  // }, []);


  const cs = (term, lat, lng, categories, radius, flag) => {
    // console.log("output:", output);
    // let yelpURL = "/api/yelpSearch";
    // let params=new HttpParams().set('term',term).set('latitude',lat).set('longitude',lng).set('categories',categories).set('radius',radius);
    // let headers=this.headers;
    // return this.http.get < any > (yelpURL, { params, headers })
    if (flag == 0) {
      fetch("/api/yelpSearch?term=" + term + "&latitude=" + lat + "&longitude=" + lng + "&categories=" + categories + "&radius=" + radius)
        .then((res) => res.json())
        .then((dat) => {
          console.log("da:", dat);
          // setData("hhiii");
          if (dat.hasOwnProperty("businesses")) {
            setData(dat.businesses);
            console.log("data=", data)
            if (dat.businesses.length == 0) {
              $("#results").css({ display: 'inline-block' })
              $('#no-result').css({
                display: 'inline-block',
              });
            } else {
              $("#results").css({ display: 'inline-block' })
              $("#rt-container").css({
                display: 'inline-block'
              })
              $('#no-result').css({
                display: 'none',
              });
            }
          }
          else {
            $("#results").css({ display: 'inline-block' })
            $('#no-result').css({
              display: 'inline-block',
            });
          }
          //         React.useEffect(() => {
          //   console.log("dd=",data)
          // }, []);   

        });
    } else {
      $('#no-result').css({
        display: 'inline-block',
      });
    }

  }


  // React.useEffect(() => {
  //   console.log("dd=",data)
  // }, []);



  return (
    <>
      <Router>
      <Routes>
        <Route path='/' element={
          <div className="App">
            {/* <Link to="/blogs">Blogs</Link> */}
            {/* <Nav /> */}
            <Header />
            <Form clickSubmit={cs} />
            <Table data={!data ? null : data} />
          </div>}></Route>
        <Route path='/bookings' element={<Bookings />}></Route>
      </Routes>
      </Router>
    </>
  );

}

// const s = {
//   'backgroundImage': "linear-gradient(0deg, rgba(200, 210, 255, 0.45), rgba(200, 210, 255, 0.5) 30%,rgba(249, 249, 249, 1) 90%),url('assets/city.jpg')",
//     'backgroundSize': 'cover 100%',
//     'backgroundPosition': 'center bottom',
//   'backgroundRepeat': 'no-repeat',
//   'height':'100%',
// }

export default App;
