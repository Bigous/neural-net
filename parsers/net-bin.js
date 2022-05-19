/**
 * Select the files for which the parser should work.
 */
registerFileType((fileExt, filePath, fileData) => {
		// Check for the right file extension
		if (fileExt == 'net-bin') {
				return true;
		}
		return false;
});

function intToFloat(bits) {
	let sign = ((bits >>> 31) == 0) ? 1.0 : -1.0;
	let e = ((bits >>> 23) & 0xff);
	let m = (e == 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
	let f = sign * m * Math.pow(2, e - 150);
	return f;
}

function getFloat() {
	return intToFloat(getNumberValue());
}

/**
 * The parser to decode the file.
 */
registerParser(() => {
	addStandardHeader();
	// Network Values
	read(4);
	addRow('Inputs', getNumberValue(), 'Number of inputs neurons');
	read(4);
	addRow('Hidden', getNumberValue(), 'Number of hidden neurons on the first hidden layer');
	read(4);
	addRow('Output', getNumberValue(), 'Number of output neurons');
	read(4);
	addRow('Learning Rate', getFloat(), 'Value of the Learning Rate');

	// Network Hidden Weights
	read(8);
	const colsHidden = getNumberValue();
	addRow('Cols Hidden Weights', colsHidden, 'Number of cols on hidden weights');
	read(8);
	const rowsHidden = getNumberValue();
	addRow('Rows Hidden Weights', rowsHidden, 'Number of rows on hidden weights');
	const hiddenSize = colsHidden * rowsHidden;
	read(hiddenSize * 4);
	addRow('Hidden Weights');
	addDetails(() => {
		for (let i = 0; i < rowsHidden; i++) {
			for (let j = 0; j < colsHidden; j++) {
				read(4);
				addRow(`${i}-${j}`, getFloat());
			}
		}
	});

	// Network Outout Weights
	read(8);
	const colsOutout = getNumberValue();
	addRow('Cols Outout Weights', colsOutout, 'Number of cols on Outout weights');
	read(8);
	const rowsOutout = getNumberValue();
	addRow('Rows Outout Weights', rowsOutout, 'Number of rows on Outout weights');
	const OutoutSize = colsOutout * rowsOutout;
	read(OutoutSize * 4);
	addRow('Outout Weights');
	addDetails(() => {
		for (let i = 0; i < rowsOutout; i++) {
			for (let j = 0; j < colsOutout; j++) {
				read(4);
				addRow(`${i}-${j}`, getFloat());
			}
		}
	});


});
