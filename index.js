var video = document.getElementById("video");
navigator.mediaDevices
  .getUserMedia({ video: true })
  .then(function(stream) {
    video.srcObject = stream;
    video.play();
  })
  .catch(err => console.log(err));
