// let video = document.querySelector("#hud");

// function fs() {
//   if (!document.fullscreenElement) {
//     video.requestFullscreen();
//   }
//   document.exitFullscreen();
// }

// navigator.mediaDevices
//   .getUserMedia({ video: true })
//   .then(function(stream) {
//     video.srcObject = stream;
//     video.play();
//   })
//   .catch(err => console.log(err));

let app = new Vue({
  el: "#root",
  data: {
    direction: "D",
    joystickX: 128,
    joystickY: 128,
    motorL: 0.2,
    motorR: 0.4,
    diagnostic: "Connecting to chair..."
  },
  mounted: function() {
    // setInterval(function() {
    //   app.direction = app.direction == "N" ? "D" : "N";
    // }, 1000);
  },
  computed: {
    /* 
        The simulated chair is 128x128px as opposed to the 0-255 range joystick values can be, so divide them in half and offset them by half their width so they are perfectly centered using absolute positioning in the DOM
      */
    joystickXDOM: function() {
      return this.joystickX / 2 - 10;
    },
    joystickYDOM: function() {
      return this.joystickY / 2 - 10;
    }
  }
});
