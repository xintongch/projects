const express=require('express')
const app=express()
var bodyParser = require('body-parser')
const fs = require('fs');
var csv = require("csvtojson");
// const spawn = require("child_process").spawn;
// const pythonProcess = spawn('python',["NovelTextProcessor.py"]);

var jsonParser = bodyParser.json()

const PORT = process.env.PORT || 8080
app.listen(PORT, () => {
    console.log(`Server listening on port ${PORT}...`)
})

// pythonProcess.stdout.on('data', (data) => {
//     console.log("python data=",data)
//     // Do something with the data returned from python script
//    });

app.post('/process',jsonParser,(req,res)=>{
    if (fs.existsSync(process.cwd()+"/frontend/dist/frontend/wordcloud.jpg")) {
        console.log("file exist")
    fs.unlinkSync(process.cwd()+"/frontend/dist/frontend/wordcloud.jpg",function(err){
        if(err) return console.log(err);
        console.log('file deleted successfully');
   });
}


    console.log("server post process")
    let text=req.body.content
    console.log("text=")
    console.log(text)
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
        //   console.log("The written has the following contents:");
        //   console.log(fs.readFileSync("movies.txt", "utf8"));
        }
    });

    const { spawn } = require('child_process');
    const pyProg = spawn('python3', ['./NovelTextProcessor.py']);

    pyProg.stdout.on('data', function(data) {
        console.log(data.toString());
        // res.write({'data':data});
        // res.end('end');
        // res.status(200).send({'data':data.toString()})
        // res.sendFile(process.cwd()+'/wordcloud.jpg')
        // csv()
        // .fromFile("temp.csv")
        // .then(function(jsonArrayObj){ //when parse finished, result will be emitted here.
        //     console.log("turn into json",jsonArrayObj); 
            // fs.writeFile(process.cwd()+"/frontend/dist/frontend/temp.json",jsonArrayObj,{
            //     encoding: "utf8",
            //     flag: "w",
            //     mode: 0o666
            //   },
            //   (err) => {
            //     if (err)
            //       console.log(err);
            //     else {
            //       console.log("File written successfully\n");
            //     //   console.log("The written has the following contents:");
            //     //   console.log(fs.readFileSync("movies.txt", "utf8"));
            //     }
            // })
            // res.send(jsonArrayObj)
        // })

        res.status(200).send({'filename':data.toString()})

        
    });
    // res.status(200).send()
})

app.use(express.static(process.cwd()+"/frontend/dist/frontend/"));
app.get('/*',(req,res)=>{
    console.log("test...")
    if (fs.existsSync(process.cwd()+"/frontend/dist/frontend/wordcloud.jpg")) {
    fs.unlinkSync(process.cwd()+"/frontend/dist/frontend/wordcloud.jpg",function(err){
        if(err) return console.log(err);
        console.log('file deleted successfully');
   });
}
    res.sendFile(process.cwd()+"/frontend/dist/frontend/index.html")
})

module.exports=app;