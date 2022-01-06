#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "user_profile.h"

PyObject *g_mod = NULL;

const char* init_user_profile(void)
{
	PyObject *func = PyObject_GetAttrString(g_mod, "handleRequest");
	Py_DECREF(g_mod);

	PyObject* value = NULL;
	if(func && PyCallable_Check(func)) {
		value = PyObject_CallObject(func, NULL);

	} else {
		printf("ERROR: function getInteger()\n");
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

	const char* result = init_user_profile();
	if (result) {	
		int total = snprintf(tmp, sizeof(tmp) - 1, TOOL_BAR,
			CSS, JS, NAV);
		tmp[total] = '\0';
		total = snprintf(buffer, sizeof(buffer) - 1, tmp, result);
		buffer[total] = '\0';
		printf("%s%s", "Content-type: text/html\n\n", buffer);
	} else {
		printf("app_snacke.py: getRequestParameters() asUTF8 = (null)\n");
	}
}

void display_user_profile(const int userid)
{
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
