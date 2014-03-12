module.exports = {
	naclUpdate: {
		command: 'chmod +x naclsdk && ./naclsdk update',
		options: {
			stdout: true,
			stderr: true,
			execOptions: {
				cwd: 'bower_components/nacl_sdk'
			}
		}
	},
	make: {
		command: 'make -f server/Makefile',
		options: {
			stdout: true,
			stderr: true
		}
	}
};
