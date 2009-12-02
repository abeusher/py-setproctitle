
#include "Python.h"
#include "spt_status.h"

/* defined in Modules/main.c but not publically declared */
void Py_GetArgcArgv(int *argc, char ***argv);

/* ----------------------------------------------------- */

static char spt_setproctitle__doc__[] =
"Change the process title."
;

static PyObject *
spt_setproctitle(PyObject *self /* Not used */, PyObject *args)
{
    const char *title;

    if (!PyArg_ParseTuple(args, "s", &title))
        return NULL;

    set_ps_display(title, true);

    Py_INCREF(Py_None);
    return Py_None;
}

static char spt_getproctitle__doc__[] =
"Get the current process title."
;

static PyObject *
spt_getproctitle(PyObject *self /* Not used */, PyObject *args)
{

    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    int tlen;
    const char *title;
    title = get_ps_display(&tlen);

    return Py_BuildValue("s#", title, tlen);
}

/* List of methods defined in the module */

static struct PyMethodDef spt_methods[] = {
    {"setproctitle",    (PyCFunction)spt_setproctitle,  METH_VARARGS,   spt_setproctitle__doc__},
    {"getproctitle",    (PyCFunction)spt_getproctitle,  METH_VARARGS,   spt_getproctitle__doc__},

    {NULL,   (PyCFunction)NULL, 0, NULL}        /* sentinel */
};


/* Initialization function for the module (*must* be called initsetproctitle) */

static char setproctitle_module_documentation[] =
"Allow customization of the process title."
;

void
initsetproctitle()
{
    PyObject *m;

    /* Create the module and add the functions */
    m = Py_InitModule4("setproctitle", spt_methods,
        setproctitle_module_documentation,
        (PyObject*)NULL,PYTHON_API_VERSION);

    /* Initialize the process title */
    int argc;
    char **argv;
    Py_GetArgcArgv(&argc, &argv);
    save_ps_display_args(argc, argv);

    /* TODO: there is no call to `init_ps_display()`
     * without it the first call to getproctitle returns a bad value
     * (the concatenation of argv and environ)
     */

    /* Check for errors */
    if (PyErr_Occurred())
        Py_FatalError("can't initialize module setproctitle");
}

