const Http = require('http');
const Fs = require('fs');
const Helper = require('./lib/helper_server');

//Basic configuration
const PORT = 3042;

const serverStatic = (response, absPath) => {
    "use strict";
    Fs.existsSync(absPath, function(exist) {
        if(exist) {
            Fs.readFile(absPath, function(err, data) {
                if(err) {
                    Helper.send404(response);
                } else {
                    Helper.send200(response, absPath, data);
                }
            });
        } else {
            Helper.send404(response);
        }
    });
}

//Setup server
const server = Http.createServer(function(request, response) {
    var filePath;
    if(request.url == "/") {
        filePath = 'public/index.html';
    } else {
        filePath = 'public' + request.url;
    }

    var absPath = './' + filePath;
    serverStatic(response, absPath);
});

server.listen(PORT, function() {
  console.log("Server listen on PORT " + PORT);  
})
