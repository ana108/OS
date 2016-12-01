var http = require('http').createServer(handler);
var fs = require('fs');
http.listen(3001);
console.log("Listening");
 
var googleDrive = require('./simpleGoogleConnection');
googleDrive.startup();
function handler(req,req){
  
}