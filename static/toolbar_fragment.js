function ready(callback) {
	if (document.readyState != 'loading')
		callback();
	else if (document.addEventListener)
		document.addEventListener('DOMContentLoaded', callback);
	else
		document.attachEvent('onreadystatechange', function() {
			if (document.readyState == 'complete')
				callback();
		});
}
ready(function() {
	var iframe = document.createElement('iframe');
	document.getElementById(\"content\").appendChild(iframe);
	iframe.setAttribute(\"style\",\"height:100%;width:100%;\");
	iframe.contentWindow.document.open();
	iframe.contentWindow.document.write(`%s`);
	iframe.contentWindow.document.close();
});
