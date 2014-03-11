var SIZE = 16,
	SZSQ = Math.pow(SIZE, 2),
	container = $('<div class="container">'),
	binOutput = $('<input type="text" class="form-control" maxlength="' + SZSQ + '"/>'),
	hexOutput = binOutput.clone(),
	randoMenu = $('<div class="col-md-1 btn-group dropup"><button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown"><span class="caret"></span><span class="sr-only">Toggle Dropdown</span></button><ul class="dropdown-menu" role="menu"></ul></div>').prepend($('<button type="button" class="btn btn-primary">Random</button>').click(randBlocks)),
	result = "";

function clickHandler(e) {
	$(this).toggleClass('active');
	result = result.setCharAt(($(this).data('i') * SIZE) + $(this).data('j'), Number($(this).hasClass('active')).toString());
	binOutput.val(result);
	hexOutput.val(binToHex(result));
}

function setViewport (bin) {
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			var elem = $('[data-i=' + i + '][data-j=' + j + ']');
			if(Number(bin[(i * SIZE) + j]))
				elem.addClass('active');
			else
				elem.removeClass('active');
		}
	}
	binOutput.val(bin);
	hexOutput.val(binToHex(bin));
	result = bin;
}

function randHex(){
	var hex = "",
		char = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'A', 'B', 'C', 'D', 'E', 'F'];
	
	for(i = 0; i < SZSQ/4; i++)
		hex += char[Math.floor(Math.random() * char.length)];
	
	setViewport(hexToBin(hex));
}

function randBin(){
	var bin = "";
	
	for(i = 0; i < SZSQ; i++)
		bin += Math.round(Math.random());
		
	setViewport(bin);
}

function randBlocks(){
	var rand = "",
		amount = $(this).data('amount');
		
	if(!amount)
		amount = Math.floor(Math.avg(Math.random() * SZSQ, Math.random() * SIZE));
	
	for(i = 0; i < SZSQ; i++)
		rand += "0";
		
	do {
		var pos = Math.floor(Math.random() * rand.length);
		rand = rand.setCharAt(pos, '1');
	} while(rand.replace(/0/g, '').length < amount);
	
	setViewport(rand);
}

function randPi(){
	var rand = "";

	for(i = 0; i < SZSQ - (SIZE * 5); i++)
		rand += Math.floor(Math.sqrt(Math.pow(Math.random(), 2) + Math.pow(Math.random(), 2)));

	setViewport(rand);
}

function OR(s1, s2) {
	var ret = '';
	for(i = 0; i < Math.max(s1.length, s2.length); i++) {
		if(s1 && s2 && (s1[i] == '1' || s2[i] == '1'))
			ret += '1';
		else
			ret += '0';
	}
	return ret;
}

function randBBS(){
	var rand = "",
		prim = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97];

	do {
		var temprand = "";
		for(i = 0; i < SZSQ - (SIZE * 5); i++) {
			var mod = prim[Math.floor(Math.random() * prim.length)] % prim[Math.floor(Math.random() * prim.length)];
			temprand += Number(mod > 97/2);
		}
		rand = OR(rand, temprand);
	} while(rand.replace(/0/g, '').length < SIZE);

	setViewport(rand);
}

function reset() {
	$(this).button('loading');
	$('.btn-default.active').trigger('click');
	$(this).button('reset');
}

function invert() {
	$(this).button('loading');
	$('.btn-default').trigger('click');
	$(this).button('reset');
}

for(i = 0; i < SIZE; i++) {
	var row = $('<div class="row">').css('height', 'calc((100% - (34px*2))/' + SIZE + ')');
	for(j = 0; j < SIZE; j++) {
		var button = $('<button type="button" class="btn btn-default btn-lg col-md-1"></button>').click(clickHandler).attr('data-i', i).attr('data-j', j);
		button.appendTo(row);
		result += "0";
	}
	while(result.length % 4 != 0) {
		result += '0';
	}
	row.appendTo(container);
}

var dropdown = randoMenu.children('ul.dropdown-menu').append($('<li><a href="#">Random hex value</a></li>').click(randHex)).append($('<li><a href="#">Random binary value</a></li>').click(randBin)).append($('<li><a href="#">Random PI</a></li>').click(randPi)).append($('<li><a href="#">Random BBS</a></li>').click(randBBS)).append($('<li role="presentation" class="divider"></li><li role="presentation" class="dropdown-header">Fixed amount</li>'));
for(i = 20; i > 9; i--)
	dropdown.append($('<li data-amount="' + i + '"><a href="#">' + i + ' blocks</a></li>').click(randBlocks));

$(document.body).append($('<style>.col-md-1{width:'+ 100/SIZE + '%;}</style>')).append(container
	.append($('<div class="row">')
		.append(binOutput.addClass('col-md-' + SIZE).attr('inputmode', "numeric").val(result).change(function(e) {setViewport($(this).val());})))
	.append($('<div class="row">')
		.append(hexOutput.addClass('col-md-9').css('width', Math.floor(8 * 8.33333333) + '%').val(binToHex(result)).change(function(e) {setViewport(hexToBin($(this).val()));}))
		.append(randoMenu)
		.append($('<button class="btn btn-primary col-md-1" data-loading-text="Reseting...">Reset</button>').click(reset))
		.append($('<button class="btn btn-primary col-md-1" data-loading-text="Inverting...">Invert</button>').click(invert))));
