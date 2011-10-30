#!/usr/bin/python

import os
import cxxtest

env = Environment( ENV = {
  'PATH' : "/usr/lib/colorgcc/bin:" + os.environ['PATH'],
  'TERM' : os.environ['TERM'],
  'HOME' : os.environ['HOME'],
  'CCACHE_PATH' : "/usr/bin",
  'CXX' : '/usr/lib/colorgcc/bin/g++',
  'CCACHE_LOGFILE' : '/tmp/denes.txt'
  #'tools' : ['default', 'cxxtest']
})

env['CXXFLAGS'] = '-Wall -Wextra -pedantic -Wshadow -Weffc++ -std=c++0x  -ggdb'
env['CPPPATH'] = 'include'
env['tools'] = ['default', 'cxxtest']

#from pprint import pprint
#pprint (vars(env))

env.SharedLibrary('cpp_utils',
            Glob('src/*.cpp'),
            LIBS =  ['pthread', 'rt'] )

#env.Program('test/test',
            #'test/generated_main.cpp',
            #LIBS =  ['cpp_utils'],
            #LIBPATH = '.')

env.CxxTest('target')