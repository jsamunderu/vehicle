#!/usr/bin/env python

import cgi
import html_template
from jinja2 import Environment, FileSystemLoader

if __name__ == "__main__":
	output = html_template.genHtml("user_form.html", None)
	print("Content-type: text/html\n\n")
	print(output)
