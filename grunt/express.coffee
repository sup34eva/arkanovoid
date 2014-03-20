module.exports =
	server:
		options:
			script: 'main.coffee'
			cmd: 'coffee'
			port: 3000
			node_env: 'development'
			logs:
				out: 'logs/out.log'
				err: 'logs/err.log'
