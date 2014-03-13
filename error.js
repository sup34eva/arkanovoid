var errmsg = {
	500: 'Le serveur a rencontré une erreur',
	404: 'Page non trouvée',
	418: "I'm a teapot"
};

function ServerError(code, message) {
	if (code instanceof Error) {
		message = code.message;
		code = code.code;
	}
	this.message = message || "Server Error";
	this.code = code || 500;
}
ServerError.prototype = new Error();
ServerError.prototype.constructor = ServerError;

exports.send404 = function (req, res) {
	throw new ServerError(404, 'Not Found');
};
exports.log = function (err, req, res, next) {
	console.error(err.stack);
	next(err);
};
exports.format = function (err, req, res, next) {
	next(new ServerError(err));
};
exports.xhr = function (err, req, res, next) {
	if (req.xhr) {
		res.send(err.code, {
			error: err.message
		});
	} else {
		next(err);
	}
};
exports.show = function (err, req, res, next) {
	res.status(err.code);
	res.render('error', {
		code: err.code,
		title: err.code + ' ' + err.message,
		message: errmsg[err.code],
		path: req.path
	});
};
