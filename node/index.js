
var mqtt = require('mqtt');
var Opciones = {
  host: "localhost",
  port: 1884,
  protocol: "ws",
  clientId: "MQTT"
};
var client = mqtt.connect(Opciones);
const SerialPort = require('serialport');
const port = new SerialPort('COM3',{
  baudRate: 9600
});
client.on('connect', function () {
  client.subscribe('banda/frutas', function (err) {
    console.log("MQTT Activado");
  });
});

client.on('message', function (topic, message) {
  let Mensaje = message.toString();
  if (Mensaje == "Mandarina"){
    console.log("Selecionado Mandarina");
    port.write("M");
  }else if (Mensaje == "Pera"){
  console.log("Selecionado Pera");
  port.write("P");
}else if (Mensaje == "Uva"){
  console.log("Selecionado Uva");
  port.write("U");
  }
});
