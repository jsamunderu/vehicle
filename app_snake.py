#!/usr/bin/env python

import cgi
import html_template
from firebird.driver import connect
import bcrypt
import sys

views = {
	'1000': 'profile.html'
}

modes = {
	'0': 'view',
	'1': 'update'
}

def handleRequest():
	requestFields = cgi.FieldStorage()

	viewid = requestFields.getfirst("viewid", "0")
	mode = requestFields.getfirst("mode", "0")

	name = requestFields.getvalue('name')
	email = requestFields.getvalue('email')
	passwd = requestFields.getvalue('password')

	return processRequest(viewid, mode, name, email, passwd)

def processRequest(viewid, mode, name, email, passwd):  
	template = views.get(viewid)
	if template is None:
		return render("404.html", None)
	operation = modes.get(mode)
	if operation != 'view' and operation != 'update':
		return render("404.html", None)
	return processOperation(template, operation, name, email, passwd)

def processOperation(template, operation, name, email, passwd):
	salt = bcrypt.gensalt()
	hashedpwd = bcrypt.hashpw(passwd.encode(), salt)

	if operation == "update":
		updateUser(name, email, hashedpwd)

	user = fetchUser(email)
	return render("result.html", content=user)

def getProfileByUserId(userid):
	user = fetchLogonProfile(logonid)
	return render("result.html", content=user)

def fetchLogonProfile(logonId):
	sql = "SELECT USERID, NAME, EMAIL, LOGINID, PASSWD, USERTYPE FROM USERS WHERE LOGINID = ?"
	return execSQL(sql, (logonId,))

def fetchUser(email):
	sql = "SELECT USERID, NAME, EMAIL, LOGINID, PASSWD, USERTYPE FROM USERS WHERE EMAIL = ?"
	return execSQL(sql, (email,))

def updateUser(name, email, password):
	result = None
	user = fetchUser(email)
	if user is None:
		sql = "INSERT INTO USERS(NAME, EMAIL, LOGINID, PASSWD, USERTYPE) VALUES(?,?,?,?,?)"
		result = execSQL(sql, (name, email, 0, password, 0))
	else:
		sql = "UPDATE USERS SET NAME = ?, EMAIL = ?, PASSWD = ?"
		result = execSQL(sql, (name, email, password))
	return result

def execSQL(sqlStatement, values):
	status = None
	with connect('/var/lib/fleet/data.fdb', user='fleet', password='j8gtp3z') as con:
		cur = con.cursor()
		status = cur.execute(sqlStatement, values)
		if status is not None:
			for row in cur:
				status = row
		con.commit()
	return status

def render(template, content):
	output = html_template.genHtml(template, content)
	return output #.replace('"', '\\"')

if __name__ == "__main__":
	print(handleRequest())
