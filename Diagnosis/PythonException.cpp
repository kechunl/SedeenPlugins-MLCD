
#include <sstream>
#include <memory>
#include "python.h"

#include "PythonException.h"
#include "PythonObject.h"


namespace sedeen {
namespace python {

PythonException::PythonException( const char* pExceptionMsg )
    : runtime_error( pExceptionMsg )
{
}

PythonException::PythonException( const std::string& errorMsg , const std::string& excType , const std::string& excValue , const std::string& excTraceback )
    : runtime_error( errorMsg )
    , excType_(excType) , excValue_(excValue) , excTraceback_(excTraceback)
{
}

void
PythonException::translateException()
{
    // get the Python error details
    std::string excType , excValue , excTraceback ;
    PyObject *pExcType , *pExcValue , *pExcTraceback ;
    PyErr_Fetch( &pExcType , &pExcValue , &pExcTraceback ) ;
    int line;
    char lineno[50];
    // Error type
    if ( pExcType != NULL )
    {
        PythonObject obj( pExcType , true ) ;
        std::shared_ptr<PythonObject> attrObj( obj.py_GetAttr( "__name__" ) ) ;
        excType = attrObj->py_ReprVal() ;
    }
    // Errow Message
    if ( pExcValue != NULL )
    {
        PythonObject obj( pExcValue , true ) ;
        excValue = obj.py_ReprVal() ;
    }
    if ( pExcTraceback != NULL )
    {
        PythonObject obj( pExcTraceback , true ) ;
        excTraceback = obj.py_ReprVal() ;

        PyTracebackObject* traceback = (PyTracebackObject*)pExcTraceback;
        while (traceback->tb_next != NULL)
            traceback = traceback->tb_next;

        line = traceback->tb_lineno;
        sprintf(lineno, "%d", line);
        //const char* filename = PyUnicode_AsUTF8(traceback->tb_frame->code->co_filename);
    }

    // translate the error into a C++ exception
    std::stringstream buf ;
    buf << (excType.empty() ? "???" : excType) ;
    if ( ! excValue.empty() )
        buf << ": " << excValue ;
    if ( ! excTraceback.empty())
        buf << "line " << lineno;
    throw PythonException( buf.str() , excType , excValue , excTraceback ) ;
}

} // namespace python
}

