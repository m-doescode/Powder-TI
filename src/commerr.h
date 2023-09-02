#include <stdio.h>

#ifdef TI84
    // #define throw(x, ...) { dbg_printf(__CONCAT("LINE %d: ", x), __LINE__, __VA_ARGS__); abort(); }
    inline void throw_(unsigned int lineno, const char * format, ...) {
        va_list argptr;
        va_start(argptr, format);

        sprintf(dbgout, "LINE %d: ", lineno);
        vsprintf(dbgout, format, argptr);
        sprintf(dbgout, "\n");

        va_end(argptr);
        
        abort();
    }

    #define throw(...) throw_(__LINE__, __VA_ARGS__)
#else
    #define throw(x) throw std::string(x)
#endif