# Cook the Books

## Overview
Cook the Books is an Vue application that uses WebAssembly to perform accounting transactions. The twist is that you can specify a "raw" and "cooked" transaction for reporting purposes. I cover how to build the application in my book, [Learn WebAssembly](https://www.packtpub.com/web-development/learn-webassembly).

You can see an example of the application running at http://cook-the-books.surge.sh.

## Technologies Used
- [Vue.js](https://vuejs.org/)
- [Browsersync](https://www.browsersync.io/)
- [UIkit](https://getuikit.com/)
- [accounting.js](http://openexchangerates.github.io/accounting.js/)
- [Lodash](https://lodash.com/)
- [D3.js](https://d3js.org/)
- [vue-numeric](https://www.npmjs.com/package/vue-numeric)

## Prerequisites
- Node.js >= 8.3
- Browser with WebAssembly and Module (`import`/`export`) support
- [Emscripten](https://kripken.github.io/emscripten-site/)
- GNU Make (optional)

## Instructions
### Setting Up a Mock API
You'll need a mock API to test out the application. I'm using jsonstore.io to get up and running quickly. Follow the steps below to create your own!

- Run the following command to populate a mock jsonstore.io API:
```
npm run setup-api
```
- Copy the endpoint output in the console and paste into `src/store/api.js`:
```javascript
// Before:
const API_URL = '[JSONSTORE.IO ENDPOINT]';

// After:
const API_URL = 'https://www.jsonstore.io/ba365164c3806df04e7e6f22321adfe8897b6f17eaa4f9ce6e298f1d123';
```

### Building the Application
Run the following command:
```
emcc lib/main.c -Os -s WASM=1 -s SIDE_MODULE=1 \
-s BINARYEN_ASYNC_COMPILATION=0 -o src/assets/main.wasm
```

Alternatively, you can run the following command:
```
make
```

### Running the Application
```
npm start
```

## Notes
- Unless otherwise specified, all terminal commands should be run from within this folder.
- To remove existing built files, run the following command:
```
make clean
```
