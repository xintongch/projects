import $ from 'jquery';

const Form = ({ clickSubmit }) => {
    let selectedOptionId = "all"
    const clickClear = () => {
        console.log("clickClear");
        $("#results").css({display:'none'})
        $('.form-input').val('');
        $('.form-select').val('all');
        $('.form-checkbox').prop('checked', false);
        $("#location").attr("disabled", false);
        $('#no-result').css({
            display: 'none',
        });
        $('#dc-container').css({
            display: 'none',
        });
        $('#rt-container').css({
            display: 'none',
        });
    }
    const autoDetectOnChange = () => {
        console.log("auto detect change");
        if ($('#auto-detect').is(":checked")) {
            $('#location').attr("disabled", true);
            $('#location').val('');
        } else {
            $('#location').attr("disabled", false)
        }
    }
    // const clickSubmit = () => {
    //     {clickSubmit2('click')}
    // }

    const cs = (e) => {
        console.log("new test point")
        let term = $("#keyword").val();
        if (term == "") {
            return;
        }
        e.preventDefault();
        let location = $("#location").val();
        let categories = $("#category").val();
        let radius = "";
        if ($("#distance").val() == "") {
            radius = 16093;
        } else {
            radius = Math.round(Number($("#distance").val()) * 1609.34);
        }
        let lat = "34.0223519";
        let lng = "-118.285117";
        let flag = 0;

        // $("#location").attr("disabled", false);
        // $('#no-result').css({
        //     display: 'none',
        // });
        $('#dc-container').css({
            display: 'none',
        });
        $('#rt-container').css({
            display: 'none',
        });

        if (radius > 40000) {
            flag = 1;
            clickSubmit(term, lat, lng, categories, radius, flag);
        } else {
            if ($('#auto-detect').is(":checked")) {
                fetch("https://ipinfo.io/?token=f20bca2f6b9588")
                    .then((res) => res.json())
                    .then((data) => {
                        let latlng = data.loc.split(',');
                        let lat = latlng[0];
                        let lng = latlng[1];
                        clickSubmit(term, lat, lng, categories, radius, flag);
                    })
            } else {
                clickSubmit(term, lat, lng, categories, radius, flag);
            }
        }
        // else {
        //     fetch("/api/geocode?address=" + location)
        //         .then((res) => res.json())
        //         .then((data) => {
        //             console.log("geocode data=", data);
        //             let lat = data.results[0].geometry.location.lat;
        //             let lng = data.results[0].geometry.location.lng;
        //             clickSubmit(term,lat,lng,categories,radius,flag);
        //     })
        // }
        // clickSubmit("food","34.0223519","-118.285117","all","16093",flag);
        // "food","34.0223519","-118.285117","all","16093"

    }

    return (
        <div
            className="mb-5 text-center bg-white mx-auto px-md-5 px-sm-0 pt-md-4 pt-1 pb-2 pb-md-4 rounded-4 border-lightgray col-12 col-md-6" style={s}>
            <form id="search-form">
                <div className="text-center mb-1" style={s0}>
                    <h4>Bussiness search</h4>
                </div>
                <div className="row">
                    <div className="col mt-3 text-left">
                        <label className="ml-1" htmlFor="keyword">Keyword</label><span className="mx-1 text-danger">*</span>
                        <input className="form-input fs-19 pl-3 w-100 h-input border rounded-input" id="keyword" name="keyword" required />
                    </div>
                </div>
                <div className="row">
                    <div className="col-sm-6 text-left mt-0 pr-2">
                        <label htmlFor="distance">Distance</label>
                        <input className="form-input pl-3 fs-19 w-100 h-input border rounded-input" type="text" name="distance"
                            id="distance" placeholder="10" />
                    </div>
                    <div className="col-sm-6 text-left mt-0" style={s1} >
                        <label htmlFor="category">Category</label><span className="mx-1 text-danger">*</span>
                        <select name="category" id="category" className="form-select w-100 fs-19 h-input border pl-3 rounded-input"
                            required defaultValue={selectedOptionId}>
                            <option key="all" value="all">Default</option>
                            <option value="arts">Arts & Entertainment</option>
                            <option value="health">Health & Medical</option>
                            <option value="hotelstravel">Hotels & Travel</option>
                            <option value="food">Food</option>
                            <option value="professional">Professional Services</option>
                        </select>
                    </div>
                </div>
                <div className="row mt-1 mt-md-2">
                    <div className="col text-left">
                        <label htmlFor="location">Location</label><span className="mx-1 text-danger">*</span>
                        <input className="form-input pl-3 fs-19 w-100 h-input border rounded-input" type="text" name="location"
                            id="location" />
                    </div>
                </div>
                <div className="row mt-0">
                    <div className="col text-left">
                        <input className="mr-2 form-checkbox" type="checkbox" id="auto-detect" onChange={autoDetectOnChange} name="auto-detect" />
                        <label htmlFor="auto-detect"> Auto-detect my location</label>
                    </div>
                </div>
                <div className="row mt-2 pb-2">
                    <div className="col text-right pr-2">
                        <button type="submit"
                            className="mr-3 hover-pointer border-0 py-2 px-3 rounded-input hover-lightout w-submit h-button button-text-size text-white"
                            onClick={cs} style={s2}>Submit</button>
                    </div>
                    <div className="col text-left w-100 pl-0">
                        <button type="reset"
                            className="hover-lightout border-0 py-2 px-3 rounded-input hover-pointer w-clear h-button button-text-size text-white"
                            onClick={clickClear} style={s3}>Clear</button>
                    </div>
                </div>
            </form>
        </div>
    )
    // https://yelp-frontend-8-jfb.wl.r.appspot.com/api/yelpSearch?term=food&latitude=34.0223519&longitude=-118.285117&categories=all&radius=16093
}

const s = {
    'border': '1px lightgray solid'
}
const s0 = {
    "fontSize": "21px",
    "fontWeight": "bold",
}
const s1 = {
    'paddingRight': '66px'
}
const s2 = {
    "backgroundColor": "#cd4c54"
}
const s3 = {
    "backgroundColor": "#3875f5"
}

export default Form