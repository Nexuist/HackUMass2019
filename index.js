if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
  var video = document.getElementById("video");
  navigator.mediaDevices
    .getUserMedia({ video: { deviceId: { exact: "ygjrkJP80uMJUC2b+mh0nYTuvlf8lzO6AnRQl+Yan9w=" } } })
    .then(function(stream) {
      video.srcObject = stream;
      video.play();
    });
}
