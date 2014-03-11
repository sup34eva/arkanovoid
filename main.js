var express = require('express'),
	app = express(),
	routes = require('./routes.json'),
	git = require('gift');

app.use(express.compress());
app.use(express.logger('tiny'));
app.use(require('connect-livereload')({
	port: 80
}));
app.use(app.router);
app.use(express.static('dist'));
app.use('/server', express.static('server'));
app.use('/bower', express.static('bower_components'));
app.use('/lvledit', express.static('lvledit'));

app.set('view engine', 'jade');

function handler(req, res) {
	res.render(routes[req.route.path].template, routes[req.route.path].args);
}

for(var i in routes)
	app.get(i, handler);

/*app.get('/livereload.js', function(req, res) {
	require('http').get('http://localhost:35729/livereload.js').on('response', function (pres) {
		pres.pipe(res);
		res.writeHead(pres.statusCode, pres.headers);
	});
});*/

app.all('/push', function (req, res) {
	var repo = git(__dirname);

	console.log('Push request');

	repo.sync(function(err){
		if(err) {
			res.send(500);
			console.error(err);
		} else {
			res.send(200);
			console.log('Push finished');
		}
	});
});

function kiddies (req, res) {
	res.send(418, "I'm a teapot");
}

app.get(/^\/w00tw00t\.at\.blackhats\.romanian\.anti-sec/, kiddies);
app.get('/phpMyAdmin/scripts/setup.php', kiddies);
app.get('/phpmyadmin/scripts/setup.php', kiddies);
app.get('/pma/scripts/setup.php', kiddies);
app.get('/myadmin/scripts/setup.php', kiddies);
app.get('/MyAdmin/scripts/setup.php', kiddies);

var port = process.env.port || 3000;
app.listen(port, function() {
	console.log('Listening on port ' + port);
});
