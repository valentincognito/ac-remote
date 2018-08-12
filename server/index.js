const SerialPort = require('serialport')
const Readline = SerialPort.parsers.Readline
const parser = new Readline()
const express = require('express')
const app = express()
const http = require('http').Server(app)
const io = require('socket.io')(http)

//global constant
const server_port = 3090

//server init
http.listen(server_port, "0.0.0.0", function(){
  console.log('listening on :', server_port)
})

//config
app.use(express.static('public'))

//routes
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/views/index.html')
})


io.on('connection', function (socket) {
   socket.on('new-command', function (data) {
     console.log(data);
     port.write(data + '\n');
   })
})


var port = new SerialPort('COM3', {
  autoOpen: false
})

port.pipe(parser);
port.on('data', logReceivedData);

function logReceivedData(data){
  //data = data.toString('hex');
  console.log('received: ' + data);
}


port.open(function (err) {
  if (err) {
    return console.log('Error opening port: ', err.message);
  }

  // setInterval(function () {
  //   console.log('data sent !');
  //   data = "1";
  //   port.write(data + '\n');
  // }, 1000);
});
