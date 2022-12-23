var geocodingKey = "AIzaSyDX-JdhG_Ft90f6gtYGvBtNTqtdr9pZNE4"

function init() {
    block_tables();
    document.getElementById('detail-page').setAttribute('style', 'display:none');
    document.getElementById('loc').required = true;
    document.getElementById('no-record').setAttribute('style', 'display:none')
}

function clear_search() {
    clear_table('result-table');
    clear_table('table-sorted-by-name');
    clear_table('table-sorted-by-rating');
    clear_table('table-sorted-by-distance');
    clear_table('table-sorted-by-name-desc');
    clear_table('table-sorted-by-rating-desc');
    clear_table('table-sorted-by-distance-desc');
    clear_table('table-sorted-by-distance-desc');
    clear_details();
}

function clear_table(ele) {
    chi = document.getElementById(ele).children[0].children;
    while (chi.length >= 2) {
        chi[1].remove();
    }
}

function block_tables() {
    block_table('result-table');
    block_table('table-sorted-by-name');
    block_table('table-sorted-by-rating');
    block_table('table-sorted-by-distance');
    block_table('table-sorted-by-name-desc');
    block_table('table-sorted-by-rating-desc');
    block_table('table-sorted-by-distance-desc');
    block_table('no-record');
    block_details();
}

function block_table(ele) {
    document.getElementById(ele).setAttribute('style', 'display:none');
}


function clickName(p) {
    var request = new XMLHttpRequest();
    request.open("GET", "/detail?id=" + p.id, true);
    request.onreadystatechange = function () {
        if (request.readyState == 4 && request.status == 200) {
            res = JSON.parse(this.responseText);
            display_details(res);
            window.location.href = '#detail-page';
        }
    };
    request.send(null);
}



function display_details(res) {
    console.log(res)
    document.getElementById('detail-page').setAttribute('style', 'display:inline-block');
    clear_details();
    document.getElementById('business-name').innerHTML = res.name;

    detail_page = document.getElementById('detail-page');
    detail_table = document.createElement('table');
    detail_table.setAttribute('id', 'detail-table')
    num = 0;
    if (res.hasOwnProperty('hours') && res.hours.length != 0 && res.hours[0].hasOwnProperty('is_open_now') && res.hours[0].is_open_now!=null) {
        var is_open=res.hours[0].is_open_now;
        td = document.createElement('td');
        td.setAttribute('id','status-td');
        div1=document.createElement('div');
        div1.setAttribute('class','info-titles');
        div1.setAttribute('id','status-title');
        div1.innerHTML='Status';
        div = document.createElement('div');
        div.setAttribute('id', 'status-div');
        if (is_open == false) {
            div.appendChild(document.createTextNode('Closed'));
            div.setAttribute('class','is-closed');
        } else {
            div.appendChild(document.createTextNode('Open Now'));
            div.setAttribute('class','is-open');
        }
        div0=document.createElement('div');
        div0.setAttribute('id','status-container');
        div0.appendChild(div1);
        div0.appendChild(div);
        td.appendChild(div0);
        if (num == 0) {
            num = 1;
            tr = document.createElement('tr');
            tr.appendChild(td);

        } else {
            num = 0;
            tr.appendChild(td);
            detail_table.appendChild(tr);
        }
    }
    if (res.hasOwnProperty('categories') && res.categories.length != 0) {
        td = document.createElement('td');
        td.setAttribute('id','categories-td')
        div1=document.createElement('div');
        div1.setAttribute('class','info-titles');
        div1.innerHTML='Category';
        div = document.createElement('div');
        div.setAttribute('class','infos');
        cat = '';
        res.categories.forEach((c, i) => {
            cat += c.title;
            cat += ' | ';
        });
        cat = cat.slice(0, -3);
        div.appendChild(document.createTextNode(cat));
        div0=document.createElement('div');
        div0.setAttribute('class','info-container');
        div0.appendChild(div1);
        div0.appendChild(div);
        td.appendChild(div0);
        if (num == 0) {
            num = 1;
            tr = document.createElement('tr');
            tr.appendChild(td);

        } else {
            num = 0;
            tr.appendChild(td);
            detail_table.appendChild(tr);
        }

    }
    if (res.hasOwnProperty('location') && res.location.hasOwnProperty('display_address') && res.location.display_address.length != 0) {
        td = document.createElement('td');
        td.setAttribute('id','location-td');
        div1=document.createElement('div');
        div1.setAttribute('class','info-titles');
        div1.innerHTML='Address';
        div = document.createElement('div');
        da = res.location.display_address[0] + ' ' + res.location.display_address[1];
        div.appendChild(document.createTextNode(da));
        div.setAttribute('class','infos');
        div0=document.createElement('div');
        div0.setAttribute('class','info-container');
        div0.appendChild(div1);
        div0.appendChild(div);
        td.appendChild(div0);
        if (num == 0) {
            num = 1;
            tr = document.createElement('tr');
            tr.appendChild(td);

        } else {
            num = 0;
            tr.appendChild(td);
            detail_table.appendChild(tr);
        }
    }
    if (res.hasOwnProperty('display_phone') && res.display_phone != null && res.display_phone.length != 0) {
        td = document.createElement('td');
        td.setAttribute('id','phone-td')
        div1=document.createElement('div');
        div1.setAttribute('class','info-titles');
        div1.innerHTML='Phone Number';
        div = document.createElement('div');
        div.setAttribute('class','infos');
        div.appendChild(document.createTextNode(res.display_phone));
        div0=document.createElement('div');
        div0.setAttribute('class','info-container');
        div0.appendChild(div1);
        div0.appendChild(div);
        td.appendChild(div0);
        if (num == 0) {
            num = 1;
            tr = document.createElement('tr');
            tr.appendChild(td);

        } else {
            num = 0;
            tr.appendChild(td);
            detail_table.appendChild(tr);
        }
    }else if(res.hasOwnProperty('phone') && res.phone != null && res.phone.length != 0){
        td = document.createElement('td');
        td.setAttribute('id','phone-td')
        div1=document.createElement('div');
        div1.setAttribute('class','info-titles');
        div1.innerHTML='Phone Number';
        div = document.createElement('div');
        div.setAttribute('class','infos');
        div.appendChild(document.createTextNode(res.phone));
        div0=document.createElement('div');
        div0.setAttribute('class','info-container');
        div0.appendChild(div1);
        div0.appendChild(div);
        td.appendChild(div0);
        if (num == 0) {
            num = 1;
            tr = document.createElement('tr');
            tr.appendChild(td);

        } else {
            num = 0;
            tr.appendChild(td);
            detail_table.appendChild(tr);
        }
    }
    if (res.hasOwnProperty('transactions') && res.transactions.length != 0) {
        td = document.createElement('td');
        td.setAttribute('id','transaction-td');
        div1=document.createElement('div');
        div1.setAttribute('class','info-titles');
        div1.innerHTML='Transactions Supported';
        div = document.createElement('div');
        div.setAttribute('class','infos');
        tra = '';
        res.transactions.forEach((c, i) => {
            c=c.charAt(0).toUpperCase()+c.slice(1);
            tra += c;
            tra += ' | ';
        })
        tra = tra.slice(0, -3);
        div.appendChild(document.createTextNode(tra));
        div0=document.createElement('div');
        div0.setAttribute('class','info-container');
        div0.appendChild(div1);
        div0.appendChild(div);
        td.appendChild(div0);
        if (num == 0) {
            num = 1;
            tr = document.createElement('tr');
            tr.appendChild(td);

        } else {
            num = 0;
            tr.appendChild(td);
            detail_table.appendChild(tr);
        }
    }
    if (res.hasOwnProperty('price') && res.price != null) {
        td = document.createElement('td');
        td.setAttribute('id','price-td');
        div1=document.createElement('div');
        div1.setAttribute('class','info-titles');
        div1.innerHTML='Price';
        div = document.createElement('div');
        div.setAttribute('class','infos');
        div.appendChild(document.createTextNode(res.price));
        div0=document.createElement('div');
        div0.setAttribute('class','info-container');
        div0.appendChild(div1);
        div0.appendChild(div);
        td.appendChild(div0);
        if (num == 0) {
            num = 1;
            tr = document.createElement('tr');
            tr.appendChild(td);

        } else {
            num = 0;
            tr.appendChild(td);
            detail_table.appendChild(tr);
        }
    }

    td = document.createElement('td');
    td.setAttribute('id','moreInfo-td');
    div1=document.createElement('div');
    div1.setAttribute('class','info-titles');
    div1.innerHTML='More info';
    div = document.createElement('div');
    div.setAttribute('class','infos');
    a=document.createElement('a');
    a.href=res.url;
    //https://www.yelp.com/biz/big-ants-smash-burgers-los-angeles?adjust_creative=vroosL5uGpMpWJ6xp2BguQ&utm_campaign=yelp_api_v3&utm_medium=api_v3_business_lookup&utm_source=vroosL5uGpMpWJ6xp2BguQ
    a.target="_blank";
    a.innerHTML='Yelp';
    a.setAttribute('id','yelp-link');
    div.appendChild(a);
    div0=document.createElement('div');
    div0.setAttribute('class','info-container');
    div0.appendChild(div1);
    div0.appendChild(div);
    td.appendChild(div0);
    if (num == 0) {
        num = 1;
        tr = document.createElement('tr');
        tr.appendChild(td);

    } else {
        num = 0;
        tr.appendChild(td);
        detail_table.appendChild(tr);
    }

    if (num == 1) {
        detail_table.appendChild(tr);
    }
    detail_page.appendChild(detail_table);


    display_photos(res,detail_page);
}


function display_photos(res, detail_page) {
    container=document.createElement('div');
    container.setAttribute('id','photos-container');
    if (res.hasOwnProperty('photos') && res.photos != null) {
        photos = res.photos;
        for (let i = 0; i < Math.min(3, photos.length); i++) {
            div = document.createElement('div');
            div.setAttribute('class','photo-cell');
            img_div=document.createElement('div');
            img_div.setAttribute('class','photo-div');
            img = new Image();
            img.src = photos[i];
            img_div.appendChild(img);
            div.appendChild(img_div);
            index = i + 1;
            pi_div=document.createElement('div');
            pi_div.setAttribute('class','photo-index-div');
            pi_div.innerHTML='Photo '+index;
            div.appendChild(pi_div);
            container.appendChild(div);
        }
    }
        detail_page.appendChild(container);
}


function clear_details() {
    bn = document.getElementById('business-name');
    bn.innerHTML = '';
    t = document.getElementById('detail-page');
    while (t.children.length != 2) {
        t.children[2].remove();
    }
}



function geocoding(location, term, categories, radius) {
    let lat = 0, lng = 0;
    var words = location.split(/[, ]+/)
    var p = '';

    for (i in words) {
        p += words[i]
        p += '+'
    }
    p = p.slice(0, -1);
    var request = new XMLHttpRequest();

    request.open("GET", "https://maps.googleapis.com/maps/api/geocode/json?address="+p+"&key=AIzaSyDX-JdhG_Ft90f6gtYGvBtNTqtdr9pZNE4", true);
    request.onreadystatechange = function () {
        if (request.readyState == 4 && request.status == 200) {
            res = JSON.parse(this.responseText);
            if(res.results.length==0){
                display_no_result();
            }else{
                lat = res.results[0].geometry.location.lat;
                lng = res.results[0].geometry.location.lng;
                get_search_results(lat, lng, term, categories, radius);
            }
        }
    }
    request.send(null)
}


function onChangeCheckbox() {
    if (document.getElementById('auto-detect-check').checked) {
        document.getElementById('loc').required = false;
        document.getElementById('loc').value = '';
        document.getElementById('loc').disabled = true;
    } else {
        document.getElementById('loc').required = true;
        document.getElementById('loc').disabled = false;
    }
}



function get_latlng_by_ip(term, categories, radius) {
    var request = new XMLHttpRequest();
    // request.open('GET', '/ipinfo', true);
    request.open('GET','https://ipinfo.io/?token=f20bca2f6b9588',true);
    request.onreadystatechange = function () {
        if (request.readyState == 4 && request.status == 200) {
            res = JSON.parse(this.responseText);
            latlng = res.loc.split(',')
            lat = latlng[0]
            lng = latlng[1]
            get_search_results(lat, lng, term, categories, radius)
        }
    }
    request.send(null);
}

function click_sorted_by_name() {
    if (document.getElementById('table-sorted-by-name').children[0].children.length < 2) {
        createSortedTable('table-sorted-by-name', 'name', 0);
    }
    block_tables();
    document.getElementById('table-sorted-by-name').setAttribute('style', 'display:block');
}

function click_sorted_by_rating() {
    if (document.getElementById('table-sorted-by-rating').children[0].children.length < 2) {
        createSortedTable('table-sorted-by-rating', 'rating', 0);
    }
    block_tables();
    document.getElementById('table-sorted-by-rating').setAttribute('style', 'display:block');
}

function click_sorted_by_distance() {
    if (document.getElementById('table-sorted-by-distance').children[0].children.length < 2) {
        createSortedTable('table-sorted-by-distance', 'distance', 0);
    }
    block_tables();
    document.getElementById('table-sorted-by-distance').setAttribute('style', 'display:block');
}


function click_sorted_by_name_desc() {
    if (document.getElementById('table-sorted-by-name-desc').children[0].children.length < 2) {
        createSortedTable('table-sorted-by-name-desc', 'name', 1);
    }
    document.getElementById('table-sorted-by-name').setAttribute('style', 'display:none');
    document.getElementById('table-sorted-by-name-desc').setAttribute('style', 'display:block');
}

function click_sorted_by_rating_desc() {
    if (document.getElementById('table-sorted-by-rating-desc').children[0].children.length < 2) {
        createSortedTable('table-sorted-by-rating-desc', 'rating', 1);
    }
    document.getElementById('table-sorted-by-rating').setAttribute('style', 'display:none');
    document.getElementById('table-sorted-by-rating-desc').setAttribute('style', 'display:block');
}

function click_sorted_by_distance_desc() {
    if (document.getElementById('table-sorted-by-distance-desc').children[0].children.length < 2) {
        createSortedTable('table-sorted-by-distance-desc', 'distance', 1);
    }
    document.getElementById('table-sorted-by-distance').setAttribute('style', 'display:none');
    document.getElementById('table-sorted-by-distance-desc').setAttribute('style', 'display:block');
}

function display_no_result(){
    clear_search();
    block_tables();
    document.getElementById('no-record').setAttribute('style','display:inline-block');
}


function block_details(){
    document.getElementById('detail-page').setAttribute('style','display:none');
}