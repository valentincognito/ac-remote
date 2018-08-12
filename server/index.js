const SerialPort = require('serialport')
const Readline = SerialPort.parsers.Readline
const parser = new Readline()
const express = require('express')
const app = express()
const http = require('http').Server(app)
const io = require('socket.io')(http)

//global constant
const port = 3090

//server init
http.listen(port, "0.0.0.0", function(){
  console.log('listening on :', port)
})

//config
app.use(express.static('public'))

//routes
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/views/index.html')
})

//socket
io.on('connection', function (socket) {
   socket.on('new-command', function (data) {
     console.log('new command:',data)
     arduino.write(data + '\n')
   })
})


var arduino = new SerialPort('/dev/ttyACM0', {
  autoOpen: false
})

arduino.pipe(parser)
arduino.on('data', logReceivedData)

function logReceivedData(data){
  console.log('received: ' + data)
}


arduino.open(function (err) {
  if (err) {
    return console.log('Error opening port: ', err.message)
  }
})
