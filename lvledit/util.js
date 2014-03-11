String.prototype.setCharAt=function(index, character) {
	//console.log(index);
    return this.substr(0, index) + character + this.substr(index+1);
}

Math.avg = function() {
	var sum = 0;
	
	for(i = 0; i < arguments.length; i++)
		sum += arguments[i];
		
	return sum/arguments.length;
};

Math.roundBy = function(a, b) {
	a = Math.round(a);
	b = Math.round(b);
	while(a % b != 0) {
		if(a > (Math.round(a/b)*b) + (b/2))
			a++;
		else
			a--;
	}
	return a;
};

Math.ceilBy = function(a, b) {
	a = Math.round(a);
	b = Math.round(b);
	var k = b;
	while(a % b != 0 && k > 0) {
		console.log(a, b, a%b);
		a++;
		k--;
	}
	return a;
};

function binToHex(s) {
	if(s.length % 4 != 0) {
		var newlen = Math.ceilBy(s.length, 4);
		for(i = 0; i < newlen; i++)
			s += '0';
	}
    var i, k, part, accum, ret = '';
    for (i = s.length-1; i >= 3; i -= 4) {
        part = s.substr(i+1-4, 4);
        accum = 0;
        for (k = 0; k < 4; k++) {
            if (part[k] !== '0' && part[k] !== '1') {
                return NaN;
            }
            accum = accum * 2 + parseInt(part[k], 10);
        }
        if (accum >= 10) {
            ret = String.fromCharCode(accum - 10 + 'A'.charCodeAt(0)) + ret;
        } else {
            ret = String(accum) + ret;
        }
    }
    if (i >= 0) {
        accum = 0;
        for (k = 0; k <= i; k++) {
            if (s[k] !== '0' && s[k] !== '1') {
                return NaN;
            }
            accum = accum * 2 + parseInt(s[k], 10);
        }
        ret = String(accum) + ret;
    }
    return ret;
}

function hexToBin(s) {
    var i, k, part, ret = '';
    var lookupTable = {
        '0': '0000', '1': '0001', '2': '0010', '3': '0011', '4': '0100',
        '5': '0101', '6': '0110', '7': '0111', '8': '1000', '9': '1001',
        'a': '1010', 'b': '1011', 'c': '1100', 'd': '1101',
        'e': '1110', 'f': '1111'
    };
    for (i = 0; i < s.length; i++) {
        if (lookupTable.hasOwnProperty(s[i])) {
            ret += lookupTable[s[i].toLowerCase()];
        } else {
            return NaN;
        }
    }
    return ret;
}
