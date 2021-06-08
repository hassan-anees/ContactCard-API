'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ********************

let sharedLib = ffi.Library('./sharedLib', {
  'cardToJSON': [ 'string', ['string'] ],		//return type first, argument list second
									//for void input type, leave argument list is empty
  'getSummaryFromFile': [ 'string', [ 'string' ] ],	//int return, int argument
  //'cookieLife': [ 'string', [ ] ],	//int return, int argument

});


//Sample endpoint
app.get('/someendpoint', function(req , res){
  res.send({foo: "bar"});
});

//sends an array of filenames from uploads folder to put in dropdown
app.get('/fileNames', function(req , res) {
  let path = __dirname + "/uploads/";
  var dirPath = "uploads/";
  let fileArr = [];

  fs.readdir(dirPath, (err, files) => {
    files.forEach(file => {
      let ext = file.slice(-4);
      let secondExt = file.slice(-6);
      if (ext === '.vcf' || secondExt === '.vcard'){
        //DO EXTRA CHECKING, CALL THE getSummaryFromFile FUNCTION AND SEE WHAT IT RETURNS
        let obj = sharedLib.getSummaryFromFile(path+file);
        if (obj != null) {
          console.log('yoyooy' + ext);
          fileArr.push(file);
        }
      }
    });
    console.log(fileArr);
    console.log('the num of files in array ->' + fileArr.length);
    res.send(fileArr);
  })
});


//sends a json of a card, when given a filename,(req.query.info)
app.get('/generateCardView', function(req , res){
  //let fname = "testCard.vcf";
  //var e = document.getElementById("fileDropDown");
  //let strUser = e.options[e.selectedIndex].text;
  //let name = getDropDownFile();
  let path = __dirname + "/uploads/" + req.query.info;
  //let path = "testCard.vcf";
  let myStr = JSON.parse(sharedLib.cardToJSON(path));
  console.log(myStr);
  res.send(myStr);

});

//sends and array of JSONS of fileSummaries to fileLogDisplay
app.get('/fileLogDisplay', function(req , res) {
  let path = __dirname + "/uploads/";
  let fileArr = [];

  fs.readdir(path, (err, files) => {
    files.forEach(file => {
      let ext = file.slice(-4);
      let secondExt = file.slice(-6);
      if (ext === '.vcf' || secondExt === '.vcard'){
        console.log('yoyooy flogdisplay ->'+ file );
        //here just check if null is returned and do not add to the file log display
        let fileSummaryObj = JSON.parse(sharedLib.getSummaryFromFile(path+file));
        if(fileSummaryObj!= null){
          console.log(fileSummaryObj);
          fileArr.push(fileSummaryObj);
        }
      }
    });
    console.log(fileArr);
    console.log('the num of file for fileLog ->' + fileArr.length);
    res.send(fileArr);
  })
});




app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
