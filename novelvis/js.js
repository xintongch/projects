


var parcoords = d3.parcoords()("#example")
.alpha(0.4);

  
  
  
    console.log('search-page');
    console.log("init");
    let self=this;
    document.getElementById('formFile')
  .addEventListener('change', function(){
    console.log("file input change");
    $("#im").html("")
    $("#pic_title").html("");
    $("#wc_title").html("");
    // $("#example").html("")
// window.location.reload()
$('#formFile').prop('disabled', true);
$('#clear').prop('disabled', false);
    self.readSingleFile(event)
  });



  function readSingleFile(e) {
    console.log("inside readSingleFile");
    var file = e.target.files[0];
    if (!file) {
      return;
    }
    var reader = new FileReader();
    let self=this;

    reader.onload = function(e) {
      var contents = e.target?.result;
      self.displayContents(contents)
    };
    reader.readAsText(file);
  }


  function displayContents(contents) {
    console.log("inside displaycontents")
    var self=this
    ntp_process(contents)
  }


  function ntp_process(contents){
      console.log("inside process")
      console.log("contents=",contents)
      let body=JSON.stringify({"content":contents});
      let processUrl="/process"
    //   return $http.post(processUrl,{"content":contents});

    fetch(processUrl, {
    method: 'POST',
    headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
    },
    // body: {"content":contents}
    body: JSON.stringify({"content":contents})
})
.then(response => response.json())
.then(res => {
  


          var fn=res.filename.split('/')
      console.log('last filename=',fn[fn.length-1]);
      var fin='/'+fn[fn.length-1]

      $("#im").append("<img src='"+fin+"' style='width:100%;height:100%;object-fit:cover'>");
      $("#pic_title").append("<h4>Closeness between other characters and the main character (at the middle)</h4>")
      $("#wc_title").append("<h4>Wordcloud for the novel</h4>")
      var da=res.data;

      var data=eval(res.data);
  var dimensions=Object.keys(data[0])
  
  console.log("data=",data)
  console.log("keys=",dimensions)

  parcoords
  .data(data)
  .dimensions(dimensions)
  .render()
  .brushMode("1D-axes");  // enable brushing



})


}