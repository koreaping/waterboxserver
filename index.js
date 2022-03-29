var express = require("express");
var bodyParser = require("body-parser");
const {SerialPort }= require('serialport');
const serial = new SerialPort({
    path : '/dev/ttyUSB0',
    baudRate : 115200
});
process.setMaxListeners(50);

var app = express();
var port = 9000 

var Heater = false;
var H2o = false;

const {Gpio} = require('onoff');
const channel2 = new Gpio('17' , 'out');
const channel1 = new Gpio('18' ,'out');

setInterval(()=> {
    channel2.writeSync(Heater ? 0 : 1);
    channel1.writeSync(H2o ? 0 : 1);
    Heater = !Heater
    H2o = !H2o
}, 10000);

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended : false}))

const server = app.listen(port,() => {
    console.log("good")
});

const io = require('socket.io')(server);

io.on('connection' , (socket) =>{
    console.log("Connected Successfully " , socket.id);

    socket.on('disconnect' , ()=>{
        console.log("Disconnected ", socket.id);
    });

    serial.on('data' , function(data) {
        var dataList = data.toString().trim().split(" ");
        console.log(dataList)
        socket.emit("RTD" , dataList[0]);
        socket.emit("PH" , dataList[1]);
        socket.emit("DO" ,"29.5");
        if(Heater){
            socket.emit("Heater" , "true");
        }
        else{
            socket.emit("Heater","false");
        }
        if(H2o)
        {
            socket.emit("H2O" , "true");
        }
        else{
            socket.emit("H2O" , "false");
        }
    })
})
