let app = document.getElementById("app");
app.width = 800;
app.height = 600;
let ctx = app.getContext("2d");
let w = null;

function make_environment(...envs) {
    return new Proxy(envs, {
        get(target, prop, receiver) {
            for (let env of envs) {
                if (env.hasOwnProperty(prop)) {
                    return env[prop];
                }
            }
            return (...args) => {console.error("NOT IMPLEMENTED: "+prop, args)}
        }
    });
}

// The WebAssembly.instantiateStreaming() static method compiles and instantiates
// a WebAssembly module directly from a streamed underlying source.
// This is the most efficient, optimized way to load Wasm code.
WebAssembly.instantiateStreaming(fetch('./bin/triangle.wasm'), {
  "env": make_environment({
    "atan2f": Math.atan2,
    "cosf": Math.cos,
    "sinf": Math.sin,
    "sqrtf": Math.sqrt,
  })
}).then(w0 => {
    w = w0;

    let prev = null;

    // Checkout https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame
    // The window.requestAnimationFrame() method tells the browser that you wish
    // to perform an animation. It requests the browser to call a user-supplied/
    // callback function prior to the next repaint. 
    // Below, dt = 8 milisec which is 120hz.
    function first(timestamp) {
      prev = timestamp;
      window.requestAnimationFrame(loop);
    }

    function loop(timestamp) {
      const dt = timestamp - prev;
      prev = timestamp;

      // The timestamp is a decimal number in miliseconds -> convert to seconds
      const pixels = w.instance.exports.render(dt*0.001);
      const buffer = w.instance.exports.memory.buffer;
      const image = new ImageData(new Uint8ClampedArray(buffer, pixels, app.width*app.height*4), app.width);
      ctx.putImageData(image, 0, 0);

      window.requestAnimationFrame(loop);
    }
    window.requestAnimationFrame(first);
  })
