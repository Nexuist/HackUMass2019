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
    direction: "N",
    joystickX: 128,
    joystickY: 128,
    motorL: 0,
    motorR: 0,
    diagnostic: "Connecting to chair..."
  },
  mounted: function() {
    ws = new WebSocket("ws://localhost:1337");
    ws.onopen = e => {
      this.diagnostic = "Connected!";
    };
    ws.onmessage = msg => {
      let data = msg.data;
      if (data.startsWith("DATA:")) {
        data = data.substring(5);
        let keys = data.split("|");
        let armed = +keys[0];
        let goingForward = +keys[1];
        let joystickX = +keys[2];
        let joystickY = +keys[3];
        let motorL = +keys[4];
        let motorR = +keys[5];
        if (armed && goingForward) this.direction = "D";
        if (armed && !goingForward) this.direction = "R";
        if (!armed) this.direction = "N";
        this.joystickX = joystickX;
        this.joystickY = joystickY;
        this.motorL = motorL / 255;
        this.motorR = motorR / 255;
      }
    };
    ws.onerror = err => {
      this.diagnostic = "Connection Failed";
      console.error(err);
    };
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
