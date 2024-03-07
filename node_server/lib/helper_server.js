import('mime').then((mime) => {
    // Now you can use the 'mime' module
}).catch((err) => {
    console.error('Error loading mime module:', err);
});
const path = require('path');


module.exports = { send200, send404 };

