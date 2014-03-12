module.exports = {
	options: {},
	server: {
		options: {
			script: 'main.js',
			port: 3000,
			node_env: 'development',
			logs: {
				out: 'logs/out.log',
				err: 'logs/err.log'
			}
		}
	}
};
