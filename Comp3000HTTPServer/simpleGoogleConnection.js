var fs = require('fs');
var readline = require('readline');
var google = require('googleapis');
var googleAuth = require('google-auth-library');
var signalParent = require('./userdefsignal');
var usersFileId = null;
var userSecret;
var root = "./Comp3000HTTPServer/";
var USERLIST_FILE_NAME = "usersList.txt";
// If modifying these scopes, delete your previously saved credentials
// at ~/.credentials/drive-nodejs-quickstart.json
var SCOPES = ['https://www.googleapis.com/auth/drive'];
var TOKEN_DIR = (process.env.HOME || process.env.HOMEPATH ||
    process.env.USERPROFILE) + '/.credentials/';
var TOKEN_PATH = TOKEN_DIR + 'drive-nodejs-quickstart.json';

//create a directory to store tmp file otherwise run startup
startup  = function(){
	fs.stat(root+"/tmp", function(err,stats){
			if(err) {
				console.log("No tmp dir make new one");
				fs.mkdir(root+"/tmp");
				readKeyFile();
			}
			else{
				readKeyFile();
			}
	});
}	


// Load client secrets from a local file.
function readKeyFile(){
	fs.readFile(root+'client_secret.json', function processClientSecrets(err, content) {
	if (err) {
		console.log('Error loading client secret file: ' + err);
		signalParent.failed();
		return;
	}
	// Authorize a client with the loaded credentials, then call the
	// Drive API.
	userSecret = JSON.parse(content);
	authorize(userSecret,  listFiles);
	});
}

/**
 * Create an OAuth2 client with the given credentials, and then execute the
 * given callback function.
 *
 * @param {Object} credentials The authorization client credentials.
 * @param {function} callback The callback to call with the authorized client.
 */
function authorize(credentials, callback) {
  var clientSecret = credentials.installed.client_secret;
  var clientId = credentials.installed.client_id;
  var redirectUrl = credentials.installed.redirect_uris[0];
  var auth = new googleAuth();
  var oauth2Client = new auth.OAuth2(clientId, clientSecret, redirectUrl);

  // Check if we have previously stored a token.
  fs.readFile(TOKEN_PATH, function(err, token) {
    if (err) {
      getNewToken(oauth2Client, callback);
    } else {
      oauth2Client.credentials = JSON.parse(token);
      callback(oauth2Client);
    }
  });
}

/**
 * Get and store new token after prompting for user authorization, and then
 * execute the given callback with the authorized OAuth2 client.
 *
 * @param {google.auth.OAuth2} oauth2Client The OAuth2 client to get token for.
 * @param {getEventsCallback} callback The callback to call with the authorized
 *     client.
 */
function getNewToken(oauth2Client, callback) {
  var authUrl = oauth2Client.generateAuthUrl({
    access_type: 'offline',
    scope: SCOPES
  });
  console.log('Authorize this app by visiting this url: ', authUrl);
  var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
  });
  rl.question('Enter the code from that page here: ', function(code) {
    rl.close();
    oauth2Client.getToken(code, function(err, token) {
      if (err) {
        console.log('Error while trying to retrieve access token', err);
        return;
      }
      oauth2Client.credentials = token;
      storeToken(token);
      callback(oauth2Client);
    });
  });
}

/**
 * Store token to disk be used in later program executions.
 *
 * @param {Object} token The token to store to disk.
 */
function storeToken(token) {
  try {
    fs.mkdirSync(TOKEN_DIR);
  } catch (err) {
    if (err.code != 'EEXIST') {
      throw err;
    }
  }
  fs.writeFile(TOKEN_PATH, JSON.stringify(token));
  console.log('Token stored to ' + TOKEN_PATH);
}

/**
 * Lists the names and IDs of up to 10 files.
 *
 * @param {google.auth.OAuth2} auth An authorized OAuth2 client.
 */
function listFiles(auth) {
  var service = google.drive('v3');
  service.files.list({
    auth: auth,
    fields: "nextPageToken, files(id, name)"
  }, function(err, response) {
    if (err) {
      console.log('The Google API returned an error: ' + err);
      return;
    }
    var files = response.files;
    if (files.length == 0) {
      console.log('No files found.');
    } else {//get all files on drive
      for (var i = 0; i < files.length; i++) {
        var file = files[i];
        //console.log('%s (%s)', file.name, file.id);
		if(file.name ==  USERLIST_FILE_NAME)
			usersFileId  = file.id;
      }
	  getUserFile(auth);
    }
  }); 
}
//get userlist file off google drive
  function  getUserFile(auth){
  var service = google.drive('v3');
  var files;
  if(usersFileId !=null){
	  var dest = fs.createWriteStream(root+'tmp/'+ USERLIST_FILE_NAME);
	  service.files.get({
		  auth: auth,
		  fileId: usersFileId,
		  alt: 'media'
	  })
	  .on('end', function(){
		  console.log("Done get list of users");
		  signalParent.ready();
	  })
	  .on('error', function(err){
		  console.log("Download UserList err:" + err);
		  signalParent.failed();
	  })
	  .pipe(dest);
  }
  else{
	    if (files == null) { //should grab username and ip from file
		    fs.writeFile(root+'tmp/'+ USERLIST_FILE_NAME, "Ben 100000000000\n", function(){
					uploadUserFile(auth);
			});			
		}
	}
}
//uploads new UserList to google drive
function uploadUserFile(auth){
    var service = google.drive('v3');
	var fileMetaData = {
		'name' : USERLIST_FILE_NAME,
	}
	var media = {
		body: fs.createReadStream(root+'tmp/'+ USERLIST_FILE_NAME)
	}
	service.files.create({
		auth:auth,
		resource: fileMetaData,
		media: media,
		fields : 'id'
	}, function(err,file){
		if(err) console.log("Upload UserList err :" +err);
	});	
	
}
module.exports.startup = startup;