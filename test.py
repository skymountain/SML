#! /usr/bin/env python

import os
import re
import subprocess
import sys

def test_cpp(cpp_file):
  args = ['g++', '-Wall', '-I.', cpp_file, '-lgtest', '-lpthread'];
  if subprocess.call(args) != 0:
    print "Error: can't compile %s" % (cpp_file)
    return False

  if subprocess.call(['./a.out']) != 0:
    print 'Error: test for %s failed' % (cpp_file)
    return False

  return True

def test_cpp_files(files):
  sum      = 0
  success  = 0
  fail     = 0

  for file in files:
    sum += 1
    if test_cpp(file):
      success += 1
    else:
      fail += 1

  print 'All Tests: %s, Success: %s, Failed: %s' % (
    sum,
    success,
    fail,
  )

  if fail:
    return 1
  else:
    return 0

def test_all(test_dir):
  regexp = re.compile('\.cpp$')
  cpp_files  = []

  for (parent_dir, subdirs, files) in os.walk(test_dir):
    for file in files:
      if regexp.search(file):
        cpp_files.append(os.path.join(parent_dir, file))

  return test_cpp_files(cpp_files)

def main():
  root = os.path.abspath(os.path.join(".", "test"))

  if len(sys.argv) > 1:
    cpp_files = map(lambda file: os.path.join(root, file), sys.argv[1:])
    code = test_cpp_files(cpp_files)
  else:
    code = test_all(root)

  return code

if __name__ == "__main__":
  sys.exit(main())
