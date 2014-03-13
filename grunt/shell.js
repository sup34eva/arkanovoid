module.exports = {
	update: {
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
		command: 'make',
		options: {
			stdout: true,
			stderr: true,
			execOptions: {
				cwd: 'server/'
			}
		}
	},
};
