document.getElementById('input-form').addEventListener('reset',(e)=>{
    e.preventDefault();
    document.getElementById('detail-page').setAttribute('style','display:none');
    document.getElementById('result-page').setAttribute('style','display:none');
    document.getElementById('no-record').setAttribute('style','display:none');

    document.getElementById('keyword').value = ''
    document.getElementById('loc').value = ''
    document.getElementById('default-select').selected = 'selected'
    document.getElementById('auto-detect-check').checked=false;
    document.getElementById('loc').disabled=false;
    document.getElementById('dis').value='';
});

document.getElementById('input-form').addEventListener('submit',(e)=>{
    e.preventDefault();
    var term = document.getElementById("keyword").value;
    if(term==''){
        return;
    }
    let diss=document.getElementById("dis").value*1609.34;
    if(document.getElementById("dis").value==''){
        diss=10*1609.34;
    }
    var radius = Math.round(diss);
    var categories = document.getElementById("ca").value;
    if (categories == 'Default') {
        categories = 'all';
    }else if(categories=='Arts & Entertainment'){
        categories='arts'
    }else if(categories=='Health & Medical'){
        categories='health'
    }else if(categories=='Hotels & Travel'){
        categories='hotelstravel'
    }else if(categories=='Food'){
        categories='food'
    }else if(categories=='Professional Services'){
        categories='professional'
    }

    if (document.getElementById('auto-detect-check').checked) {
        get_latlng_by_ip(term, categories, radius);
    } else {
        var location = document.getElementById("loc").value;
        if(location==''){
            return;
        }
        geocoding(location, term, categories, radius);
    }
});


var data=[];

function displayResult(res) {
    document.getElementById('result-page').setAttribute('style','display:inline-block');
    clear_search();
    block_tables();

    if(!res.hasOwnProperty('businesses') || res.businesses.length==0){
        document.getElementById('no-record').setAttribute('style','display:inline-block');
        return;
    }else{
        document.getElementById('result-table').setAttribute('style','display:block');
    }

    result_table_body = document.getElementById('result-table').children[0];

    business_list = res.businesses;
    var index = 0;
    business_list.forEach(element => {
        if(index==20){
            return;
        }
        data[index]={}
        index += 1;
        table_row = document.createElement('tr');

        table_data = document.createElement('td');
        table_data.innerHTML = index;
        table_row.appendChild(table_data);

        table_data = document.createElement('td');
        img = new Image();
        img.src = element['image_url'];
        img.setAttribute("width", "100px");
        img.setAttribute("height", "100px");
        table_data.appendChild(img);
        table_data.setAttribute('style','padding:5px')
        table_row.appendChild(table_data);
        data[index-1].image_url=element.image_url;

        table_data = document.createElement('td');
        td_div=document.createElement('div');
        td_div.innerHTML = element['name'];
        td_div.setAttribute('style', 'cursor:pointer');
        td_div.setAttribute('onclick', 'clickName(this)');
        td_div.setAttribute('class','result-name');
        td_div.setAttribute('id', element['id']);
        table_data.appendChild(td_div);
        table_row.appendChild(table_data);
        data[index-1].id=element.id;
        data[index-1].name=element.name;

        table_data = document.createElement('td');
        table_data.innerHTML = element['rating']
        table_row.appendChild(table_data);
        data[index-1].rating=element.rating;

        table_data = document.createElement('td');
        table_data.innerHTML = (element.distance/1609.34).toFixed(2);
        table_row.appendChild(table_data);
        data[index-1].distance=(element.distance/1609.34).toFixed(2);

        result_table_body.appendChild(table_row);
        result_table_body.setAttribute('width', '100%')
    });
}

function get_search_results(lat, lng, term, categories, radius) {
    var request = new XMLHttpRequest();
    request.open("GET", "/search?key=" + term + "&lat=" + lat + "&long=" + lng + "&category=" + categories + "&distance=" + radius, true);
    
    request.onreadystatechange = function () {
        if (request.readyState == 4 && request.status == 200) {
            res = JSON.parse(this.responseText)
            displayResult(res);
            window.location.href = '#result-page';
        }
    };
    request.send(null);
}


function createSortedTable(table,ele,isDesc){
    if(isDesc==0){
        data=data.sort((a,b)=>{return a[ele]>b[ele]?1:a[ele]<b[ele]?-1:0})
    }else{
        data=data.sort((a,b)=>{return a[ele]<b[ele]?1:a[ele]>b[ele]?-1:0})
    }

    result_table_body = document.getElementById(table).children[0];
   
    var index = 0
    data.forEach(element => {
        index += 1;
        table_row = document.createElement('tr');

        table_data = document.createElement('td');
        table_data.innerHTML = index
        table_row.appendChild(table_data);

        table_data = document.createElement('td');
        img = new Image();
        img.src = element.image_url;
        img.setAttribute("width", "100px");
        img.setAttribute("height", "100px");
        table_data.appendChild(img);
        table_data.setAttribute('style','padding:5px')
        table_row.appendChild(table_data);

        table_data = document.createElement('td');
        td_div=document.createElement('div');
        td_div.innerHTML = element['name'];
        td_div.setAttribute('style', 'cursor:pointer');
        td_div.setAttribute('onclick', 'clickName(this)');
        td_div.setAttribute('class','result-name');
        td_div.setAttribute('id', element['id']);
        table_data.appendChild(td_div);
        table_row.appendChild(table_data);

        table_data = document.createElement('td');
        table_data.innerHTML = element['rating']
        table_row.appendChild(table_data);

        table_data = document.createElement('td');
        table_data.innerHTML = element.distance;
        table_row.appendChild(table_data);

        result_table_body.appendChild(table_row);
        result_table_body.setAttribute('width', '100%')
    });
}








