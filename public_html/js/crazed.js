function invalidEmailLink(email) {
	var webEmail = email +' - at - crazedsanity - dot - com'
	var realEmail = email +'@crazedsanity.com';
	var link = 'mailto:' + email + '@127.0.0.1';
	var answer = confirm('FAKE ADDRESS:   ' + webEmail + '\n'
		+'REAL ADDRESS: ' + realEmail + '\n\n'
		+'Just so you understand what\'s about to happen if you decide to continue (by clicking "OK"): a compose screen will show up, where you can write a great email to me about how you\'ve been trying to contact me forever... unfortunately, since you can\'t seem to read between the lines, you\'ll probably never know that it will NOT GO ANYWHERE.\n\n'
		+'The IP address 127.0.0.1 is a LOOPBACK ADDRESS: when you send the email you\'re about to compose, your poor mailserver will get it & attempt to deliver to itself... and it will probably succeed, though the webmaster @ your isp will probably get kinda mad.\n\nClicking "OK" will allow you to continue... but you won\'t do that, because I just **KNOW** you read this message.')
	if(answer) {
		//they clicked "OK"
		alert('You have chosen POORLY.  Have fun wasting your time composing an email that will never be read... well, at least not by anybody at CrazedSanity.com.')
		window.open(link)
	} else {
		alert('You have chosen WISELY.  Maybe now you\'ll think twice about clicking on a link before reading it.')
	}
}
