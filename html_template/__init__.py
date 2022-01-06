#!/usr/bin/env python
from jinja2 import Environment, FileSystemLoader

def genHtml(template, content):
	file_loader = FileSystemLoader('templates')
	env = Environment(loader=file_loader)

	template = env.get_template(template)

	return template.render(content=content)
