function verifyPassword() {
	var pwd = document.getElementById("password").value;  
	if (pwd == "" || pwd.length < 4) {
		document.getElementById("errorMessage").innerHTML = "**Password length should be at least 5 characters";  
		const button = document.querySelector('submit')
		button.disabled = true
		button.disabled = false
		return false;  
	}
}
