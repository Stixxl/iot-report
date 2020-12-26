const SerialPort = require('serialport');
const ConsoleReader = require('readline');

const Readline = SerialPort.parsers.Readline;
const parser = new Readline();


var first=true;

var myArgs=process.argv.slice(2);
console.log('myArgs: ', myArgs);
if (myArgs.length==0){
	console.log("Error: serial port not specified.");
	process.abort();
}

if (myArgs.length>2){
	console.log("Error: too many arguments");
	process.abort();
}

const port = new SerialPort(myArgs[0]);

const rl = ConsoleReader.createInterface({
  input: process.stdin,
  prompt: "hallo>"
});


try{
	port.pipe(parser);
	parser.on('data', handleDeviceString);
}
catch(e) {
  console.log(e);
  process.abort();
}

//handle input from terminal
rl.on('line', (input) => {
  console.log(`Received: ${input}`);
  input=input+"\n";
  port.write(input);
});


//If only the serial port is given, send current time only.
//If a file name is given, open file and read the commands and set he time to the time of the first command - 30 seconds
if (myArgs.length==2){
	//read and process file given in myArgs[1]
	processFile(myArgs[1],port);
	sendCurrentTime(port);
} else {
	sendCurrentTime(port);
}


//Handles lines printed in device to serial connection
//Echos all lines and handles SEND commands by pushing them to the MQTT gateway of the IoT platform
function handleDeviceString(str){
  console.log('>>> '+str);
  if(str.startsWith("<<SEND:")){

	var payload = {"username": "username",
				   "<sensor Name>": "<sensor value>",
				   "device_id": "<deviceID>",
				   "timestamp": "<timestamp in mseconds>"};

	//myObj = { "name":"John", "age":30, "car":null };
	var payload = str.substring(7, str.length-3).split(":")

	//<<SEND:1609024921:0:>>
	console.log("Hier zu mqtt senden");
  }
}


function sendCurrentTime(port) {
    var utc = new Date();

	var hours = utc.getHours();
	var minutes = utc.getMinutes();
	var seconds = utc.getSeconds();
	var day =utc.getDate();
	var month = utc.getMonth()+1;
	var year = utc.getFullYear();
  
  
	var tmp = utc.getTime()/1000;
	var time=Math.trunc(tmp)+7200;
	var timeString = "0:0:"+String(time)+"\n";

	port.write(timeString);
};

function processFile(fileName,port){
	var fs = require('fs');
	var buffer = fs.readFileSync(__dirname + "\\"+ fileName);
	port.write(buffer.toString());
}


process.on('uncaughtException', function (exception) {
	console.log(exception); // to see your exception details in the console
	// if you are on production, maybe you can send the exception details to your
	// email as well ?
});