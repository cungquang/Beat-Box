import('mime').then((mime) => {
    // Now you can use the 'mime' module
}).catch((err) => {
    console.error('Error loading mime module:', err);
});
const path = require('path');

//Construct response 200
const send200 = (response, filePath, fileContents) => {
    response.writeHead(200, {"Content-Type": mime.lookup(path.basename(filePath))});
    response.end(fileContents);
}

//Construct reponse for 404
const send404 = (response) => {
    response.writeHead(404, {'Content-Type':'text/plain'});
    response.write('Error 404: resources not found');
    response.end();
}

module.exports = { send200, send404 };

