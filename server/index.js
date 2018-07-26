var SerialPort = require('serialport')
var port = new SerialPort('/dev/tty.usbmodem1421')

port.write('0', function(err) {
  if (err) {
    return console.log('Error on write: ', err.message)
  }
  console.log('message written')
})

// port.on('data', function(data) {
//   console.log('data: ', data)
// })

// Open errors will be emitted as an error event
port.on('error', function(err) {
  console.log('Error: ', err.message)
})
