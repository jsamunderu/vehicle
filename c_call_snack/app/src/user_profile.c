#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <string.h>

#include "user_profile.h"

PyObject *g_mod = NULL;

int read_file(const char* filename, char* buf, size_t buf_size)
{
	char tmp_buf[64];
	if (filename == NULL || strlen(filename) < 1) {
		return -1;
	}
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		return -1;
	}

	int n = -1, count = 0, tmp_buf_size = sizeof(tmp_buf);
	while ((n = fread(tmp_buf, 1, tmp_buf_size, file)) != -1) {
		if ((count + n) > buf_size) {
			n = buf_size - count;
			memcpy(&buf[count], tmp_buf, n);
			count += n - 1;
			break;
		}
		memcpy(&buf[count], tmp_buf, n);
		count += n;
		if (n < tmp_buf_size) {
			break;
		}
	}
	buf[count] = '\0';
	return count;
}

const char* init_user_profile(void)
{
	PyObject *func = PyObject_GetAttrString(g_mod, "handleRequest");
	Py_DECREF(g_mod);

	PyObject *value = NULL;
	if(func && PyCallable_Check(func)) {
		value = PyObject_CallObject(func, NULL);

	} else {
		printf("ERROR: function handleRequest()\n");
		Py_Finalize();
		return NULL;
	}

	if (!value) {	
		printf("app_snacke.py: init_user_profile() = (null)\n");
	}

	return PyUnicode_AsUTF8(value);
}

void display_toolbar(void)
{
	char tmp[16384], buffer[16384];
	char tool_bar[1024], tool_bar_nav[1024], tool_bar_css[1024],
		tool_bar_js[1024];

	int flag = read_file("static/toolbar.css",
		tool_bar_css, sizeof(tool_bar_css));
	if (flag == -1) {
		printf("Error reading file! static/toolbar.css");
		return;
	}

	flag = read_file("static/toolbar_fragment.html",
		tool_bar, sizeof(tool_bar));
	if (flag == -1) {
		printf("Error reading file! static/toolbar_fragment.html");
		return;
	}


	flag = read_file("static/toolbar_fragment.js",
		tool_bar_js, sizeof(tool_bar_js));
	if (flag == -1) {
		printf("Error reading file! static/toolbar_fragment.js");
		return;
	}

	flag = read_file("static/toolbar_nav_fragment.html",
		tool_bar_nav, sizeof(tool_bar_nav));
	if (flag == -1) {
		printf("Error reading file! static/toolbar_nav_fragment.html");
		return;
	}

	const char *result = init_user_profile();
	if (result) {	
		int total = snprintf(tmp, sizeof(tmp) - 1, tool_bar,
			tool_bar_css, tool_bar_js, tool_bar_nav);
		tmp[total] = '\0';
		total = snprintf(buffer, sizeof(buffer) - 1, tmp, result);
		buffer[total] = '\0';
		printf("%s%s", "Content-type: text/html\n\n", buffer);
	} else {
		printf("app_snacke.py: handleRequest() asUTF8 = (null)\n");
	}
}

const char* display_user_profile(const int userid)
{
	PyObject *func = PyObject_GetAttrString(g_mod, "getProfileByUserId");
	Py_DECREF(g_mod);

	PyObject *value = NULL;
	if(func && PyCallable_Check(func)) {
		value = PyObject_CallObject(func, NULL);
	} else {
		printf("ERROR: function getProfileByUserId()\n");
		Py_Finalize();
		return NULL;
	}

	if (!value) {	
		printf("app_snacke.py: getProfileByUserId() = (null)\n");
	}

	return PyUnicode_AsUTF8(value);
}

int main(int argc, char **argv)
{
	setenv("PYTHONPATH", ".", 1);

	Py_Initialize();

	Py_Initialize();
	g_mod = PyImport_ImportModule("app_snake");
	if (!g_mod) {
		printf("Module not loaded\n");
		Py_Finalize();
		return EXIT_FAILURE;
	}

	display_toolbar();

	Py_Finalize();
	return EXIT_SUCCESS;
}
