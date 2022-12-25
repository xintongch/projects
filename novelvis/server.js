const express=require('express')
const app=express()
var bodyParser = require('body-parser')
const fs = require('fs');


var jsonParser = bodyParser.json()

const PORT = process.env.PORT || 8080
app.listen(PORT, () => {
    console.log(`Server listening on port ${PORT}...`)
})


app.use(bodyParser({limit: '50mb'}));

app.post('/process',jsonParser,(req,res)=>{

    var files = fs.readdirSync(process.cwd()).filter(fn => fn.startsWith('wordcloud'));
    // console.log("files=",files)
    if(files.length>0){
        files.forEach((file)=>{
                fs.unlinkSync(process.cwd()+'/'+file,function(err){
                if(err) return console.log(err);
                console.log('file deleted successfully');
            });
        }
        )
    }


    console.log("server post process")
    let text=req.body.content
    fs.writeFile("temp.txt", text,{
        encoding: "utf8",
        flag: "w",
        mode: 0o666
      },
      (err) => {
        if (err)
          console.log(err);
        else {
          console.log("File written successfully\n");
        }
    });

    const { spawn } = require('child_process');
    const pyProg = spawn('python3', ['./NovelTextProcessor.py']);

    pyProg.stdout.on('data', function(data) {
        console.log('inside post=',data.toString());
        var d=data.toString().split('|')
        res.status(200).send(JSON.stringify({'filename':d[0],'data':d[1]}))

        
    });
})

app.use(express.static(process.cwd()));

app.get('/*',(req,res)=>{
    // console.log("test...")
    var files = fs.readdirSync(process.cwd()).filter(fn => fn.startsWith('wordcloud'));
    // console.log("files=",files)
    if(files.length>0){
        files.forEach((file)=>{
                fs.unlinkSync(process.cwd()+'/'+file,function(err){
                if(err) return console.log(err);
                console.log('file deleted successfully');
            });
        }
        )
    }
    res.sendFile(process.cwd()+"/index.html")
})

module.exports=app;