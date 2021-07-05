
var mqtt = require('mqtt');
var Opciones = {
  host: "localhost",
  port: 1884,
  protocol: "ws",
  clientId: "MQTT"
};
var client = mqtt.connect(Opciones);
const SerialPort = require('serialport');
const port = new SerialPort('COM11',{
  baudRate: 9600
});
client.on('connect', function () {
  client.subscribe('banda/frutas', function (err) {
    console.log("MQTT Activado");
  });
});

client.on('message', function (topic, message) {
  let Mensaje = message.toString();
  if (Mensaje == "Encender"){
    console.log("Encender foco");
    port.write("H");
  }else if (Mensaje == "Apagar"){
  console.log("Apagar foco");
  port.write("L");
  }
});
