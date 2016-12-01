
function signalReady(){
	console.log(process.argv[2]);
	process.kill(process.argv[2], 'SIGCONT');
}

function signalFailed(){
	console.log(process.argv[2]);
	process.kill(process.argv[2], 'SIGUSR2');
	process.exit(1);
}

module.exports.ready = signalReady;

module.exports.failed = signalFailed;