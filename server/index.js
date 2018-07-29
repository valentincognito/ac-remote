// const SerialPort = require('serialport');
// const Readline = SerialPort.parsers.Readline;
// const parser = new Readline();
//
// var port = new SerialPort('/dev/tty.usbmodem1421', {
//   autoOpen: false
// })
//
// port.pipe(parser);
// port.on('data', logReceivedData);
//
// function logReceivedData(data){
//   data = data.toString('hex');
//   console.log('received: ' + data);
// }
//
//
// port.open(function (err) {
//   if (err) {
//     return console.log('Error opening port: ', err.message);
//   }
//
//   setInterval(function () {
//     console.log('data sent !');
//     data = "led=2";
//     port.write(data + '\n');
//   }, 1000);
// });
//
// // port.on('data', function(data) {
// //   var buff = new Buffer(data, 'utf8')
// //   console.log('data received: ' + buff.toString('hex'))
// // })



var five = require("johnny-five");
var board = new five.Board({
  repl: false
});

board.on("ready", function() {
  console.log('ready');

  setInterval(function () {
    console.log('data sent !');
    board.digitalWrite(12, 0);
    //board.analogWrite(9, 1);
  }, 2000);

});
