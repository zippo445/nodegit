import Options, Utils
from subprocess import Popen
import os, shutil, platform
from os import system
from os.path import exists, abspath

VERSION = '0.0.6'
APPNAME = 'nodegit'
srcdir = '.'
blddir = 'build'

def set_options(opt):
  opt.tool_options('gcc')
  opt.tool_options('compiler_cxx')

def configure(conf):
  import preproc
  preproc.go_absolute = True

  conf.check_tool('gcc')
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

  os.chdir('vendor/libgit2')

  # Change this later...
  Popen('mkdir build', shell=True).wait()
  os.chdir('build')
  Popen('cmake -DBUILD_TESTS=OFF -DTHREADSAFE=ON .. ', shell=True).wait()

  conf.env.append_value('LIBPATH_GIT2', abspath('.'))
  conf.env.append_value('LIB_GIT2', 'git2')

def build(bld):
  try: os.chdir('vendor/libgit2/build')
  except: pass
  Popen('cmake --build .', shell=True).wait()

  os.chdir('../../')

  main = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  main.target = 'nodegit'
  main.source = 'src/base.cc src/sig.cc src/blob.cc src/error.cc src/object.cc src/reference.cc src/repo.cc src/commit.cc src/oid.cc src/revwalk.cc src/tree.cc src/tree_entry.cc'
  main.rpath = abspath('libgit2/build')
  main.uselib = 'GIT2'
