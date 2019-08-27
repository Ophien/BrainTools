[docs]: https://github.com/Ophien/BrainTools/docs/documentation.md

# Users Guide

It is fairly simple to use Brain Tools library. Make sure you have followed all instructions to install it in your system or other development environment. 

Here is a base program that incorporates the Brain Tools library and calls for a check function to see if it is properly intalled and working.

```
#include <libbraintools/BrainTools.h>

int main(int argc, int char* argv[])
{
    // brain tools object
    braintools::BrainTools* pBrainToolsObject;

    // create object
    pBrainToolsObject = new braintools:BrainTools("log.out", "profiling.out");

    // test object
    pBrainToolsObject->checkStatus();

    return 0;
}
```

In order to compile the aforementioned piece of code save it into a file called main.cpp and do the following. 

```
g++ main.cpp -L /usr/local/lib/libbraintools -lbraintools
```

This command will generate the output file a.out. Run the following to check if everything is ok and that the program is running.

```
./a.out
```

You should see

```
Status OK
```

If that happens, congratulations the library is ready to be used. Please see our [Documentation](docs) to learn how to use it according to your needs.